#include "../library.h"
#include "../includeAll.h"
#include "pageAtelier.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#define POPUPATELIERMEISTERLIST_BY 240
#define POPUPATELIERMEISTERLIST_BH 130

// ポップアップカートリッジ構造体
struct popupCartridgeAtelierMeisterList{
	struct popupCartridge super;
	struct pageAtelierStatus *stat;
	int stepTutorial1;
	int stepTutorial2;	

	struct buttonAtelierMeister *btnMeister[STORAGEMEISTER0001_KIND];
	struct partsButtonBox btnSelect;
	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage belt;
	struct e3dImage titleWindow;
	struct e3dImage titleLabel;
	struct e3dImage ProfileImage;
	struct e3dImage ProfileName;
	struct e3dImage ProfileText;
	struct e3dImage tutorialArrow1;
	struct e3dImage tutorialArrow2;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	int bufferMeisterProfileStatus;
	int bufferMeisterProfileScreen;
	int e3dIdMeisterProfileVert;
	int e3dIdMeisterProfileTexc;
	int e3dIdMeisterProfileFace;

	int picked;
	int result;
	// 画面移動
	double slide;
	double slideNext;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct popupCartridgeAtelierMeisterList *this){
	// マイスターボタン作成
	for(int i = 0; i < STORAGEMEISTER0001_KIND; i++){
		this->btnMeister[i] = buttonAtelierMeisterInit(i);
		buttonAtelierMeisterPosition(this->btnMeister[i], i * 75, POPUPATELIERMEISTERLIST_BY + 15);
		this->btnMeister[i]->super.inactive = (storageMeisterGet()->list[i].level <= 0);
		this->btnMeister[i]->select = (i == this->picked) ? 1 : 0;
	}
	// 選択ボタン
	partsButtonBoxInit(&this->btnSelect);
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;

	// 画面移動
	this->slideNext = this->picked >= 0 ? this->picked * 75 : this->stat->slideMeisterList;
	this->slide = this->slideNext + 200;
}

// ----------------------------------------------------------------

// ベルト移動の計算
static void calcSlide(struct popupCartridgeAtelierMeisterList *this){
	static int prevsx;
	// タッチ状態の確認
	struct engineCtrlTouch *t = engineCtrlTouchGet(ENGINECTRLTOUCHID_SCREEN);
	// ベルト内ボタンの確認
	int mv = (t != NULL && !t->mv);
	for(int i = 0; i < STORAGEMEISTER0001_KIND; i++){buttonAtelierMeisterSubCalc(this->btnMeister[i], t, mv);}
	// ベルト移動の確認
	if(t != NULL){
		if(t->dn){
			if(!t->active && POPUPATELIERMEISTERLIST_BY < t->screen.y && t->screen.y < POPUPATELIERMEISTERLIST_BY + POPUPATELIERMEISTERLIST_BH){
				// タッチ開始時
				prevsx = t->screen.x;
				engineCtrlTouchOwn();
			}

			// 画面移動処理
			if(t->mv){
				this->slideNext += 1.0 * (prevsx - t->screen.x);
				double min = 0;
				double max = 75 * (STORAGEMEISTER0001_KIND - 1);
				if(this->slideNext < min){this->slideNext = min;}
				if(this->slideNext > max){this->slideNext = max;}
			}
			prevsx = t->screen.x;
		}else{
			// タッチ終了時
			engineCtrlTouchFree();
		}
	}
}

// 減衰計算
static void calcDamper(struct popupCartridgeAtelierMeisterList *this){
	// ベルト移動の減衰計算
	double ds = this->slide - this->slideNext;
	if(engineMathAbs(ds) > 0.01){this->slide -= ds * 0.2;}
	for(int i = 0; i < STORAGEMEISTER0001_KIND; i++){
		int x = global.screen.w * 0.5 - 35 - this->slide; 
		int y = this->btnMeister[i]->super.movey;
		y = (i == this->picked) ? (y > -10 ? y - 5 : -10) : (y < 10 ? y + 5 : 10);
		buttonAtelierMeisterMove(this->btnMeister[i], x, y);
	}
}

// 前方計算
static void popupCalcBefore(struct popupCartridgeAtelierMeisterList *this){
	calcDamper(this);
}

