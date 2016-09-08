#include "../library.h"
#include "../includeAll.h"
#include "pageAtelier.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#define PAGEATELIERMAIN_SLIDELIMIT 8

enum pageAtelierMainPuppetActionType{
	PAGEATELIERMAINPUPPETACTIONTYPE_NONE = 0,
	PAGEATELIERMAINPUPPETACTIONTYPE_CREATE,
	PAGEATELIERMAINPUPPETACTIONTYPE_HARVEST
};

// ページカートリッジ構造体
struct pageCartridgeAtelierMain{
	struct pageCartridge super;
	struct pageAtelierStatus *stat;
	struct popupCartridge *popupMeisterList;
	struct popupCartridge *popupPuppetList;
	struct popupCartridge *popupGacha;
	struct popupCartridge *popupGachaResult;
	struct popupCartridge *popupItemSelect;
	struct popupCartridge *popupItemUse1;
	struct popupCartridge *popupItemUse2;
	struct popupCartridge *popupTutorial;
	int stepTutorial01;
	int stepTutorial02;
	int stepTutorial03;
	int stepTutorial04;
	int stepTutorial05;
	int stepTutorial11;
	int stepTutorial12;
	int stepTutorial13;

	struct hknwMap *mapCanvas;
	struct hknwMap *mapAdd;

	struct buttonAtelierMeister *btnMeister[STORAGEATELIER0001_WORKSPACEXMAX];
	struct partsButton btnPuppetCage;
	struct partsButtonBox btnGacha;
	struct partsButtonBox btnItem;
	struct partsButtonBox btnBack;

	struct e3dImage slideArrow;
	struct e3dImage puppetCage;
	struct e3dImage tutorialArrow1;
	struct e3dImage tutorialArrow2;
	struct e3dImage tutorialArrow3;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	struct{int size; int step;} canvas;
	struct{bool loading; int size; int step;} workspace;
	int puppetCageStep;

	// パペットアクション
	struct{
		enum pageAtelierMainPuppetActionType type;
		int step;
	} puppetAction[STORAGEATELIER0001_CANVASXMAX][STORAGEATELIER0001_CANVASYMAX];

	// 処理落ち防止の効果音カウント
	struct{
		int harvest1;
		int harvest2;
	} soundSeCount;

	// ワールド行列
	struct engineMathMatrix44 worldMat;
	// 画面移動
	double slide;
	double slideNext;
	double slideArrowSlide;
	double slideArrowSlideNext;
	// 画面回転
	double rotv;
	double roth;
	double dist;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageCartridgeAtelierMain *this){
	this->canvas.size = storageAtelierGet()->canvasSize;
	this->workspace.size = storageAtelierGet()->workspaceSize;

	// マイスターボタン
	for(int i = 0; i < STORAGEATELIER0001_WORKSPACEXMAX; i++){this->btnMeister[i] = buttonAtelierMeisterInit(storageAtelierGet()->workspace[i].meisterKind);}
	// パペット容器ボタン
	partsButtonInit(&this->btnPuppetCage);
	// ガチャボタン作成
	partsButtonBoxInit(&this->btnGacha);
	// アイテムボタン作成
	partsButtonBoxInit(&this->btnItem);
	// 戻るボタン作成
	partsButtonBoxInit(&this->btnBack);

	// 画面回転
	this->rotv = 3.1415 / 180 * 30;
	this->roth = 3.1415 / 180 * 30;
	this->dist = 8;
	// 画面移動
	this->slideNext = this->canvas.size * 0.5;
	this->slide = this->slideNext + 2.0;
	this->slideArrowSlideNext = this->slideNext;
	this->slideArrowSlide = this->slideArrowSlideNext;
}

// ----------------------------------------------------------------

// 収穫の計算
static void calcHarvest(struct pageCartridgeAtelierMain *this){
	// タッチ状態の確認
	struct engineCtrlTouch *t = engineCtrlTouchGet(ENGINECTRLTOUCHID_TEST1);
	if(t != NULL){
		if(t->dn){
			int touchX = -1;
			int touchY = -1;
			double touchDepth = 1;
			bool within = false;
			for(int i = 0; i < this->canvas.size; i++){
				for(int j = 0; j < STORAGEATELIER0001_CANVASYMAX; j++){
					if(storageAtelierGet()->canvas[i][j].puppetKind < 0){continue;}
					enum pageAtelierMainPuppetActionType type = this->puppetAction[i][j].type;
					if(type == PAGEATELIERMAINPUPPETACTIONTYPE_CREATE){continue;}
					if(type == PAGEATELIERMAINPUPPETACTIONTYPE_HARVEST && this->puppetAction[i][j].step > 30){continue;}
					// パペット中心の画面座標上位置を計算
					struct engineMathMatrix44 tempmat;
					engineMathMat4Copy(&tempmat, &this->worldMat);
					engineMathMat4Translate(&tempmat, 0.5 + i, 3.5, 0.5 + j);
					double x0 = (1 + tempmat.m30 / tempmat.m33) * 0.5 * global.window.w;
					double y0 = (1 - tempmat.m31 / tempmat.m33) * 0.5 * global.window.h;
					double z0 = tempmat.m32 / tempmat.m33;
					// パペット半径を求める計算
					engineMathMat4Translate(&tempmat, 0, 0.6, 0);
					double r = y0 - (1 - tempmat.m31 / tempmat.m33) * 0.5 * global.window.h;
					double rr = r * r;
					// タッチ位置と比較
					double x = t->window.x - x0;
					double y = t->window.y - y0;
					double xxyy = x * x + y * y;
					if(xxyy < rr && touchDepth > z0){
						touchX = i;
						touchY = j;
						touchDepth = z0;
						if(type != PAGEATELIERMAINPUPPETACTIONTYPE_NONE){touchX = -1;}
					}
					// タッチ開始判定はもう少しゆるく
					within = within || (xxyy < 2 * 2 * rr);
				}
			}

			if(touchX >= 0){
				// 収穫開始処理
				if(this->soundSeCount.harvest1 <= 0){this->soundSeCount.harvest1 = 12; soundSePlayHarvest1();}
				this->puppetAction[touchX][touchY].type = PAGEATELIERMAINPUPPETACTIONTYPE_HARVEST;
				this->puppetAction[touchX][touchY].step = 0;
			}

			if(!t->active && (within || this->canvas.size < PAGEATELIERMAIN_SLIDELIMIT)){
				// タッチ開始時
				engineCtrlTouchOwn();
			}
		}else{
			// タッチ終了時
			engineCtrlTouchFree();
		}
	}
}


// 画面移動の計算
static void calcSlide(struct pageCartridgeAtelierMain *this){
	static int prevsx;
	// タッチ状態の確認
	struct engineCtrlTouch *t = engineCtrlTouchGet(ENGINECTRLTOUCHID_SCREEN);
	if(t != NULL){
		if(t->dn){
			if(!t->active){
				// タッチ開始時
				prevsx = t->screen.x;
				engineCtrlTouchOwn();
			}

			// 画面移動処理
			if(t->mv && this->canvas.size >= PAGEATELIERMAIN_SLIDELIMIT){
				this->slideNext += 0.04 * (prevsx - t->screen.x);
				double min = 2.0;
				double max = this->canvas.size - 2.0;
				if(this->slideNext < min){this->slideNext = min;}
				if(this->slideNext > max){this->slideNext = max;}
			}
			prevsx = t->screen.x;
		}else{
			// タッチ終了時
			engineCtrlTouchFree();
			this->slideArrowSlideNext = this->slideNext;
		}
	}
}

// 減衰計算
static void calcDamper(struct pageCartridgeAtelierMain *this){
	// 画面移動の減衰計算
	double ds = this->slide - this->slideNext;
	if(engineMathAbs(ds) > 0.01){this->slide -= ds * 0.2;}
	// スライドできることを示す矢印位置の減衰計算
	ds = this->slideArrowSlide - this->slideArrowSlideNext;
	if(engineMathAbs(ds) > 0.01){this->slideArrowSlide -= ds * 0.1;}
}

// 時間経過計算
static void calcTimeStep(struct pageCartridgeAtelierMain *this){
	long long int time = platformPluginUtilTimeGet();

	// 時間経過によりマイスターが仕事をする
	int timeDiff = (int)(time - storageAtelierGet()->time);
	if(timeDiff > 0){
		this->workspace.loading = false;
		// マイスターデータのロード確認と獲得
		struct dataMeister *dataMeister[STORAGEATELIER0001_WORKSPACEXMAX];
		for(int i = 0; i < this->workspace.size; i++){
			int meisterKind = storageAtelierGet()->workspace[i].meisterKind;
			if(meisterKind >= 0){
				dataMeister[i] = dataMeisterGet(meisterKind, NULL);
				if(dataMeister[i] == NULL){this->workspace.loading = true;}
			}else{
				dataMeister[i] = NULL;
			}
		}
		if(!this->workspace.loading){
			// 空いているキャンバス数の確認
			int canvasFree = 0;
			for(int i = 0; i < this->canvas.size; i++){
				for(int j = 0; j < STORAGEATELIER0001_CANVASYMAX; j++){
					if(storageAtelierGet()->canvas[i][j].puppetKind < 0){canvasFree++;}
				}
			}
			// ワークスペースの空確認
			bool isCreate = false;
			bool isMeisterEmpty = true;
			for(int i = 0; i < this->workspace.size; i++){if(storageAtelierGet()->workspace[i].meisterKind >= 0){isMeisterEmpty = false; break;}}
			if(isMeisterEmpty){timeDiff = 0;}
			// タイムステップ毎に全マイスターにチャージ
			int canvasFrequencyTable[STORAGEATELIER0001_CANVASXMAX * STORAGEATELIER0001_CANVASYMAX] = {-1};
			int puppetFrequencyTable[STORAGEATELIER0001_WORKSPACEXMAX][DATAMEISTER_PRODUCTMAX];
			for(int i = 0; i < timeDiff && canvasFree > 0; i++){
				for(int j = 0; j < this->workspace.size && canvasFree > 0; j++){
					if(dataMeister[j] == NULL){continue;}
					storageAtelierGet()->workspace[j].charge++;
					if(storageAtelierGet()->workspace[j].charge <= dataMeister[j]->charge){continue;}
					// チャージが十分溜まったのでパペット作成

					int size = this->canvas.size * STORAGEATELIER0001_CANVASYMAX;
					if(canvasFrequencyTable[0] < 0){
						// キャンバス位置確率表作成
						for(int k = 0; k < size; k++){
							int x = k % this->canvas.size;
							int y = k / this->canvas.size;
							canvasFrequencyTable[k] = (storageAtelierGet()->canvas[x][y].puppetKind < 0) ? 1 : 0;
						}

						// パペット抽選確率表作成
						for(int k = 0; k < this->workspace.size; k++){
							int meisterKind = storageAtelierGet()->workspace[k].meisterKind;
							if(meisterKind < 0){continue;}
							int meisterLevel = storageMeisterGet()->list[meisterKind].level;
							for(int l = 0; l < DATAMEISTER_PRODUCTMAX; l++){
								struct dataMeisterProduct *product = &dataMeister[k]->product[l];
								puppetFrequencyTable[k][l] = (meisterLevel >= product->openLevel && product->puppetKind >= 0) ? product->frequency : 0;
							}
						}
					}

					// キャンバス位置抽選
					int canvasIndex = -1;
					int canvasTotalFrequency = 0;
					for(int k = 0; k < size; k++){canvasTotalFrequency += canvasFrequencyTable[k];}
					int canvasRandomValue = engineMathRandomGet() % canvasTotalFrequency;
					for(int k = 0; k < size; k++){
						if(canvasRandomValue < canvasFrequencyTable[k]){
							canvasIndex = k;
							break;
						}else{canvasRandomValue -= canvasFrequencyTable[k];}
					}
					if(canvasIndex < 0){continue;}

					// パペット抽選
					int puppetKind = -1;
					int puppetTotalFrequency = 0;
					for(int k = 0; k < DATAMEISTER_PRODUCTMAX; k++){puppetTotalFrequency += puppetFrequencyTable[j][k];}
					int puppetRandomValue = engineMathRandomGet() % puppetTotalFrequency;
					for(int k = 0; k < DATAMEISTER_PRODUCTMAX; k++){
						if(puppetRandomValue < puppetFrequencyTable[j][k]){
							puppetKind = dataMeister[j]->product[k].puppetKind;
							break;
						}else{puppetRandomValue -= puppetFrequencyTable[j][k];}
					}
					if(puppetKind < 0){continue;}

					// キャンバスにパペット作成
					int x = canvasIndex % this->canvas.size;
					int y = canvasIndex / this->canvas.size;
					storageAtelierGet()->workspace[j].charge = 0;
					storageAtelierGet()->canvas[x][y].puppetKind = puppetKind;
					storageAtelierGet()->canvas[x][y].level = storageMeisterGet()->list[storageAtelierGet()->workspace[j].meisterKind].level;
					this->puppetAction[x][y].type = PAGEATELIERMAINPUPPETACTIONTYPE_CREATE;
					this->puppetAction[x][y].step = 0;
					canvasFrequencyTable[canvasIndex] = 0;
					canvasFree--;
					isCreate = true;
				}
			}
			if(isCreate){soundSePlayHarvest0();}
			storageAtelierGet()->time = time;
			storageAtelierUpdate();
		}
	}
}