// 主計算
static int popupCalcMain(struct popupCartridgeAtelierMeisterList *this){
	// タッチ処理
	calcSlide(this);
	partsButtonBoxCalc(&this->btnSelect);
	partsButtonBoxCalc(&this->btnClose);
	partsButtonCalc(&this->btnOuter);

	// チュートリアルステップカウント
	if(!storageStatusGet()->tutorial.atelier03 && storageAtelierGet()->workspace[0].meisterKind < 0){
		if(this->picked >= 0){
			this->stepTutorial1 = 0;
			this->stepTutorial2++;
		}else{
			this->stepTutorial1++;
			this->stepTutorial2 = 0;
		}
	}else{this->stepTutorial1 = this->stepTutorial2 = 0;}

	// マイスターボタン押下確認
	for(int i = 0; i < STORAGEMEISTER0001_KIND; i++){
		if(this->btnMeister[i]->super.trigger){
			this->btnMeister[i]->super.trigger = false;
			soundSePlayOK();
			// 選択物変更
			if(this->picked != i){
				// 隠れていた場合見える位置にスクロール
				int x = this->btnMeister[i]->super.x + this->btnMeister[i]->super.movex;
				int min = 20;
				int max = global.screen.w - this->btnMeister[i]->super.w - 20;
				if(x < min){this->slideNext += x - min;}
				if(x > max){this->slideNext += x - max;}

				this->picked = i;
			}else{this->picked = -1;}
			return 0;
		}
	}

	// 選択ボタン押下確認
	if(this->btnSelect.super.trigger){
		this->btnSelect.super.trigger = false;
		soundSePlayOK();
		// ポップアップ閉じる
		this->result = this->picked;
		return 1;
	}

	// 閉じるボタン押下確認
	if(this->btnClose.super.trigger || this->btnOuter.trigger || global.key.bk.triggerInactive){
		this->btnClose.super.trigger = false;
		this->btnOuter.trigger = false;
		global.key.bk.triggerInactive = false;
		soundSePlayNG();
		// ポップアップ閉じる
		this->result = -2;
		return 1;
	}

	return 0; 
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct popupCartridgeAtelierMeisterList *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// ウインドウ外ボタン位置
		partsButtonPosition(&this->btnOuter, 0, POPUPATELIERMEISTERLIST_BY, global.screen.w, POPUPATELIERMEISTERLIST_BH + 40);

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ベルトとタイトル作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageWhiteCreateArray(&this->belt, 0, POPUPATELIERMEISTERLIST_BY, global.screen.w, POPUPATELIERMEISTERLIST_BH);
		e3dImageBoxDefaultCreateArray(&this->titleWindow, (global.screen.w - 200) * 0.5, 5, 200, 30);
		e3dImageTextCreateArray(&this->titleLabel, global.screen.w * 0.5, 5 + 30 * 0.5, "マイスターせんたく", 1.0, 0, 0);
		// チュートリアル矢印作成
		e3dImageCreateArray(&this->tutorialArrow1, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, (global.screen.w - 70) * 0.5 + 20 - 40, POPUPATELIERMEISTERLIST_BY + 25 + (100 - 40) * 0.5, 40, 40, TEXPOS_SYSTEM_IMAGETUTORIALARROW_X, TEXPOS_SYSTEM_IMAGETUTORIALARROW_Y, TEXPOS_SYSTEM_IMAGETUTORIALARROW_W, TEXPOS_SYSTEM_IMAGETUTORIALARROW_H);
		e3dImageCreateArray(&this->tutorialArrow2, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, global.screen.w - 100 - 5 - 60 - 5 + 20 - 40, POPUPATELIERMEISTERLIST_BY + POPUPATELIERMEISTERLIST_BH + 5 + (30 - 40) * 0.5, 40, 40, TEXPOS_SYSTEM_IMAGETUTORIALARROW_X, TEXPOS_SYSTEM_IMAGETUTORIALARROW_Y, TEXPOS_SYSTEM_IMAGETUTORIALARROW_W, TEXPOS_SYSTEM_IMAGETUTORIALARROW_H);

		// 選択ボタン
		partsButtonBoxCreateArray(&this->btnSelect, global.screen.w - 100 - 5 - 60 - 5, POPUPATELIERMEISTERLIST_BY + POPUPATELIERMEISTERLIST_BH + 5, 100, 30, "けってい", 1.0);
		// 閉じるボタン
		partsButtonBoxCreateArray(&this->btnClose, global.screen.w - 60 - 5, POPUPATELIERMEISTERLIST_BY + POPUPATELIERMEISTERLIST_BH + 5, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// バッファ作成
static void createBufferMeisterProfile(struct popupCartridgeAtelierMeisterList *this){
	struct dataMeister *dataMeister = NULL;
	// 状態確認
	int status = 1 | ((this->picked & 0x7fff) << 1);
	if(this->picked < 0){
		// 選択無し
		status = 0;
	}else{
		dataMeister = dataMeisterGet(this->picked, NULL);
		if(dataMeister == NULL){
			// ロード中
			status = 0;
		}
	}
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferMeisterProfileStatus != status || this->bufferMeisterProfileScreen != screen){
		this->bufferMeisterProfileStatus = status;
		this->bufferMeisterProfileScreen = screen;

		if(this->bufferMeisterProfileStatus){
			// バッファ作成開始
			engineGraphicBufferBegin();

			e3dImageCreateArray(&this->ProfileImage, TEXSIZ_MEISTER_W, TEXSIZ_MEISTER_H, global.screen.w * 0.5 - 10 - 140, 50, 140, 180, TEXPOS_MEISTER_PROFILE_X, TEXPOS_MEISTER_PROFILE_Y, TEXPOS_MEISTER_PROFILE_W, TEXPOS_MEISTER_PROFILE_H);
			e3dImageTextCreateArray(&this->ProfileName, global.screen.w * 0.5, 50, dataMeister->fullname, 1.0, 1, 1);
			e3dImageTextCreateArray(&this->ProfileText, global.screen.w * 0.5, 80, dataMeister->profile, 1.0, 1, 1);

			// バッファ作成完了
			engineGraphicBufferEnd(&this->e3dIdMeisterProfileVert, NULL, &this->e3dIdMeisterProfileTexc, &this->e3dIdMeisterProfileFace);
		}
	}
}

// 描画
static void draw(struct popupCartridgeAtelierMeisterList *this){
	// 描画準備
	createBuffer(this);
	createBufferMeisterProfile(this);

	struct engineMathMatrix44 tempmat1;
	struct engineMathMatrix44 tempmat2;
	double num = (double)this->super.openStep / this->super.openStepMax;
	double alpha = 1 - num * num;
	
	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);
	
	// 背景半透明幕描画
	engineMathMat4Identity(&tempmat1);
	engineGraphicEngineSetMatrix(&tempmat1);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * 0.5);
	e3dImageDraw(&this->back);

	engineMathMat4Copy(&tempmat1, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat1, global.screen.w * 0.5, global.screen.h * 0.5, 0);
	engineMathMat4Scale(&tempmat1, 1 + 0.5 * num * num, 1 + 0.5 * num * num, 1.0);
	engineMathMat4Translate(&tempmat1, -global.screen.w * 0.5, -global.screen.h * 0.5, 0);
	engineGraphicEngineSetMatrix(&tempmat1);
	// タイトル描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	e3dImageDraw(&this->titleWindow);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->titleLabel);
	// ボタン描画
	partsButtonBoxDraw(&this->btnSelect, &tempmat1, alpha);
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);

	// チュートリアル矢印描画
	if(this->stepTutorial2 > 0){
		engineMathMat4Copy(&tempmat2, &tempmat1);
		engineMathMat4Translate(&tempmat2, -engineMathAbs(10 * engineMathSin(3.1415 * this->stepTutorial2 * 0.05)), 0, 0);
		engineGraphicEngineSetMatrix(&tempmat2);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
		e3dImageDraw(&this->tutorialArrow2);
	}

	// マイスタープロフィール描画
	if(this->bufferMeisterProfileStatus){
		engineGraphicEngineBindVertVBO(this->e3dIdMeisterProfileVert);
		engineGraphicEngineBindTexcVBO(this->e3dIdMeisterProfileTexc);
		engineGraphicEngineBindFaceIBO(this->e3dIdMeisterProfileFace);

		engineGraphicEngineSetMatrix(&tempmat1);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);

		// プロフィール描画
		e3dImageDraw(&this->ProfileName);
		e3dImageDraw(&this->ProfileText);

		// マイスター画像描画
		int e3dIdMeisterImage;
		struct dataMeister *dataMeister = dataMeisterGet(this->picked, &e3dIdMeisterImage);
		engineGraphicEngineBindTexture(e3dIdMeisterImage);
		e3dImageDraw(&this->ProfileImage);

		engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
		engineGraphicEngineBindVertVBO(this->e3dIdVert);
		engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
		engineGraphicEngineBindFaceIBO(this->e3dIdFace);
	}

	engineMathMat4Copy(&tempmat1, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat1, 0, (POPUPATELIERMEISTERLIST_BY + POPUPATELIERMEISTERLIST_BH * 0.5), 0);
	engineMathMat4Scale(&tempmat1, 1.0, 1 - num * num, 1.0);
	engineMathMat4Translate(&tempmat1, 0, -(POPUPATELIERMEISTERLIST_BY + POPUPATELIERMEISTERLIST_BH * 0.5), 0);
	engineGraphicEngineSetMatrix(&tempmat1);

	// ベルトマスク設定
	engineGraphicEngineClearStencil();
	engineGraphicEngineSetStencilMode(ENGINEGRAPHICENGINEMODESTENCIL_MASK_1);
	e3dImageDraw(&this->belt);
	engineGraphicEngineSetStencilMode(ENGINEGRAPHICENGINEMODESTENCIL_READ_EQ1);

	// ベルト描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha * 0.5);
	e3dImageDraw(&this->belt);

	// マイスターボタン描画
	for(int i = 0; i < STORAGEMEISTER0001_KIND; i++){
		double charge = -1;
		for(int j = 0; j < storageAtelierGet()->workspaceSize; j++){if(this->btnMeister[i]->meisterKind == storageAtelierGet()->workspace[j].meisterKind){charge = storageAtelierGet()->workspace[j].charge;}}
		buttonAtelierMeisterDraw(this->btnMeister[i], &tempmat1, charge, alpha);
	}

	engineGraphicEngineSetStencilMode(ENGINEGRAPHICENGINEMODESTENCIL_NONE);

	// チュートリアル矢印描画
	if(this->stepTutorial1 > 0){
		engineGraphicEngineBindVertVBO(this->e3dIdVert);
		engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
		engineGraphicEngineBindFaceIBO(this->e3dIdFace);
		engineMathMat4Copy(&tempmat2, &tempmat1);
		engineMathMat4Translate(&tempmat2, -engineMathAbs(10 * engineMathSin(3.1415 * this->stepTutorial1 * 0.05)) - this->slide, 0, 0);
		engineGraphicEngineSetMatrix(&tempmat2);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
		e3dImageDraw(&this->tutorialArrow1);
	}
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct popupCartridgeAtelierMeisterList *this){
	return this->result;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct popupCartridgeAtelierMeisterList *this){
	this->stat->slideMeisterList = this->slideNext;

	for(int i = 0; i < STORAGEMEISTER0001_KIND; i++){buttonAtelierMeisterDispose(this->btnMeister[i]);}
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineGraphicObjectVBODispose(this->e3dIdMeisterProfileVert);
	engineGraphicObjectVBODispose(this->e3dIdMeisterProfileTexc);
	engineGraphicObjectIBODispose(this->e3dIdMeisterProfileFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *popupAtelierMeisterList(struct pageAtelierStatus *stat, int meisterKind){
	struct popupCartridgeAtelierMeisterList *this = (struct popupCartridgeAtelierMeisterList*)engineUtilMemoryCalloc(1, sizeof(struct popupCartridgeAtelierMeisterList));
	this->stat = stat;
	this->picked = meisterKind;
	init(this);

	struct popupCartridge *cartridge = (struct popupCartridge*)this;
	popupCartridgeInit(cartridge);
	cartridge->popupCalcBefore = (void(*)(struct popupCartridge*))popupCalcBefore;
	cartridge->popupCalcMain = (int(*)(struct popupCartridge*))popupCalcMain;
	cartridge->draw = (void(*)(struct popupCartridge*))draw;
	cartridge->result = (int(*)(struct popupCartridge*))result;
	cartridge->dispose = (void(*)(struct popupCartridge*))dispose;
	return cartridge;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