// パペット収穫完了処理
static void getPuppet(struct pageCartridgeAtelierMain *this, int x, int y){
	if(this->puppetAction[x][y].type != PAGEATELIERMAINPUPPETACTIONTYPE_HARVEST){return;}
	this->puppetAction[x][y].type = PAGEATELIERMAINPUPPETACTIONTYPE_NONE;
	int puppetKind = storageAtelierGet()->canvas[x][y].puppetKind;
	int num = storagePuppetGet()->list[puppetKind].num + 1;
	if(num > 99){num = 99;}
	storagePuppetGet()->list[puppetKind].num = num;
	storagePuppetGet()->list[puppetKind].level = engineMathCeil((double)(storagePuppetGet()->list[puppetKind].level * (num - 1) + storageAtelierGet()->canvas[x][y].level) / num);
	storagePuppetUpdate();
	storageAtelierGet()->canvas[x][y].puppetKind = -1;
	storageAtelierUpdate();
}

// 計算
static int calc(struct pageCartridgeAtelierMain *this){
	// メモリートレースボタン計算
	partsButtonMemoryCalc();
	
	calcDamper(this);
	calcTimeStep(this);

	if(this->canvas.step > 0){
		// キャンバス拡張アニメーション
		if(this->canvas.step++ > 60){
			this->canvas.step = 0;
			this->canvas.size++;
			if(this->mapCanvas != NULL){hknwMapDispose(this->mapCanvas); this->mapCanvas = NULL;}
			if(this->canvas.size == PAGEATELIERMAIN_SLIDELIMIT){this->slideArrowSlide = 0.0;}
			if(this->canvas.size < storageAtelierGet()->canvasSize){
				this->canvas.step = 1;
				int nextCanvasSize = this->canvas.size + 1;
				this->slideArrowSlideNext = this->slideNext = (nextCanvasSize >= PAGEATELIERMAIN_SLIDELIMIT) ? nextCanvasSize - 2.0 : nextCanvasSize * 0.5;
			}
		}
	}

	if(this->workspace.step > 0){
		// ワークスペース拡張アニメーション
		if(this->workspace.step++ > 60){
			this->workspace.step = 0;
			if(this->workspace.size < storageAtelierGet()->workspaceSize){
				this->workspace.size++;
				this->workspace.step = 1;
			}
		}
	}

	// パペットアクション
	for(int i = 0; i < this->canvas.size; i++){
		for(int j = 0; j < STORAGEATELIER0001_CANVASYMAX; j++){
			int type = this->puppetAction[i][j].type;
			if(type == PAGEATELIERMAINPUPPETACTIONTYPE_CREATE){
				// パペット完成処理
				if(++this->puppetAction[i][j].step == 70){
					type = this->puppetAction[i][j].type = PAGEATELIERMAINPUPPETACTIONTYPE_NONE;
				}
			}else if(type == PAGEATELIERMAINPUPPETACTIONTYPE_HARVEST){
				// パペット収穫処理
				if(++this->puppetAction[i][j].step == 120){
					if(this->soundSeCount.harvest2 <= 0){this->soundSeCount.harvest2 = 12; soundSePlayHarvest2();}
					getPuppet(this, i, j);
					this->puppetCageStep = 0;
				}
			}
		}
	}
	// パペット容器アクション
	this->puppetCageStep++;
	if(this->puppetCageStep == 50){this->puppetCageStep = -200;}

	// 処理落ち防止の効果音カウント
	if(this->soundSeCount.harvest1 > 0){this->soundSeCount.harvest1--;}
	if(this->soundSeCount.harvest2 > 0){this->soundSeCount.harvest2--;}

	// チュートリアルステップカウント
	if(!storageStatusGet()->tutorial.atelier03 && storageStatusGet()->tutorial.atelier01 && this->popupMeisterList == NULL && storageAtelierGet()->workspace[0].meisterKind < 0){this->stepTutorial11++;}else{this->stepTutorial11 = 0;}
	if(!storageStatusGet()->tutorial.atelier05 && storageStatusGet()->tutorial.atelier03){this->stepTutorial12++;}else{this->stepTutorial12 = 0;}
	if(!storageStatusGet()->tutorial.top02 && storageStatusGet()->tutorial.atelier05){this->stepTutorial13++;}else{this->stepTutorial13 = 0;}

	if(this->popupMeisterList != NULL){
		// マイスター選択ポップアップ計算
		if(!this->popupMeisterList->calc(this->popupMeisterList)){return 0;}
		// ポップアップ完了時処理
		int meisterKind0 = -1;
		int meisterKind1 = this->popupMeisterList->result(this->popupMeisterList);
		int charge0 = 0;
		int charge1 = 0;
		if(meisterKind1 >= -1){
			// 入れ替え調査
			for(int i = 0; i < this->workspace.size; i++){
				if(this->btnMeister[i]->select){
					meisterKind0 = storageAtelierGet()->workspace[i].meisterKind;
					if(meisterKind0 >= 0){charge0 = storageAtelierGet()->workspace[i].charge;}
				}else if(meisterKind1 >= 0 && meisterKind1 == storageAtelierGet()->workspace[i].meisterKind){
					charge1 = storageAtelierGet()->workspace[i].charge;
				}
			}

			if(meisterKind0 != meisterKind1){
				// データ更新
				for(int i = 0; i < this->workspace.size; i++){
					if(this->btnMeister[i]->select){
						storageAtelierGet()->workspace[i].meisterKind = meisterKind1;
						storageAtelierGet()->workspace[i].charge = charge1;
						storageAtelierUpdate();
						this->btnMeister[i]->meisterKind = meisterKind1;
						if(meisterKind1 >= 0){this->btnMeister[i]->meisterLevel = storageMeisterGet()->list[meisterKind1].level;}
					}else if(meisterKind1 >= 0 && meisterKind1 == storageAtelierGet()->workspace[i].meisterKind){
						storageAtelierGet()->workspace[i].meisterKind = meisterKind0;
						storageAtelierGet()->workspace[i].charge = charge0;
						storageAtelierUpdate();
						this->btnMeister[i]->meisterKind = meisterKind0;
						if(meisterKind0 >= 0){this->btnMeister[i]->meisterLevel = storageMeisterGet()->list[meisterKind0].level;}
					}     
				}
			}
		}
		for(int i = 0; i < this->workspace.size; i++){this->btnMeister[i]->select = 0;}
		// ポップアップ除去
		this->popupMeisterList->dispose(this->popupMeisterList);
		this->popupMeisterList = NULL;
	}

	if(this->popupPuppetList != NULL){
		// パペットリストポップアップ計算
		if(!this->popupPuppetList->calc(this->popupPuppetList)){return 0;}
		// ポップアップ完了時処理
		if(this->popupPuppetList->result(this->popupPuppetList)){}
		this->popupPuppetList->dispose(this->popupPuppetList);
		this->popupPuppetList = NULL;
	}

	if(this->popupGacha != NULL){
		// ガチャポップアップ計算
		if(!this->popupGacha->calc(this->popupGacha)){return 0;}
		// ポップアップ完了時処理
		int gachaKind = this->popupGacha->result(this->popupGacha);
		struct dataGacha *dataGacha = dataGachaGet(gachaKind, NULL);
		this->popupGacha->dispose(this->popupGacha);
		this->popupGacha = NULL;
		if(dataGacha != NULL && (storageStatusGet()->money >= dataGacha->money)){
			storageStatusGet()->money -= dataGacha->money;
			storageStatusGet()->gachaCount[gachaKind]++;
			storageStatusUpdate();
			// マイスター抽選
			int meisterKind = -1;
			int totalFrequency = 0;
			bool isFirst = (storageStatusGet()->gachaCount[gachaKind] <= 1);
			for(int i = 0; i < DATAGACHA_PRODUCTMAX; i++){
				struct dataGachaProduct *product = &dataGacha->product[i];
				if(product->meisterKind >= 0 && (!isFirst || product->isFirst)){
					totalFrequency += product->frequency;
				}
			}
			int randomValue = engineMathRandomGet() % totalFrequency;
			for(int i = 0; i < DATAGACHA_PRODUCTMAX; i++){
				struct dataGachaProduct *product = &dataGacha->product[i];
				if(product->meisterKind >= 0 && (!isFirst || product->isFirst)){
					if(randomValue < product->frequency){
						meisterKind = product->meisterKind;
						break;
					}else{randomValue -= product->frequency;}
				}
			}
			// マイスター獲得でデータ保存とポップアップ表示
			if(meisterKind >= 0){
				int typeNew = 0;
				int level = storageMeisterGet()->list[meisterKind].level + 1;
				if(level <= 1){typeNew = 1;}else if(level > 99){typeNew = -1; level = 99;}
				storageMeisterGet()->list[meisterKind].level = level;
				storageMeisterUpdate();
				this->popupGachaResult = popupAtelierGachaResult(this->stat, meisterKind, typeNew);
				return 0;
			}
		}
	}

	if(this->popupGachaResult != NULL){
		// ガチャ結果ポップアップ計算
		if(!this->popupGachaResult->calc(this->popupGachaResult)){return 0;}
		// ポップアップ完了時処理
		if(this->popupGachaResult->result(this->popupGachaResult)){}
		this->popupGachaResult->dispose(this->popupGachaResult);
		this->popupGachaResult = NULL;
		// 上昇したマイスターレベルの反映
		for(int i = 0; i < this->workspace.size; i++){
			int meisterKind = storageAtelierGet()->workspace[i].meisterKind;
			if(meisterKind >= 0){this->btnMeister[i]->meisterLevel = storageMeisterGet()->list[meisterKind].level;}    
		}
	}

	if(this->popupItemSelect != NULL){
		// アイテム選択ポップアップ計算
		if(!this->popupItemSelect->calc(this->popupItemSelect)){return 0;}
		// アイテム選択ポップアップ完了時処理
		int itemKind = this->popupItemSelect->result(this->popupItemSelect);
		this->popupItemSelect->dispose(this->popupItemSelect);
		this->popupItemSelect = NULL;
		if(itemKind >= 0){this->popupItemUse1 = partsPopupItemAskUse(itemKind); return 0;}
	}

	if(this->popupItemUse1 != NULL){
		// アイテム使用確認ポップアップ計算
		if(!this->popupItemUse1->calc(this->popupItemUse1)){return 0;}
		// アイテム使用確認ポップアップ完了時処理
		int itemKind = this->popupItemUse1->result(this->popupItemUse1);
		if(itemKind == DATAITEMKIND_CANVASFULL && storageStatusGet()->itemNum[itemKind] > 0){
			// キャンバス充填アイテム使用
			storageStatusGet()->itemNum[itemKind]--;
			storageStatusUpdate();
			long long int time = storageAtelierGet()->time;
			int charge[STORAGEATELIER0001_WORKSPACEXMAX];
			storageAtelierGet()->time = 0;
			for(int i = 0; i < this->workspace.size; i++){charge[i] = storageAtelierGet()->workspace[i].charge;}
			calcTimeStep(this);
			storageAtelierGet()->time = time;
			for(int i = 0; i < this->workspace.size; i++){storageAtelierGet()->workspace[i].charge = charge[i];}
			storageAtelierUpdate();
		}
		if(itemKind == DATAITEMKIND_CANVASOPEN && storageStatusGet()->itemNum[itemKind] > 0 && storageAtelierGet()->canvasSize < STORAGEATELIER0001_CANVASXMAX){
			// キャンバス拡張アイテム効果適用
			storageStatusGet()->itemNum[itemKind]--;
			storageStatusUpdate();
			storageAtelierGet()->canvasSize++;
			storageAtelierUpdate();
			if(this->canvas.step == 0 && this->canvas.size < storageAtelierGet()->canvasSize){
				this->canvas.step = 1;
				int nextCanvasSize = this->canvas.size + 1;
				this->slideArrowSlideNext = this->slideNext = (nextCanvasSize >= PAGEATELIERMAIN_SLIDELIMIT) ? nextCanvasSize - 2.0 : nextCanvasSize * 0.5;
			}
		}
		if(itemKind == DATAITEMKIND_WORKSPACEOPEN && storageStatusGet()->itemNum[itemKind] > 0 && storageAtelierGet()->workspaceSize < STORAGEATELIER0001_WORKSPACEXMAX){
			// ワークスペース拡張アイテム効果適用
			storageStatusGet()->itemNum[itemKind]--;
			storageStatusUpdate();
			storageAtelierGet()->workspaceSize++;
			storageAtelierUpdate();
			if(this->workspace.step == 0 && this->workspace.size < storageAtelierGet()->workspaceSize){
				this->workspace.size++;
				this->workspace.step = 1;
			}
		}
		this->popupItemUse1->dispose(this->popupItemUse1);
		this->popupItemUse1 = NULL;
		if(itemKind >= 0){this->popupItemUse2 = partsPopupItemInfoUse(itemKind); return 0;}
	}

	if(this->popupItemUse2 != NULL){
		// アイテム使用完了ポップアップ計算
		if(!this->popupItemUse2->calc(this->popupItemUse2)){return 0;}
		// アイテム使用完了ポップアップ完了時処理
		int itemKind = this->popupItemUse2->result(this->popupItemUse2);
		if(itemKind == DATAITEMKIND_CANVASFULL){}
		if(itemKind == DATAITEMKIND_CANVASOPEN){}
		if(itemKind == DATAITEMKIND_WORKSPACEOPEN){}
		this->popupItemUse2->dispose(this->popupItemUse2);
		this->popupItemUse2 = NULL;
	}

	if(this->popupTutorial != NULL){
		// チュートリアルポップアップ計算
		if(!this->popupTutorial->calc(this->popupTutorial)){return 0;}
		// チュートリアルポップアップ完了時処理
		if(this->popupTutorial->result(this->popupTutorial)){}
		this->popupTutorial->dispose(this->popupTutorial);
		this->popupTutorial = NULL;
	}else if(!storageStatusGet()->tutorial.atelier01){
		// チュートリアル計算
		if(this->stepTutorial01 == 60){soundSePlayOK(); this->popupTutorial = partsPopupTutorialAtelier01();}
		if(this->stepTutorial01 == 60){storageStatusGet()->tutorial.atelier01 = true; storageStatusUpdate();}
		this->stepTutorial01++;
		return 0;
	}else if(!storageStatusGet()->tutorial.atelier02){
		// チュートリアル計算
		if(storageAtelierGet()->workspace[0].meisterKind >= 0){
			if(this->stepTutorial02 == 60){soundSePlayOK(); this->popupTutorial = partsPopupTutorialAtelier02();}
			if(this->stepTutorial02 == 60){storageStatusGet()->tutorial.atelier02 = true; storageStatusUpdate();}
			this->stepTutorial02++;
			return 0;
		}else{this->stepTutorial02 = 0;}
	}else if(!storageStatusGet()->tutorial.atelier03){
		// チュートリアル計算
		bool createdFlag = (this->stepTutorial03 > 0);
		if(!createdFlag){for(int i = 0; i < this->canvas.size; i++){for(int j = 0; j < STORAGEATELIER0001_CANVASYMAX; j++){if(storageAtelierGet()->canvas[i][j].puppetKind >= 0){createdFlag = true; break;}} if(createdFlag){break;}}}
		if(createdFlag){
			if(this->stepTutorial03 == 80){soundSePlayOK(); this->popupTutorial = partsPopupTutorialAtelier03();}
			if(this->stepTutorial03 == 80){storageStatusGet()->tutorial.atelier03 = true; storageStatusUpdate();}
			this->stepTutorial03++;
			return 0;
		}
	}else if(!storageStatusGet()->tutorial.atelier04){
		// チュートリアル計算
		if(storagePuppetGet()->list[0].num >= 5){
			if(this->stepTutorial04 == 60){soundSePlayOK(); this->popupTutorial = partsPopupTutorialAtelier05B();}
			if(this->stepTutorial04 == 60){storageStatusGet()->tutorial.atelier04 = storageStatusGet()->tutorial.atelier05 = true; storageStatusUpdate();}
			this->stepTutorial04++;
			return 0;
		}else if(storagePuppetGet()->list[0].num >= 1){
			if(this->stepTutorial04 == 60){soundSePlayOK(); this->popupTutorial = partsPopupTutorialAtelier04();}
			if(this->stepTutorial04 == 60){storageStatusGet()->tutorial.atelier04 = true; storageStatusUpdate();}
			this->stepTutorial04++;
			return 0;
		}
	}else if(!storageStatusGet()->tutorial.atelier05){
		// チュートリアル計算
		if(storagePuppetGet()->list[0].num >= 5){
			if(this->stepTutorial05 == 60){soundSePlayOK(); this->popupTutorial = partsPopupTutorialAtelier05A();}
			if(this->stepTutorial05 == 60){storageStatusGet()->tutorial.atelier05 = true; storageStatusUpdate();}
			this->stepTutorial05++;
			return 0;
		}
	}

	// タッチ処理
	for(int i = 0; i < this->workspace.size; i++){buttonAtelierMeisterCalc(this->btnMeister[i]);}
	partsButtonBoxCalc(&this->btnPuppetCage);
	partsButtonBoxCalc(&this->btnGacha);
	partsButtonBoxCalc(&this->btnItem);
	partsButtonBoxCalc(&this->btnBack);
	calcHarvest(this);
	calcSlide(this);

	// マイスターボタン押下確認
	for(int i = 0; i < this->workspace.size; i++){
		this->btnMeister[i]->super.inactive = this->workspace.loading;
		if(this->btnMeister[i]->super.trigger){
			this->btnMeister[i]->super.trigger = false;
			soundSePlayOK();
			// ポップアップ展開
			this->btnMeister[i]->select = 1;
			this->popupMeisterList = popupAtelierMeisterList(this->stat, storageAtelierGet()->workspace[i].meisterKind);
			return 0;
		}
	}

	// パペット容器ボタン押下確認
	if(this->btnPuppetCage.trigger){
		this->btnPuppetCage.trigger = false;
		soundSePlayOK();
		// ポップアップ展開
		this->popupPuppetList = popupAtelierPuppetList(this->stat);
		return 0;
	}

	// ガチャボタン押下確認
	if(this->btnGacha.super.trigger){
		this->btnGacha.super.trigger = false;
		soundSePlayOK();
		// ポップアップ展開
		this->popupGacha = popupAtelierGacha(this->stat);
		return 0;
	}

	// アイテムボタン押下確認
	if(this->btnItem.super.trigger){
		this->btnItem.super.trigger = false;
		soundSePlayOK();
		// ポップアップ展開
		bool isCanvasFull = !this->workspace.loading;
		this->popupItemSelect = partsPopupItemSelect((isCanvasFull << DATAITEMKIND_CANVASFULL) | (1 << DATAITEMKIND_CANVASOPEN) | (1 << DATAITEMKIND_WORKSPACEOPEN));
		return 0;
	}

	// もどるボタン押下確認
	if(this->btnBack.super.trigger || global.key.bk.triggerInactive){
		this->btnBack.super.trigger = false;
		global.key.bk.triggerInactive = false;
		soundSePlayNG();
		// ページ遷移
		pageCartridgePush(pageTopInit());
		pageAtelierStatusDispose(this->stat);
		return 1;
	}

	return 0;
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct pageCartridgeAtelierMain *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// マイスターボタン位置
		for(int i = 0; i < STORAGEATELIER0001_WORKSPACEXMAX; i++){buttonAtelierMeisterPosition(this->btnMeister[i], (70 + 3 * 2) * i + 3, 5);}
		// パペット容器ボタン位置
		partsButtonPosition(&this->btnPuppetCage, 5, global.screen.h - 60 - 5, 60, 60);

		// バッファ作成開始
		engineGraphicBufferBegin();

		// スライドできることを示す矢印作成
		this->slideArrow.faceIndex = engineGraphicBufferFaceIndexGet();
		this->slideArrow.faceNum = 2;
		engineGraphicBufferPushTetraFace(engineGraphicBufferVretIndexGet());
		engineGraphicBufferPushVert(-2.5, 2.5, 4.5);
		engineGraphicBufferPushVert(-2.5, 0.5, 4.5);
		engineGraphicBufferPushVert( 0.5, 0.5, 4.5);
		engineGraphicBufferPushVert( 0.5, 2.5, 4.5);
		engineGraphicBufferPushTetraTexc(TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, TEXPOS_SYSTEM_IMAGESLIDE_X, TEXPOS_SYSTEM_IMAGESLIDE_Y, TEXPOS_SYSTEM_IMAGESLIDE_W, TEXPOS_SYSTEM_IMAGESLIDE_H);

		// パペット容器作成
		e3dImageCreateArray(&this->puppetCage, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, 5, global.screen.h - 60 - 5, 60, 60, TEXPOS_SYSTEM_IMAGEPUPPETCAGE_X, TEXPOS_SYSTEM_IMAGEPUPPETCAGE_Y, TEXPOS_SYSTEM_IMAGEPUPPETCAGE_W, TEXPOS_SYSTEM_IMAGEPUPPETCAGE_H);

		// チュートリアル矢印作成
		e3dImageCreateArray(&this->tutorialArrow1, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, (global.screen.w - 70) * 0.5 + 20 - 40, 5 + (100 - 40) * 0.5, 40, 40, TEXPOS_SYSTEM_IMAGETUTORIALARROW_X, TEXPOS_SYSTEM_IMAGETUTORIALARROW_Y, TEXPOS_SYSTEM_IMAGETUTORIALARROW_W, TEXPOS_SYSTEM_IMAGETUTORIALARROW_H);
		e3dImageCreateArray(&this->tutorialArrow2, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, -20, -20, 40, 40, TEXPOS_SYSTEM_IMAGETUTORIALPOINT_X, TEXPOS_SYSTEM_IMAGETUTORIALPOINT_Y, TEXPOS_SYSTEM_IMAGETUTORIALPOINT_W, TEXPOS_SYSTEM_IMAGETUTORIALPOINT_H);
		e3dImageCreateArray(&this->tutorialArrow3, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, global.screen.w - 100 - 5 + 20 - 40, global.screen.h - (30 + 5) * 1 + (30 - 40) * 0.5, 40, 40, TEXPOS_SYSTEM_IMAGETUTORIALARROW_X, TEXPOS_SYSTEM_IMAGETUTORIALARROW_Y, TEXPOS_SYSTEM_IMAGETUTORIALARROW_W, TEXPOS_SYSTEM_IMAGETUTORIALARROW_H);

		// ガチャボタン作成
		partsButtonBoxCreateArray(&this->btnGacha, global.screen.w - 100 - 5, global.screen.h - (30 + 5) * 3, 100, 30, "ガチャ", 1.0);
		// アイテムボタン作成
		partsButtonBoxCreateArray(&this->btnItem, global.screen.w - 100 - 5, global.screen.h - (30 + 5) * 2, 100, 30, "アイテム", 1.0);
		// 戻るボタン作成
		partsButtonBoxCreateArray(&this->btnBack, global.screen.w - 100 - 5, global.screen.h - (30 + 5) * 1, 100, 30, "もどる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// マップ作成
static void createMap(struct pageCartridgeAtelierMain *this){
	if(this->mapCanvas == NULL){
		// マップチップ作成
		int mapchip[3][4][16];
		for(int i = 0; i < STORAGEATELIER0001_CANVASXMAX; i++){
			for(int j = 0; j < STORAGEATELIER0001_CANVASYMAX; j++){
				int flag = (i < this->canvas.size);
				mapchip[0][j][i] = mapchip[1][j][i] = flag ? 2 : 0;
				mapchip[2][j][i] = flag ? 4 : 0;
			}
		}
		this->mapCanvas = hknwMapInit(16, 4, 3, (int*)mapchip);
	}

	if(this->mapAdd == NULL){
		// マップチップ作成
		int mapchip[3][4][1] = {{{2}, {2}, {2}, {2}}, {{2}, {2}, {2}, {2}}, {{4}, {4}, {4}, {4}}};
		this->mapAdd = hknwMapInit(1, 4, 3, (int*)mapchip);
	}
}

// 描画
static void draw(struct pageCartridgeAtelierMain *this){
	// 描画準備
	createBuffer(this);
	createMap(this);

	struct engineMathMatrix44 tempmat;
	// 射影行列作成
	double xFrustum = 0.050 * global.screen.aspect.x * ((global.screen.h < global.screen.w) ? ((double)global.screen.w / global.screen.h) : 1);
	double yFrustum = 0.050 * global.screen.aspect.y * ((global.screen.h > global.screen.w) ? ((double)global.screen.h / global.screen.w) : 1);
	engineMathMat4Frustum(&e3dMatFrustum, -xFrustum, xFrustum, -yFrustum, yFrustum, 0.1, 100);
	engineMathMat4Ortho(&e3dMatOrtho, -global.screen.offset.x, global.screen.w + global.screen.offset.x, global.screen.h + global.screen.offset.y, -global.screen.offset.y, -1, 1);
	// ワールド行列作成
	engineMathMat4Copy(&this->worldMat, &e3dMatFrustum);
	engineMathMat4Translate(&this->worldMat, 0, 0, -this->dist);
	engineMathMat4RotateX(&this->worldMat, this->roth);
	engineMathMat4RotateY(&this->worldMat, this->rotv);
	engineMathMat4Translate(&this->worldMat, -this->slide, -3.0, -2.0);

	engineGraphicEngineClearAll();

	// 背景描画
	dataBackgroundDraw(0);

	// ハコニワ描画
	engineGraphicEngineBindTexture(e3dIdSystemImageNearest);
	hknwMapDraw(this->mapCanvas, &this->worldMat);
	if(this->canvas.step > 0){
		double num = (double)this->canvas.step / 60;
		engineMathMat4Copy(&tempmat, &this->worldMat);
		engineMathMat4Translate(&tempmat, this->canvas.size, 10 * (1 - num) * (1 - num), 0.0);
		hknwMapDraw(this->mapAdd, &tempmat);
	}

	// キャラクタ描画
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_NORMAL);
	hknwCharaListBind();
	for(int i = 0; i < this->canvas.size; i++){
		for(int j = 0; j < STORAGEATELIER0001_CANVASYMAX; j++){
			int puppetKind = storageAtelierGet()->canvas[i][j].puppetKind;
			if(puppetKind < 0){continue;}
			int e3dIdImage;
			struct dataPuppet *dataPuppet = dataPuppetGet(puppetKind, &e3dIdImage);
			if(dataPuppet == NULL){continue;}
			double x0 = i + 0.5;
			double y0 = j + 0.5;
			double z0 = 3.0;
			double r0 = 3.1415 / 180 * 90;
			int type = this->puppetAction[i][j].type;
			int step = this->puppetAction[i][j].step;

			// クリッピング
			if(type != PAGEATELIERMAINPUPPETACTIONTYPE_HARVEST){
				double x = x0 - this->slide;
				if((x < -5.0 || 9.0 < x) || (j == 0 && x < -4.0) || (j == 1 && 8.0 < x) || (j == 2 && 7.0 < x) || (j == 3 && 6.0 < x)){continue;}
			}

			// パペットアクション
			engineMathMat4Copy(&tempmat, &this->worldMat);
			engineMathMat4Translate(&tempmat, x0, z0, y0);
			if(type == PAGEATELIERMAINPUPPETACTIONTYPE_CREATE){
				// パペット完成処理
				if(step < 30){
					double num = (step - 0) / 30.0;
					engineMathMat4Translate(&tempmat, 0.0, (1 - num * num) * 2.5, 0.0);
				}else if(step < 30 + 20){
					double num = (step - 30) / 20.0;
					double stretch = 0.5 * engineMathSin(3.1415 * 1.0 * num);
					engineMathMat4Scale(&tempmat, 1.0 + stretch, 1.0 - stretch, 1);
				}else if(step < 50 + 20){
					double num = (step - 50) / 20.0;
					engineMathMat4Translate(&tempmat, 0.0, (1 - (num * 2 - 1) * (num * 2 - 1)) * 0.5, 0.0);
				}
			}else if(type == PAGEATELIERMAINPUPPETACTIONTYPE_HARVEST){
				// パペット収穫アクション
				double x1 = 0.0;
				double y1 = 0.0;
				double z1 = 1.5;
				double x2 = this->slide - 3.7 - x0;
				double y2 = 5.0 - y0;
				double z2 = 2.0 - z0;
				if(step < 30){
					double num = (step - 0) / 30.0;
					double stretch = 0.8 * engineMathSin(3.1415 * 1.5 * num);
					engineMathMat4Scale(&tempmat, 1.0 + stretch, 1.0 - stretch, 1);
				}else if(step < 30 + 50){
					double num = (step - 30) / 50.0;
					double stretch = -0.8 * engineMathCos(3.1415 * 0.5 * num);
					engineMathMat4Scale(&tempmat, 1.0 + stretch, 1.0 - stretch, 1);
					engineMathMat4Translate(&tempmat, x1, (1 - (1 - num) * (1 - num)) * z1, y1);
				}else if(step < 80 + 40){
					double num = (step - 80) / 40.0;
					//r0 = atan2(y2 - y1, x2 - x1);
					engineMathMat4Translate(&tempmat, x1 - num * (x1 - x2), z1 - num * num * (z1 - z2), y1 - num * (y1 - y2));
				}else{
					engineMathMat4Translate(&tempmat, x2, z2, y2);
				}
			}

			engineGraphicEngineBindTexture(e3dIdImage);
			struct hknwChara *chara = hknwCharaListGet(dataPuppet->charaKind);
			hknwCharaDrawChara(chara, &tempmat, this->rotv, this->roth, 0.0, 0.0, 0.0, r0, 1.5, HKNWCHARAPOSESID_STAND, 0);
			// パペットの影描画
			double num = 1;
			if(type == PAGEATELIERMAINPUPPETACTIONTYPE_CREATE){num = (step - 0) / 30.0;}
			if(type == PAGEATELIERMAINPUPPETACTIONTYPE_HARVEST){num = 1 - (step - 30) / 50.0;}
			if(num < 0){num = 0;}else if(num > 1){num = 1;}
			hknwCharaDrawShadow(chara, &this->worldMat, x0, y0, z0, 1.5 * num);
		}
	}

	if(this->canvas.size >= PAGEATELIERMAIN_SLIDELIMIT){
		// スライドできることを示す矢印描画
		engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
		engineGraphicEngineBindVertVBO(this->e3dIdVert);
		engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
		engineGraphicEngineBindFaceIBO(this->e3dIdFace);
		engineMathMat4Copy(&tempmat, &this->worldMat);
		engineMathMat4Translate(&tempmat, this->slideArrowSlide, 0.0, 0.0);
		engineGraphicEngineSetMatrix(&tempmat);
		e3dImageDraw(&this->slideArrow);
	}

	// 2D描画準備
	engineGraphicEngineSetStencilMode(ENGINEGRAPHICENGINEMODESTENCIL_NONE);
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);

	// マイスターボタン描画
	for(int i = 0; i < this->workspace.size; i++){
		double alpha = 1.0;
		int x = (global.screen.w - (70 + 3 * 2) * this->workspace.size) * 0.5;
		if(this->workspace.step > 0){
			// ワークスペース拡張アニメーション
			double num = (double)this->workspace.step / 60;
			x += (70 + 3 * 2) * 0.5 * (1 - num) * (1 - num);
			if(i == this->workspace.size - 1){
				x += 50 * (1 - num) * (1 - num);
				alpha = num;
			}
		}
		buttonAtelierMeisterMove(this->btnMeister[i], x, 0);
		buttonAtelierMeisterDraw(this->btnMeister[i], &e3dMatOrtho, storageAtelierGet()->workspace[i].charge, alpha);
	}

	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);
	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);

	// パペット容器描画
	int pcx = this->btnPuppetCage.x + this->btnPuppetCage.w * 0.5;
	int pcy = this->btnPuppetCage.y + this->btnPuppetCage.h * 0.5;
	int pcActive = this->btnPuppetCage.active;
	engineMathMat4Copy(&tempmat, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat, pcx, pcy, 0);
	if(pcActive){engineMathMat4Scale(&tempmat, 0.8, 0.8, 1);}
	if(0 < this->puppetCageStep && this->puppetCageStep < 50){
		double num = (this->puppetCageStep - 0) / 50.0;
		double stretch = 0.3 * engineMathSin(3.1415 * 2.0 * num);
		engineMathMat4Scale(&tempmat, 1.0 + stretch, 1.0 - stretch, 1);
	}
	engineMathMat4Translate(&tempmat, -pcx, -pcy, 0);
	engineGraphicEngineSetMatrix(&tempmat);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
	e3dImageDraw(&this->puppetCage);

	// ボタン描画
	partsButtonBoxDraw(&this->btnGacha, &e3dMatOrtho, 1.0);
	partsButtonBoxDraw(&this->btnItem, &e3dMatOrtho, 1.0);
	partsButtonBoxDraw(&this->btnBack, &e3dMatOrtho, 1.0);

	// チュートリアル矢印描画
	if(this->stepTutorial11 > 0){
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, -engineMathAbs(10 * engineMathSin(3.1415 * this->stepTutorial11 * 0.05)), 0, 0);
		engineGraphicEngineSetMatrix(&tempmat);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
		e3dImageDraw(&this->tutorialArrow1);
	}
	if(this->stepTutorial12 > 0){
		double s = engineMathSin(3.1415 * this->stepTutorial12 * 0.02);
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, global.screen.w * 0.5 + 60 + 30 * s, global.screen.h * 0.5 + 60 - 20 * s, 0);
		engineGraphicEngineSetMatrix(&tempmat);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
		e3dImageDraw(&this->tutorialArrow2);
	}
	if(this->stepTutorial13 > 0){
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, -engineMathAbs(10 * engineMathSin(3.1415 * this->stepTutorial13 * 0.05)), 0, 0);
		engineGraphicEngineSetMatrix(&tempmat);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
		e3dImageDraw(&this->tutorialArrow3);
	}

	// ポップアップ描画
	if(this->popupMeisterList != NULL){this->popupMeisterList->draw(this->popupMeisterList);}
	if(this->popupPuppetList != NULL){this->popupPuppetList->draw(this->popupPuppetList);}
	if(this->popupGacha != NULL){this->popupGacha->draw(this->popupGacha);}
	if(this->popupGachaResult != NULL){this->popupGachaResult->draw(this->popupGachaResult);}
	if(this->popupItemSelect != NULL){this->popupItemSelect->draw(this->popupItemSelect);}
	if(this->popupItemUse1 != NULL){this->popupItemUse1->draw(this->popupItemUse1);}
	if(this->popupItemUse2 != NULL){this->popupItemUse2->draw(this->popupItemUse2);}
	if(this->popupTutorial != NULL){this->popupTutorial->draw(this->popupTutorial);}

	// メモリートレースボタン描画
	partsButtonMemoryDraw();
	
	engineGraphicEngineFlush();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageCartridgeAtelierMain *this){
	// 収穫エフェクトの完了処理
	for(int i = 0; i < this->canvas.size; i++){
		for(int j = 0; j < STORAGEATELIER0001_CANVASYMAX; j++){
			getPuppet(this, i, j);
		}
	}
}

// 破棄
static void dispose(struct pageCartridgeAtelierMain *this){
	if(this->popupMeisterList != NULL){this->popupMeisterList->dispose(this->popupMeisterList);}
	if(this->popupPuppetList != NULL){this->popupPuppetList->dispose(this->popupPuppetList);}
	if(this->popupGacha != NULL){this->popupGacha->dispose(this->popupGacha);}
	if(this->popupGachaResult != NULL){this->popupGachaResult->dispose(this->popupGachaResult);}
	if(this->popupItemSelect != NULL){this->popupItemSelect->dispose(this->popupItemSelect);}
	if(this->popupItemUse1 != NULL){this->popupItemUse1->dispose(this->popupItemUse1);}
	if(this->popupItemUse2 != NULL){this->popupItemUse2->dispose(this->popupItemUse2);}
	if(this->popupTutorial != NULL){this->popupTutorial->dispose(this->popupTutorial);}
	for(int i = 0; i < STORAGEATELIER0001_WORKSPACEXMAX; i++){buttonAtelierMeisterDispose(this->btnMeister[i]);}
	if(this->mapCanvas != NULL){hknwMapDispose(this->mapCanvas);}
	if(this->mapAdd != NULL){hknwMapDispose(this->mapAdd);}
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページカートリッジ作成
struct pageCartridge *pageAtelierMain(struct pageAtelierStatus *stat){
	struct pageCartridgeAtelierMain *this = (struct pageCartridgeAtelierMain*)engineUtilMemoryCalloc(1, sizeof(struct pageCartridgeAtelierMain));
	this->stat = stat;

	struct pageCartridge *cartridge = (struct pageCartridge*)this;
	cartridge->init = (void(*)(struct pageCartridge*))init;
	cartridge->calc = (int(*)(struct pageCartridge*))calc;
	cartridge->draw = (void(*)(struct pageCartridge*))draw;
	cartridge->pause = (void(*)(struct pageCartridge*))pause;
	cartridge->dispose = (void(*)(struct pageCartridge*))dispose;
	return cartridge;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

