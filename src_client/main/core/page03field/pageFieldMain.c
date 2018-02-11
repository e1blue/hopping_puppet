#include "../includeAll.h"
#include "pageField.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#define PAGEFIELDMAIN_TENSIONNUM 6

// ページカートリッジ構造体
struct pageCartridgeFieldMain{
	struct pageCartridge super;
	struct pageFieldStatus *stat;
	struct popupCartridge *popupOpen;
	struct popupCartridge *popupInfo;
	struct popupCartridge *popupItemGet;
	struct popupCartridge *popupItemSelect;
	struct popupCartridge *popupItemUse1;
	struct popupCartridge *popupItemUse2;
	struct popupCartridge *popupTutorial;
	int stepTutorial01;
	int stepTutorial02;
	int stepTutorial11;
	int stepTutorial12;

	struct partsButtonBox btnCheck;
	struct partsButtonBox btnItem;
	struct partsButtonBox btnBack;

	struct e3dImage fieldMask;
	struct e3dImage fieldWindow;
	struct e3dImage fieldName;
	struct e3dImage fieldCircle[4];
	struct e3dImage fieldDecoration[6];
	struct e3dImage fieldEnemy;
	struct e3dImage fieldGauge;
	struct e3dImage playerWindow;
	struct e3dImage playerImage;
	struct e3dImage playerTension;
	struct e3dImage playerName;
	struct e3dImage playerGauge;
	struct e3dImage playerHealthMax;
	struct e3dNumber playerHealth;
	struct e3dImage tutorialArrow1;
	struct e3dImage tutorialArrow2;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	// マップデータ
	bool mapInit;
	int mapSelectIndex;
	struct{
		int mapchip;
		int icon;
		bool isIcon;
		int step;
	} mapData[DATAFIELD_MAPMAX];
	struct{int eventIndex; int step;} toggleField;
	struct{int enemyIndex; int step;} toggleEnemy;

	// プレイヤーデータ
	struct{
		double healthPrev;
		struct{int x; int y;} tensionEffect[PAGEFIELDMAIN_TENSIONNUM];
	} playerData;

	// エネミーデータ
	struct{
		int step;
	} enemyData[STORAGEFIELD0001_ENEMYMAX];

	// ワールド行列
	struct engineMathMatrix44 worldMat;
	// 画面移動
	double sx;
	double sy;
	double sz;
	double sxNext;
	double syNext;
	double szNext;
	// 画面回転
	double rotv;
	double roth;
	double dist;
	double rotvNext;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageCartridgeFieldMain *this){
	// ボタン作成
	partsButtonBoxInit(&this->btnCheck);
	partsButtonBoxInit(&this->btnItem);
	partsButtonBoxInit(&this->btnBack);

	// プレイヤー初期化
	this->playerData.healthPrev = storageStatusGet()->health;
	for(int i = 0; i < PAGEFIELDMAIN_TENSIONNUM; i++){
		this->playerData.tensionEffect[i].x = engineMathRandomGet() % 44;
		this->playerData.tensionEffect[i].y = engineMathRandomGet() % 44;
	}

	// 画面回転
	this->rotvNext = storageStatusGet()->fieldr * (3.1415 / 6);
	this->rotv = this->rotvNext + 3.1415 / 180 * 30;
	this->roth = 3.1415 / 180 * 30;
	this->dist = 10;
}

// ----------------------------------------------------------------

// 画面回転の計算
static void calcRotate(struct pageCartridgeFieldMain *this){
	static int prevsx;
	static int prevsy;
	// タッチ状態の確認
	struct engineCtrlTouch *t = engineCtrlTouchGet(ENGINECTRLTOUCHID_SCREEN);
	if(t != NULL){
		if(t->dn){
			if(!t->active){
				// タッチ開始時
				prevsx = t->screen.x;
				prevsy = t->screen.y;
				engineCtrlTouchOwn();
			}

			if(t->mv){
				// フィールド回転処理
				double cx = global.screen.w * 0.5;
				double cy = global.screen.h * 0.5;
				double x0 = prevsx - cx;
				double y0 = prevsy - cy;
				double r0 = engineMathSqrt(x0 * x0 + y0 * y0);
				double x1 = t->screen.x - cx;
				double y1 = t->screen.y - cy;
				double r1 = engineMathSqrt(x1 * x1 + y1 * y1);
				if(r0 > 20 && r1 > 20){
					double cos = (x0 * x1 + y0 * y1) / (r0 * r1);
					if(cos > 1){cos = 1;}else if(cos < -1){cos = -1;}
					this->rotvNext -= 0.8 * engineMathAcos(cos) * ((x0 * y1 - y0 * x1 > 0) ? 1 : -1);
				}
			}
			prevsx = t->screen.x;
			prevsy = t->screen.y;
		}else{
			if(!t->mv && (t->screen.x < global.screen.w - 110 || t->screen.y < global.screen.h - 110)){
				// タッチ処理
				struct dataField *dataField = dataFieldGet(storageStatusGet()->fieldKind, NULL);
				if(dataField != NULL){
					int touchIndex = -1;
					double touchDepth = 1;
					double touchx = 0;
					double touchy = 0;
					double touchz = 0;
					for(int i = 0; i < dataField->xsize; i++){
						for(int j = 0; j < dataField->ysize; j++){
							int index = dataField->xsize * j + i;
							if(this->mapData[index].mapchip <= 0){continue;}
							// 円柱中心の画面座標上位置を計算
							struct engineMathMatrix44 tempmat;
							engineMathMat4Copy(&tempmat, &this->worldMat);
							double x0 = (i + j * 0.5) * (80 + 5);
							double y0 = j * (80 + 5) * 0.86602540378;
							double z0 = (dataField->map[index].height - 1) * 35.0;
							engineMathMat4Translate(&tempmat, x0, z0, y0);
							double x1 = (1 + tempmat.m30 / tempmat.m33) * 0.5 * global.window.w;
							double y1 = (1 - tempmat.m31 / tempmat.m33) * 0.5 * global.window.h;
							double z1 = tempmat.m32 / tempmat.m33;
							double x2 = (x1 - t->window.x) * global.screen.aspect.x * global.screen.w / global.window.w;
							double y2 = (y1 - t->window.y) * global.screen.aspect.y * global.screen.h / global.window.h;
							bool isCircle = (-40 < x2 && x2 < 40 && -45 < y2 && y2 < 15);
							bool isIcon = (this->mapData[index].isIcon && -25 < x2 && x2 < 25 && -45 < y2 && y2 < 45);
							if(touchDepth > z1 && (isCircle || isIcon)){
								touchIndex = index;
								touchDepth = z1;
								touchx = x0;
								touchy = y0;
								touchz = z0;
							}
						}
					}

					if(touchIndex >= 0){
						soundSePlayOK();
						double r = this->rotvNext / (3.1415 / 6) + 0.5;
						while(r <  0){r += 12;}
						while(r > 12){r -= 12;}
						this->mapSelectIndex = touchIndex;
						this->sxNext = touchx;
						this->syNext = touchy;
						this->szNext = touchz;
						storageStatusGet()->fieldx = touchIndex % dataField->xsize;
						storageStatusGet()->fieldy = engineMathFloor(touchIndex / dataField->xsize);
						storageStatusGet()->fieldr = r;
						storageStatusUpdate();
					}
				}
			}
			// タッチ終了時
			engineCtrlTouchFree();
		}
	}
}

// 減衰計算
static void calcDamper(struct pageCartridgeFieldMain *this){
	// 画面移動の減衰計算
	double dsx = this->sx - this->sxNext;
	double dsy = this->sy - this->syNext;
	double dsz = this->sz - this->szNext;
	if(engineMathAbs(dsx) > 0.01){this->sx -= dsx * 0.2;}
	if(engineMathAbs(dsy) > 0.01){this->sy -= dsy * 0.2;}
	if(engineMathAbs(dsz) > 0.01){this->sz -= dsz * 0.2;}
	// 画面回転角度の減衰計算
	double drv = this->rotv - this->rotvNext;
	while(drv < -3.1415){drv += 3.1415 * 2;}
	while(drv >  3.1415){drv -= 3.1415 * 2;}
	if(engineMathAbs(drv) > 0.01){this->rotv -= drv * 0.2;}
}

// 円柱の解放可否と位置確認
static int circleCheck(struct pageCartridgeFieldMain *this, struct dataField *dataField, struct storageFieldList *storageField, int fieldx, int fieldy, int fieldz){
	if(fieldx < 0 || dataField->xsize <= fieldx){return -1;}
	if(fieldy < 0 || dataField->ysize <= fieldy){return -1;}
	int index = dataField->xsize * fieldy + fieldx;
	int height = dataField->map[index].height;
	if(height <= 0 || height < fieldz - 1 || fieldz + 1 < height){return -1;}
	if(storageField->map[index].open & 0x01){return -1;}
	return index;
}

// 円柱の解放
static bool circleOpen(struct pageCartridgeFieldMain *this, struct dataField *dataField, struct storageFieldList *storageField, int fieldx, int fieldy){
	int fieldz = dataField->map[dataField->xsize * fieldy + fieldx].height;

	// 解放する円柱の解放可否と位置確認
	int openIndexCenter = circleCheck(this, dataField, storageField, fieldx + 0, fieldy + 0, fieldz);
	int openIndexCircleList[6];
	int openIndexCircleNum = 6;
	openIndexCircleList[0] = circleCheck(this, dataField, storageField, fieldx + 0, fieldy - 1, fieldz);
	openIndexCircleList[1] = circleCheck(this, dataField, storageField, fieldx + 1, fieldy - 1, fieldz);
	openIndexCircleList[2] = circleCheck(this, dataField, storageField, fieldx + 1, fieldy + 0, fieldz);
	openIndexCircleList[3] = circleCheck(this, dataField, storageField, fieldx + 0, fieldy + 1, fieldz);
	openIndexCircleList[4] = circleCheck(this, dataField, storageField, fieldx - 1, fieldy + 1, fieldz);
	openIndexCircleList[5] = circleCheck(this, dataField, storageField, fieldx - 1, fieldy + 0, fieldz);

	// 解放にあたり一番見栄えの良い開始位置を調べる
	int openIndexCircleStart = 0;
	int maxScore = 0;
	for(int i = 0; i < openIndexCircleNum; i++){
		int score = 0;
		for(int j = 0; j < openIndexCircleNum; j++){
			int idx = openIndexCircleList[(i + j) % openIndexCircleNum];
			if(idx >= 0){score += (1 << (openIndexCircleNum - j));}
		}
		if(maxScore < score){
			openIndexCircleStart = i;
			maxScore = score;
		}
	}

	int openNum = 0;
	bool isOpen = false;
	// 中心の解放処理を行う
	if(openIndexCenter >= 0){
		storageField->map[openIndexCenter].open |= 0x01;
		this->mapData[openIndexCenter].step = -30 - 20 * (openNum++);
		isOpen = true;
	}
	// 開始位置から解放処理を行う
	for(int i = 0; i < openIndexCircleNum; i++){
		int idx = openIndexCircleList[(i + openIndexCircleStart) % openIndexCircleNum];
		if(idx >= 0){
			storageField->map[idx].open |= 0x01;
			this->mapData[idx].step = -30 - 20 * (openNum++);
			isOpen = true;
		}
	}
	if(isOpen){storageFieldUpdate();}
	return isOpen;
}

// エネミーが出現するマップ位置計算
static int calcMapIndex(struct pageCartridgeFieldMain *this, struct dataField *dataField, struct storageFieldList *storageField, int *frequencyTable){
	int size = dataField->xsize * dataField->ysize;
	if(frequencyTable[0] < 0){
		// マップ位置確率表作成
		for(int j = 0; j < size; j++){
			frequencyTable[j] = 0;
			int fieldx = j % dataField->xsize;
			int fieldy = j / dataField->xsize;
			int fieldz = dataField->map[j].height;
			if(fieldz <= 0){continue;}
			if(!(storageField->map[j].open & 0x01)){continue;}
			if(this->mapData[j].step != 0){continue;}
			if(storageStatusGet()->fieldx == fieldx && storageStatusGet()->fieldy == fieldy){continue;}
			frequencyTable[j] += 1;
			if(circleCheck(this, dataField, storageField, fieldx + 0, fieldy - 1, fieldz) >= 0){frequencyTable[j] += 40;}
			if(circleCheck(this, dataField, storageField, fieldx + 1, fieldy - 1, fieldz) >= 0){frequencyTable[j] += 40;}
			if(circleCheck(this, dataField, storageField, fieldx + 1, fieldy + 0, fieldz) >= 0){frequencyTable[j] += 40;}
			if(circleCheck(this, dataField, storageField, fieldx + 0, fieldy + 1, fieldz) >= 0){frequencyTable[j] += 40;}
			if(circleCheck(this, dataField, storageField, fieldx - 1, fieldy + 1, fieldz) >= 0){frequencyTable[j] += 40;}
			if(circleCheck(this, dataField, storageField, fieldx - 1, fieldy + 0, fieldz) >= 0){frequencyTable[j] += 40;}
		}
		for(int j = 0; j < STORAGEFIELD0001_ENEMYMAX; j++){
			if(storageField->enemy[j].enemyKind < 0){continue;}
			int fieldx = storageField->enemy[j].fieldx;
			int fieldy = storageField->enemy[j].fieldy;
			int index = dataField->xsize * fieldy + fieldx;
			frequencyTable[index] = 0;
		}
	}

	// マップ位置抽選
	int mapIndex = -1;
	int mapTotalFrequency = 0;
	for(int j = 0; j < size; j++){mapTotalFrequency += frequencyTable[j];}
	if(mapTotalFrequency > 0){
		int mapRandomValue = engineMathRandomGet() % mapTotalFrequency;
		for(int j = 0; j < size; j++){
			if(mapRandomValue < frequencyTable[j]){
				mapIndex = j;
				break;
			}else{mapRandomValue -= frequencyTable[j];}
		}
	}

	return mapIndex;
}

// エネミー作成
static void createEnemy(struct pageCartridgeFieldMain *this, struct dataField *dataField, struct storageFieldList *storageField, int enemyIndex, int mapIndex, int enemyProductIndex){
	struct dataFieldProduct *product = &dataField->product[enemyProductIndex];
	storageField->enemy[enemyIndex].fieldx = mapIndex % dataField->xsize;
	storageField->enemy[enemyIndex].fieldy = mapIndex / dataField->xsize;
	storageField->enemy[enemyIndex].enemyKind = product->enemyKind;
	storageField->enemy[enemyIndex].level = product->level;
	storageField->enemy[enemyIndex].health = 0x7fffffff;
	storageField->enemy[enemyIndex].healthPercent = 100;
	storageField->enemy[enemyIndex].itemKind = product->itemKind;
	storageField->enemy[enemyIndex].isItemGet = ((engineMathRandomGet() % 100) < product->itemPercent);			
	this->enemyData[enemyIndex].step = -50;
}

// 時間経過計算
static void calcTimeStep(struct pageCartridgeFieldMain *this, struct dataField *dataField, struct storageFieldList *storageField){
	long long int time = (long long int)platformPluginUtilTimeGet();

	// 時間経過による体力の回復
	int timeDiff = (int)(time - storageStatusGet()->time);
	if(timeDiff > 0){
		int health = storageStatusGet()->health;
		int healthMax = parameterPlayerHealthMax();
		health += timeDiff;
		if(health > healthMax){health = healthMax;}
		if(health >= healthMax){storageStatusGet()->isWeak = false;} // 体力全快で虚弱体質から復帰
		storageStatusGet()->health = health;
		storageStatusGet()->time = time;
		storageStatusUpdate();
	}

	// 時間経過によるエネミーの出現
	timeDiff = (int)(time - storageField->time);
	if(timeDiff > 0 && this->mapInit){
		storageField->charge += timeDiff;

		if(!storageStatusGet()->tutorial.jump03){
			// チュートリアル時エネミー出現確認
			if(storageField->enemy[0].enemyKind >= 0){storageField->charge = 0;}
			if(storageField->charge >= dataField->charge){
				storageField->charge = 0;
				createEnemy(this, dataField, storageField, 0, 9, 0);
				storageField->enemy[0].isItemGet = true;
			}
		}else{
			// 通常時エネミー出現確認
			int countEnemy = 0;
			for(int i = 0; i < STORAGEFIELD0001_ENEMYMAX; i++){if(storageField->enemy[i].enemyKind >= 0){countEnemy++;}}
			if(countEnemy < STORAGEFIELD0001_ENEMYMAX){
				// 出現するエネミーの種類数と出現したエネミーの数を確認
				int productKindNum = 0;
				int productedKindNum = 0;
				int productedEnemyNum[DATAFIELD_PRODUCTMAX] = {0};
				for(int i = 0; i < DATAFIELD_PRODUCTMAX; i++){
					struct dataFieldProduct *product = &dataField->product[i];
					if(product->frequency <= 0){continue;}
					for(int j = 0; j < STORAGEFIELD0001_ENEMYMAX; j++){if(product->enemyKind == storageField->enemy[i].enemyKind){productedEnemyNum[i]++;}}
					if(productedEnemyNum[i] > 0){productedKindNum++;}
					productKindNum++;
				}

				// エネミーを出現させる
				int frequencyTable[DATAFIELD_MAPMAX] = {-1};
				for(int i = 0; i < STORAGEFIELD0001_ENEMYMAX; i++){
					if(storageField->charge < dataField->charge){break;}
					if(storageField->enemy[i].enemyKind >= 0){continue;}

					// 全ての種類のエネミーを極力1体以上出現させるために、出現済みエネミーの出現制限フラグ
					bool isRestrict = (STORAGEFIELD0001_ENEMYMAX - countEnemy <= productKindNum - productedKindNum);

					// マップ位置抽選
					int mapIndex = calcMapIndex(this, dataField, storageField, frequencyTable);
					if(mapIndex < 0){continue;}

					// エネミー抽選
					int enemyProductIndex = -1;
					int enemyTotalFrequency = 0;
					for(int j = 0; j < DATAFIELD_PRODUCTMAX; j++){
						if(isRestrict && productedEnemyNum[j] > 0){continue;}
						struct dataFieldProduct *product = &dataField->product[j];
						if(product->frequency <= 0){continue;}
						enemyTotalFrequency += product->frequency;
					}
					int enemyRandomValue = engineMathRandomGet() % enemyTotalFrequency;
					for(int j = 0; j < DATAFIELD_PRODUCTMAX; j++){
						if(isRestrict && productedEnemyNum[j] > 0){continue;}
						struct dataFieldProduct *product = &dataField->product[j];
						if(product->frequency <= 0){continue;}
						if(enemyRandomValue < product->frequency){enemyProductIndex = j; break;}
						enemyRandomValue -= product->frequency;
					}
					if(enemyProductIndex < 0){continue;}

					// マップにエネミー作成
					storageField->charge -= dataField->charge;
					createEnemy(this, dataField, storageField, i, mapIndex, enemyProductIndex);
					countEnemy++;
					productedKindNum++;
					productedEnemyNum[enemyProductIndex]++;
					frequencyTable[mapIndex] = 0;
				}
			}

			if(countEnemy >= STORAGEFIELD0001_ENEMYMAX){storageField->charge = 0;}
		}

		storageField->time = time;
		storageFieldUpdate();
	}
}

// 計算
static int calc(struct pageCartridgeFieldMain *this){
	// メモリートレースボタン計算
	partsButtonMemoryCalc();
	
	calcDamper(this);
	bool isOpenCircle = false;
	struct dataField *dataField = dataFieldGet(storageStatusGet()->fieldKind, NULL);
	struct storageFieldList *storageField = &storageFieldGet()->list[storageStatusGet()->fieldKind];
	calcTimeStep(this, dataField, storageField);

	// マップ計算
	int mapSelectEnemyIndex = -1;
	int mapSelectEventIndex = -1;
	int mapSelectLockNum = 0;
	if(dataField != NULL){
		int fieldx = storageStatusGet()->fieldx;
		int fieldy = storageStatusGet()->fieldy;
		int index = dataField->xsize * fieldy + fieldx;
		int fieldz = dataField->map[index].height;

		if(!this->mapInit){
			// マップ初期化処理
			this->mapInit = true;
			this->mapSelectIndex = -1;

			// 画面移動
			this->sxNext = (fieldx + fieldy * 0.5) * (80 + 5);
			this->syNext = fieldy * (80 + 5) * 0.86602540378;
			this->szNext = (dataField->map[index].height - 1) * 35.0;
			this->sx = this->sxNext;
			this->sy = this->syNext;
			this->sz = this->szNext;

			for(int i = 0; i < DATAFIELD_MAPMAX; i++){this->mapData[i].step = 0;}
			for(int i = 0; i < STORAGEFIELD0001_ENEMYMAX; i++){this->enemyData[i].step = 0;}

			// かべのなかにいる解放確認
			if(!(storageField->map[index].open & 0x01)){
				if(false){
					// テスト用全開放
					int size = dataField->xsize * dataField->ysize;
					for(int i = 0; i < size; i++){
						int fieldx = i % dataField->xsize;
						int fieldy = i / dataField->xsize;
						int fieldz = dataField->map[i].height;
						int openIndex = circleCheck(this, dataField, storageField, fieldx, fieldy, fieldz);
						if(openIndex >= 0){storageField->map[openIndex].open |= 0x01;}
					}
				}
				if(circleOpen(this, dataField, storageField, fieldx, fieldy)){soundSePlayOpenCircle();}
				storageField->time = platformPluginUtilTimeGet() - (dataField->charge - 5);
				storageFieldUpdate();
			}

			// 敵撃破解放確認
			for(int i = 0; i < STORAGEFIELD0001_ENEMYMAX; i++){
				if(storageField->enemy[i].enemyKind < 0){continue;}
				if(storageField->enemy[i].health <= 0){
					if(circleOpen(this, dataField, storageField, fieldx, fieldy)){soundSePlayOpenCircle();}
					storageField->enemy[i].enemyKind = -1;
					storageFieldUpdate();
				}
			}
		}

		if(this->mapSelectIndex >= 0){
			// 解放可否確認
			if(circleCheck(this, dataField, storageField, fieldx + 0, fieldy + 0, fieldz) >= 0){mapSelectLockNum++;}
			if(circleCheck(this, dataField, storageField, fieldx + 0, fieldy - 1, fieldz) >= 0){mapSelectLockNum++;}
			if(circleCheck(this, dataField, storageField, fieldx + 1, fieldy - 1, fieldz) >= 0){mapSelectLockNum++;}
			if(circleCheck(this, dataField, storageField, fieldx + 1, fieldy + 0, fieldz) >= 0){mapSelectLockNum++;}
			if(circleCheck(this, dataField, storageField, fieldx + 0, fieldy + 1, fieldz) >= 0){mapSelectLockNum++;}
			if(circleCheck(this, dataField, storageField, fieldx - 1, fieldy + 1, fieldz) >= 0){mapSelectLockNum++;}
			if(circleCheck(this, dataField, storageField, fieldx - 1, fieldy + 0, fieldz) >= 0){mapSelectLockNum++;}
		}

		// 円柱計算
		for(int i = 0; i < dataField->xsize; i++){
			for(int j = 0; j < dataField->ysize; j++){
				int index = dataField->xsize * j + i;
				bool isOpen = (storageField->map[index].open & 0x01) && (this->mapData[index].step > -15);
				this->mapData[index].mapchip = isOpen ? (1 + dataField->map[index].mapchip) : 0;
				this->mapData[index].icon = -1;
				this->mapData[index].isIcon = false;
				if(this->mapSelectIndex != index && this->mapData[index].step >= 30){this->mapData[index].step %= 30;}
				if(this->mapSelectIndex == index || this->mapData[index].step != 0){this->mapData[index].step++;}
			}
		}
		// イベント計算
		for(int i = 0; i < DATAFIELD_EVENTMAX; i++){
			if(dataField->event[i].type == DATAFIELDEVENTTYPE_NONE){continue;}
			int index = dataField->xsize * dataField->event[i].fieldy + dataField->event[i].fieldx;
			if(!(storageField->map[index].open & 0x02)){
				if(this->mapData[index].mapchip > 0){this->mapData[index].icon = dataField->event[i].icon; this->mapData[index].isIcon = true;}
				if(this->mapSelectIndex == index && dataField->event[i].type != DATAFIELDEVENTTYPE_ICON){mapSelectEventIndex = i;}
			}
		}
	}

	if(storageStatusGet()->isWeak){
		this->playerData.healthPrev = 0;
	}else{
		// プレイヤー体力ゲージ減衰
		int playerHealth = storageStatusGet()->health;
		this->playerData.healthPrev -= (this->playerData.healthPrev - playerHealth) * 0.1;
		if(engineMathAbs(this->playerData.healthPrev - playerHealth) < 0.5){this->playerData.healthPrev = playerHealth;}
	}
	// プレイヤーテンション計算
	for(int i = 0; i < PAGEFIELDMAIN_TENSIONNUM; i++){
		int y = this->playerData.tensionEffect[i].y - 1;
		if(y < 0){y = 44; this->playerData.tensionEffect[i].x = engineMathRandomGet() % 44;}
		this->playerData.tensionEffect[i].y = y;
	}

	// エネミー計算
	if(dataField != NULL){
		for(int i = 0; i < STORAGEFIELD0001_ENEMYMAX; i++){
			if(storageField->enemy[i].enemyKind < 0){continue;}
			int fieldx = storageField->enemy[i].fieldx;
			int fieldy = storageField->enemy[i].fieldy;
			int index = dataField->xsize * fieldy + fieldx;
			if(this->mapSelectIndex == index){mapSelectEnemyIndex = i;}
			this->mapData[index].isIcon = true;
			this->enemyData[i].step++;
		}
	}

	// チュートリアルステップカウント
	if(!storageStatusGet()->tutorial.jump03 && storageStatusGet()->tutorial.field01 && parameterPlayerPuppetNum() > 0){
		if(this->mapSelectIndex == 9){
			this->stepTutorial11 = 0;
			this->stepTutorial12++;
		}else{
			this->stepTutorial11++;
			this->stepTutorial12 = 0;
		}
	}else{this->stepTutorial11 = this->stepTutorial12 = 0;}

	if(this->popupOpen != NULL && dataField != NULL){
		// 解放ポップアップ計算
		if(!this->popupOpen->calc(this->popupOpen)){return 0;}
		// 解放ポップアップ完了時処理
		if(this->popupOpen->result(this->popupOpen) && (storageStatusGet()->money >= dataField->openCost)){
			this->mapSelectIndex = -1;
			int fieldx = storageStatusGet()->fieldx;
			int fieldy = storageStatusGet()->fieldy;
			if(circleOpen(this, dataField, storageField, fieldx, fieldy)){soundSePlayOpenCircle();}
			storageStatusGet()->money -= dataField->openCost;
			storageStatusUpdate();
		}
		this->popupOpen->dispose(this->popupOpen);
		this->popupOpen = NULL;
	}

	if(this->popupInfo != NULL){
		// 情報ポップアップ計算
		if(!this->popupInfo->calc(this->popupInfo)){return 0;}
		// 情報ポップアップ完了時処理
		if(this->popupInfo->result(this->popupInfo)){}
		this->popupInfo->dispose(this->popupInfo);
		this->popupInfo = NULL;
	}

	if(this->popupItemGet != NULL){
		// アイテム獲得ポップアップ計算
		if(!this->popupItemGet->calc(this->popupItemGet)){return 0;}
		// アイテム獲得ポップアップ完了時処理
		if(this->popupItemGet->result(this->popupItemGet)){}
		this->popupItemGet->dispose(this->popupItemGet);
		this->popupItemGet = NULL;
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
		if(itemKind == DATAITEMKIND_HEALTH && storageStatusGet()->itemNum[itemKind] > 0){
			// 体力回復アイテムの使用
			storageStatusGet()->itemNum[itemKind]--;
			storageStatusGet()->health = parameterPlayerHealthMax();
			storageStatusUpdate();
		}
		if(itemKind == DATAITEMKIND_TENSION && storageStatusGet()->itemNum[itemKind] > 0){
			// テンション上昇アイテムの使用
			storageStatusGet()->itemNum[itemKind]--;
			storageStatusGet()->isTension = true;
			storageStatusUpdate();
		}
		if(itemKind == DATAITEMKIND_MONSTERMOVE && storageStatusGet()->itemNum[itemKind] > 0 && dataField != NULL){
			// エネミー移動アイテムの使用
			storageStatusGet()->itemNum[itemKind]--;
			int frequencyTable[DATAFIELD_MAPMAX] = {-1};
			int enemyProductOffset = engineMathRandomGet() % STORAGEFIELD0001_ENEMYMAX;
			for(int i = 0; i < STORAGEFIELD0001_ENEMYMAX; i++){
				int enemyProductIndex = (i + enemyProductOffset) % STORAGEFIELD0001_ENEMYMAX;
				if(storageField->enemy[enemyProductIndex].enemyKind < 0){continue;}
				int mapIndex = calcMapIndex(this, dataField, storageField, frequencyTable);
				if(mapIndex < 0){continue;}
				// エネミー移動
				storageField->enemy[enemyProductIndex].fieldx = mapIndex % dataField->xsize;
				storageField->enemy[enemyProductIndex].fieldy = mapIndex / dataField->xsize;
				this->enemyData[enemyProductIndex].step = -50;
				frequencyTable[mapIndex] = 0;
			}
			storageStatusUpdate();
			storageFieldUpdate();
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
		if(itemKind == DATAITEMKIND_HEALTH){}
		if(itemKind == DATAITEMKIND_TENSION){}
		if(itemKind == DATAITEMKIND_MONSTERMOVE){}
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
	}else if(!storageStatusGet()->tutorial.field01 && storageField->enemy[0].enemyKind >= 0){
		// チュートリアル計算
		if(this->stepTutorial01 == 60){soundSePlayOK(); this->popupTutorial = partsPopupTutorialField01();}
		if(this->stepTutorial01 == 60){storageStatusGet()->tutorial.field01 = true; storageStatusUpdate();}
		this->stepTutorial01++;
		return 0;
	}else if(!storageStatusGet()->tutorial.field02 && storageStatusGet()->tutorial.jump03){
		// チュートリアル計算
		if(this->stepTutorial02 == 120){soundSePlayOK(); this->popupTutorial = partsPopupTutorialField02A();}
		if(this->stepTutorial02 == 180){soundSePlayOK(); this->popupTutorial = partsPopupTutorialField02B();}
		if(this->stepTutorial02 == 180){
			// チュートリアル完了
			storageStatusGet()->tutorial.top01 = true;
			storageStatusGet()->tutorial.top02 = true;
			storageStatusGet()->tutorial.atelier01 = true;
			storageStatusGet()->tutorial.atelier02 = true;
			storageStatusGet()->tutorial.atelier03 = true;
			storageStatusGet()->tutorial.atelier04 = true;
			storageStatusGet()->tutorial.atelier05 = true;
			storageStatusGet()->tutorial.field01 = true;
			storageStatusGet()->tutorial.field02 = true;
			storageStatusGet()->tutorial.jump01 = true;
			storageStatusGet()->tutorial.jump02 = true;
			storageStatusGet()->tutorial.jump03 = true;
			storageStatusUpdate();
		}
		this->stepTutorial02++;
		return 0;
	}

	// タッチ処理
	partsButtonBoxCalc(&this->btnCheck);
	partsButtonBoxCalc(&this->btnItem);
	partsButtonBoxCalc(&this->btnBack);
	calcRotate(this);

	// フィールド切り替え処理
	if(this->toggleField.step != 0){
		if(this->toggleField.eventIndex >= 0){
			if(abs((this->toggleField.step > 0) ? ++this->toggleField.step : --this->toggleField.step) >= 30){
				// フィールド切り替え
				storageStatusGet()->fieldKind = dataField->event[this->toggleField.eventIndex].nextFieldKind;
				storageStatusGet()->fieldx = dataField->event[this->toggleField.eventIndex].nextFieldx;
				storageStatusGet()->fieldy = dataField->event[this->toggleField.eventIndex].nextFieldy;
				storageStatusGet()->fieldr = dataField->event[this->toggleField.eventIndex].nextFieldr;
				storageStatusUpdate();
				this->rotv = this->rotvNext = storageStatusGet()->fieldr * (3.1415 / 6.0);
				this->mapInit = false;
				this->toggleField.eventIndex = -1;
				this->toggleField.step *= -1;
			}
		}else{if(this->toggleField.step > 0){this->toggleField.step--;}else{this->toggleField.step++;}}
		this->btnCheck.super.inactive = true;
		this->btnItem.super.inactive = true;
		this->btnBack.super.inactive = true;
		return 0;
	}

	// エネミー遷移処理
	if(this->toggleEnemy.step > 0){
		if(this->toggleEnemy.step++ > 30){
			storageStatusGet()->enemyIndex = this->toggleEnemy.enemyIndex;
			storageStatusUpdate();
			// ページ遷移
			pageCartridgePush(pageJumpInit());
			pageFieldStatusDispose(this->stat);
			return 1;
		}
		this->btnCheck.super.inactive = true;
		this->btnItem.super.inactive = true;
		this->btnBack.super.inactive = true;
		return 0;
	}

	// 調査ボタン押下確認
	this->btnCheck.super.inactive = (mapSelectEnemyIndex < 0 && mapSelectEventIndex < 0 && mapSelectLockNum <= 0);
	if(this->btnCheck.super.trigger){
		this->btnCheck.super.trigger = false;
		if(mapSelectEnemyIndex >= 0){
			if(storageStatusGet()->isWeak){
				// 虚弱体質ポップアップ展開
				soundSePlayNG();
				this->popupInfo = popupFieldInfoHealth(this->stat);
				this->btnCheck.super.inactive = true;
				this->mapSelectIndex = -1;
				return 0;
			}else if(parameterPlayerPuppetNum() <= 0){
				// パペット不足ポップアップ展開
				soundSePlayNG();
				this->popupInfo = popupFieldInfoPuppet(this->stat);
				this->btnCheck.super.inactive = true;
				this->mapSelectIndex = -1;
				return 0;
			}else{
				// BGM設定
				soundBgmStop();
				soundSePlayEnemyEnter();
				// エネミーとの戦闘開始
				this->toggleEnemy.enemyIndex = mapSelectEnemyIndex;
				this->toggleEnemy.step = 1;
				return 0;
			}
		}else if(mapSelectEventIndex >= 0 && dataField != NULL){
			// イベント処理
			if(dataField->event[mapSelectEventIndex].type == DATAFIELDEVENTTYPE_PREV){
				// 前のマップ
				soundSePlayTransfer();
				this->toggleField.eventIndex = mapSelectEventIndex;
				this->toggleField.step = -1;
			}else if(dataField->event[mapSelectEventIndex].type == DATAFIELDEVENTTYPE_NEXT){
				// 次のマップ
				soundSePlayTransfer();
				this->toggleField.eventIndex = mapSelectEventIndex;
				this->toggleField.step = 1;
			}else if(dataField->event[mapSelectEventIndex].type == DATAFIELDEVENTTYPE_TRIAL){
				// 体験版ポップアップ展開
				soundSePlayNG();
				this->popupInfo = popupFieldInfoTrial(this->stat);
				this->btnCheck.super.inactive = true;
				this->mapSelectIndex = -1;
			}else if(dataField->event[mapSelectEventIndex].type == DATAFIELDEVENTTYPE_MONEY && dataField != NULL){
				// おかね獲得
				soundSePlayGet();
				int getMoney = dataField->event[mapSelectEventIndex].money;
				int money = storageStatusGet()->money + getMoney;
				if(money > UTILPARAMETER_MONEYMAX){money = UTILPARAMETER_MONEYMAX;}
				storageStatusGet()->money = money;
				storageStatusUpdate();
				// おかねイベント完了
				int fieldx = dataField->event[mapSelectEventIndex].fieldx;
				int fieldy = dataField->event[mapSelectEventIndex].fieldy;
				int index = dataField->xsize * fieldy + fieldx;
				storageField->map[index].open |= 0x02;
				storageFieldUpdate();
				// ポップアップ展開
				this->popupItemGet = partsPopupMoneyInfoGet(getMoney);
				this->btnCheck.super.inactive = true;
				this->mapSelectIndex = -1;
				return 0;
			}else if(dataField->event[mapSelectEventIndex].type == DATAFIELDEVENTTYPE_ITEM && dataField != NULL){
				// アイテム獲得
				soundSePlayGet();
				int itemKind = dataField->event[mapSelectEventIndex].itemKind;
				int itemNum = storageStatusGet()->itemNum[itemKind] + 1;
				if(itemNum > UTILPARAMETER_ITEMNUMMAX){itemNum = UTILPARAMETER_ITEMNUMMAX;}
				storageStatusGet()->itemNum[itemKind] = itemNum;
				storageStatusUpdate();
				// アイテムイベント完了
				int fieldx = dataField->event[mapSelectEventIndex].fieldx;
				int fieldy = dataField->event[mapSelectEventIndex].fieldy;
				int index = dataField->xsize * fieldy + fieldx;
				storageField->map[index].open |= 0x02;
				storageFieldUpdate();
				// ポップアップ展開
				this->popupItemGet = partsPopupItemInfoGet(itemKind);
				this->btnCheck.super.inactive = true;
				this->mapSelectIndex = -1;
				return 0;
			}
		}else if(mapSelectLockNum > 0 && dataField != NULL){
			// 解放ポップアップ展開
			soundSePlayOK();
			this->popupOpen = popupFieldOpen(this->stat, dataField->openCost);
			return 0;
		}
	}

	// アイテムボタン押下確認
	this->btnItem.super.inactive = false;
	if(this->btnItem.super.trigger){
		this->btnItem.super.trigger = false;
		soundSePlayOK();
		// アイテム選択ポップアップ展開
		this->popupItemSelect = partsPopupItemSelect((1 << DATAITEMKIND_HEALTH) | (1 << DATAITEMKIND_TENSION) | (1 << DATAITEMKIND_MONSTERMOVE));
		return 0;
	}

	// もどるボタン押下確認
	this->btnBack.super.inactive = false;
	if(this->btnBack.super.trigger || global.key.bk.triggerInactive){
		this->btnBack.super.trigger = false;
		global.key.bk.triggerInactive = false;
		soundSePlayNG();
		// ページ遷移
		pageCartridgePush(pageTopInit());
		pageFieldStatusDispose(this->stat);
		return 1;
	}

	return 0;
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct pageCartridgeFieldMain *this){
	// 状態確認
	int status = 1 | ((storageStatusGet()->fieldKind & 0x7fff) << 1) | ((parameterPlayerLevel() & 0xff) << 16);
	struct dataField *dataField = dataFieldGet(storageStatusGet()->fieldKind, NULL);
	if(dataField == NULL){
		// ロード中
		status = -1;
	}
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// バッファ作成開始
		engineGraphicBufferBegin();

		// マスク
		e3dImageWhiteCreateArray(&this->fieldMask, 0, 0, global.screen.w, global.screen.h);
		// フィールド名作成
		e3dImageBoxDefaultCreateArray(&this->fieldWindow, (global.screen.w - (166)) * 0.5, 5, 10 * 15 + 8 * 2, 15 + 8 * 2);
		e3dImageTextCreateArray(&this->fieldName, global.screen.w * 0.5, 13, (dataField == NULL) ? "" : dataField->name, 1.0, 0, 1);
		// フィールドマップ 円柱
		e3dImageCreateArray(&this->fieldCircle[0], TEXSIZ_FIELD_W, TEXSIZ_FIELD_H, -40, 20, 80, -70, TEXPOS_FIELD_CIRCLE_X + (TEXPOS_FIELD_CIRCLE_W + TEXPOS_SPACE) * 0, TEXPOS_FIELD_CIRCLE_Y, TEXPOS_FIELD_CIRCLE_W, TEXPOS_FIELD_CIRCLE_H);
		e3dImageCreateArray(&this->fieldCircle[1], TEXSIZ_FIELD_W, TEXSIZ_FIELD_H, -40, 20, 80, -70, TEXPOS_FIELD_CIRCLE_X + (TEXPOS_FIELD_CIRCLE_W + TEXPOS_SPACE) * 1, TEXPOS_FIELD_CIRCLE_Y, TEXPOS_FIELD_CIRCLE_W, TEXPOS_FIELD_CIRCLE_H);
		e3dImageCreateArray(&this->fieldCircle[2], TEXSIZ_FIELD_W, TEXSIZ_FIELD_H, -40, 20, 80, -70, TEXPOS_FIELD_CIRCLE_X + (TEXPOS_FIELD_CIRCLE_W + TEXPOS_SPACE) * 2, TEXPOS_FIELD_CIRCLE_Y, TEXPOS_FIELD_CIRCLE_W, TEXPOS_FIELD_CIRCLE_H);
		e3dImageCreateArray(&this->fieldCircle[3], TEXSIZ_FIELD_W, TEXSIZ_FIELD_H, -40, 20, 80, -70, TEXPOS_FIELD_CIRCLE_X + (TEXPOS_FIELD_CIRCLE_W + TEXPOS_SPACE) * 3, TEXPOS_FIELD_CIRCLE_Y, TEXPOS_FIELD_CIRCLE_W, TEXPOS_FIELD_CIRCLE_H);
		// フィールドマップ 飾り
		e3dImageCreateArray(&this->fieldDecoration[0], TEXSIZ_FIELD_W, TEXSIZ_FIELD_H, -40, 60, 80, -80, TEXPOS_FIELD_DECORATION_X + (TEXPOS_FIELD_DECORATION_W + TEXPOS_SPACE) * 0, TEXPOS_FIELD_DECORATION_Y + (TEXPOS_FIELD_DECORATION_H + TEXPOS_SPACE) * 0, TEXPOS_FIELD_DECORATION_W, TEXPOS_FIELD_DECORATION_H);
		e3dImageCreateArray(&this->fieldDecoration[1], TEXSIZ_FIELD_W, TEXSIZ_FIELD_H, -40, 60, 80, -80, TEXPOS_FIELD_DECORATION_X + (TEXPOS_FIELD_DECORATION_W + TEXPOS_SPACE) * 1, TEXPOS_FIELD_DECORATION_Y + (TEXPOS_FIELD_DECORATION_H + TEXPOS_SPACE) * 0, TEXPOS_FIELD_DECORATION_W, TEXPOS_FIELD_DECORATION_H);
		e3dImageCreateArray(&this->fieldDecoration[2], TEXSIZ_FIELD_W, TEXSIZ_FIELD_H, -40, 60, 80, -80, TEXPOS_FIELD_DECORATION_X + (TEXPOS_FIELD_DECORATION_W + TEXPOS_SPACE) * 0, TEXPOS_FIELD_DECORATION_Y + (TEXPOS_FIELD_DECORATION_H + TEXPOS_SPACE) * 1, TEXPOS_FIELD_DECORATION_W, TEXPOS_FIELD_DECORATION_H);
		e3dImageCreateArray(&this->fieldDecoration[3], TEXSIZ_FIELD_W, TEXSIZ_FIELD_H, -40, 60, 80, -80, TEXPOS_FIELD_DECORATION_X + (TEXPOS_FIELD_DECORATION_W + TEXPOS_SPACE) * 1, TEXPOS_FIELD_DECORATION_Y + (TEXPOS_FIELD_DECORATION_H + TEXPOS_SPACE) * 1, TEXPOS_FIELD_DECORATION_W, TEXPOS_FIELD_DECORATION_H);
		e3dImageCreateArray(&this->fieldDecoration[4], TEXSIZ_FIELD_W, TEXSIZ_FIELD_H, -40, 60, 80, -80, TEXPOS_FIELD_DECORATION_X + (TEXPOS_FIELD_DECORATION_W + TEXPOS_SPACE) * 0, TEXPOS_FIELD_DECORATION_Y + (TEXPOS_FIELD_DECORATION_H + TEXPOS_SPACE) * 2, TEXPOS_FIELD_DECORATION_W, TEXPOS_FIELD_DECORATION_H);
		e3dImageCreateArray(&this->fieldDecoration[5], TEXSIZ_FIELD_W, TEXSIZ_FIELD_H, -40, 60, 80, -80, TEXPOS_FIELD_DECORATION_X + (TEXPOS_FIELD_DECORATION_W + TEXPOS_SPACE) * 1, TEXPOS_FIELD_DECORATION_Y + (TEXPOS_FIELD_DECORATION_H + TEXPOS_SPACE) * 2, TEXPOS_FIELD_DECORATION_W, TEXPOS_FIELD_DECORATION_H);
		// フィールドマップ 敵
		e3dImageCreateArray(&this->fieldEnemy, TEXSIZ_ENEMY_W, TEXSIZ_ENEMY_H, -40, 60, 80, -80, TEXPOS_ENEMY_FIELD_X, TEXPOS_ENEMY_FIELD_Y, TEXPOS_ENEMY_FIELD_W, TEXPOS_ENEMY_FIELD_H);
		e3dImageWhiteCreateArray(&this->fieldGauge, -25.0, -3.0, 50.0, 6.0);

		// プレイヤーウインドウ作成
		char data[32];
		e3dImageBoxDefaultCreateArray(&this->playerWindow, 5, global.screen.h - 5 - 50, 205, 50);
		e3dImageCreateArray(&this->playerImage, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, 8, global.screen.h - 5 - 50 + 3, 44, 44, TEXPOS_SYSTEM_IMAGEPLAYERICON_X, TEXPOS_SYSTEM_IMAGEPLAYERICON_Y + (TEXPOS_SYSTEM_IMAGEPLAYERICON_H + TEXPOS_SPACE) * 0, TEXPOS_SYSTEM_IMAGEPLAYERICON_W, TEXPOS_SYSTEM_IMAGEPLAYERICON_H);
		e3dImageWhiteCreateArray(&this->playerTension, 8 - 2 * 0.5, global.screen.h - 5 - 50 + 3 - 20 * 0.5, 2, 20);
		e3dImageWhiteCreateArray(&this->playerGauge, 55, global.screen.h - 5 - 50 + 20 + 2, 145, 6);
		sprintf(data, "コッペン Lv.%d", parameterPlayerLevel());
		e3dImageTextCreateArray(&this->playerName, 55, global.screen.h - 5 - 50 + 5, data, 1.0, 1, 1);
		int length = sprintf(data, "/%d", parameterPlayerHealthMax());
		e3dImageTextCreateArray(&this->playerHealthMax, 200, global.screen.h - 5 - 50 + 30, data, 1.0, -1, 1);
		e3dNumberDefaultCreateArray(&this->playerHealth, 200 - 10 * length, global.screen.h - 5 - 50 + 30, 1.0, -1, 1);

		// チュートリアル矢印作成
		e3dImageCreateArray(&this->tutorialArrow1, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, -40, -20, 40, 40, TEXPOS_SYSTEM_IMAGETUTORIALARROW_X, TEXPOS_SYSTEM_IMAGETUTORIALARROW_Y, TEXPOS_SYSTEM_IMAGETUTORIALARROW_W, TEXPOS_SYSTEM_IMAGETUTORIALARROW_H);
		e3dImageCreateArray(&this->tutorialArrow2, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, global.screen.w - 5 - 100 + 20 - 40, global.screen.h - 105 + (30 - 40) * 0.5, 40, 40, TEXPOS_SYSTEM_IMAGETUTORIALARROW_X, TEXPOS_SYSTEM_IMAGETUTORIALARROW_Y, TEXPOS_SYSTEM_IMAGETUTORIALARROW_W, TEXPOS_SYSTEM_IMAGETUTORIALARROW_H);

		// ボタン作成
		partsButtonBoxCreateArray(&this->btnCheck, global.screen.w - 100 - 5, global.screen.h - (30 + 5) * 3, 100, 30, "しらべる", 1.0);
		partsButtonBoxCreateArray(&this->btnItem, global.screen.w - 100 - 5, global.screen.h - (30 + 5) * 2, 100, 30, "アイテム", 1.0);
		partsButtonBoxCreateArray(&this->btnBack, global.screen.w - 100 - 5, global.screen.h - (30 + 5) * 1, 100, 30, "もどる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct pageCartridgeFieldMain *this){
	// 描画準備
	createBuffer(this);
	struct engineMathMatrix44 tempmat;
	int e3dIdFieldImage;
	struct dataField *dataField = dataFieldGet(storageStatusGet()->fieldKind, &e3dIdFieldImage);
	struct storageFieldList *storageField = &storageFieldGet()->list[storageStatusGet()->fieldKind];
	double toggleFieldNum = this->toggleField.step / 30.0;
	double toggleEnemyNum = this->toggleEnemy.step / 30.0;

	// 射影行列作成
	engineMathMat4Ortho(&e3dMatOrtho, -global.screen.offset.x, global.screen.w + global.screen.offset.x, global.screen.h + global.screen.offset.y, -global.screen.offset.y, -1, 1);
	// ワールド行列作成
	engineMathMat4Copy(&this->worldMat, &e3dMatOrtho);
	engineMathMat4Translate(&this->worldMat, global.screen.w * 0.5, global.screen.h * 0.5, 0);
	engineMathMat4Scale(&this->worldMat, 1.0, -1.0, 0.001);
	if(this->toggleField.step != 0){engineMathMat4Translate(&this->worldMat, -200 * toggleFieldNum * toggleFieldNum * toggleFieldNum, 0, 0);}
	if(this->toggleEnemy.step > 0){double scale = 1 + 2 * toggleEnemyNum * toggleEnemyNum; engineMathMat4Scale(&this->worldMat, scale, scale, 1.0);}
	engineMathMat4Translate(&this->worldMat, 0, 0, -this->dist);
	engineMathMat4RotateX(&this->worldMat, this->roth);
	engineMathMat4RotateY(&this->worldMat, this->rotv);
	engineMathMat4Translate(&this->worldMat, -this->sx, -this->sz, -this->sy);

	engineGraphicEngineClearAll();
	engineGraphicStencilClear();
	engineGraphicStencilSetMode(ENGINEGRAPHICSTENCILMODE_NONE);

	// 背景描画
	dataBackgroundDraw((dataField == NULL) ? -1 : dataField->backgroundKindField);

	// 地形描画
	if(this->mapInit && dataField != NULL){
		engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_NORMAL);
		engineGraphicEngineBindVertVBO(this->e3dIdVert);
		engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
		engineGraphicEngineBindFaceIBO(this->e3dIdFace);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
		// 地形描画
		while(this->rotv < -3.1415){this->rotv += 3.1415 * 2;}
		while(this->rotv >  3.1415){this->rotv -= 3.1415 * 2;}
		bool iOrder = (3.1415 * -0.16 < this->rotv && this->rotv < 3.1415 * 0.83);
		bool jOrder = (3.1415 * -0.50 < this->rotv && this->rotv < 3.1415 * 0.50);
		engineGraphicEngineBindTexture(e3dIdFieldImage);
		for(int i0 = 0; i0 < dataField->xsize; i0++){
			for(int j0 = 0; j0 < dataField->ysize; j0++){
				int i1 = iOrder ? i0 : (dataField->xsize - 1 - i0);
				int j1 = jOrder ? (dataField->ysize - 1 - j0) : j0;
				int index = dataField->xsize * j1 + i1;
				if(dataField->map[index].height <= 0){continue;}
				int step = this->mapData[index].step;
				double x = (i1 + j1 * 0.5) * (80 + 5);
				double y = j1 * (80 + 5) * 0.86602540378;
				double jump = (step < -30 || step == 0) ? 0 : 30 * engineMathAbs(engineMathSin(3.1415 * 2 * step / 60));
				if(this->mapData[index].icon >= 0){
					// アイコン描画
					engineMathMat4Copy(&tempmat, &this->worldMat);
					engineMathMat4Translate(&tempmat, x, (dataField->map[index].height - 1) * 35.0 + jump + 0.01, y);
					engineMathMat4RotateY(&tempmat, -this->rotv);
					engineMathMat4RotateX(&tempmat, -this->roth);
					engineGraphicEngineSetMatrix(&tempmat);
					e3dImageDraw(&this->fieldDecoration[this->mapData[index].icon]);
				}
				for(int k = dataField->map[index].height - 1; k >= 0; k--){
					// 円柱描画 高さの分だけ重ねる
					engineMathMat4Copy(&tempmat, &this->worldMat);
					engineMathMat4Translate(&tempmat, x, k * 35.0 + jump, y);
					engineMathMat4RotateY(&tempmat, -this->rotv);
					engineMathMat4RotateX(&tempmat, -this->roth);
					engineGraphicEngineSetMatrix(&tempmat);
					e3dImageDraw(&this->fieldCircle[this->mapData[index].mapchip]);
				}
			}
		}
		// エネミー描画
		for(int i = 0; i < STORAGEFIELD0001_ENEMYMAX; i++){
			int enemyKind = storageField->enemy[i].enemyKind;
			if(enemyKind < 0){continue;}
			int e3dIdEnemyImage;
			struct dataEnemy *dataEnemy = dataEnemyGet(enemyKind, &e3dIdEnemyImage);
			if(dataEnemy != NULL){
				int fieldx = storageField->enemy[i].fieldx;
				int fieldy = storageField->enemy[i].fieldy;
				int index = dataField->xsize * fieldy + fieldx;
				int step = this->mapData[index].step;
				double x = (fieldx + fieldy * 0.5) * (80 + 5);
				double y = fieldy * (80 + 5) * 0.86602540378;
				double jump = (step < -30 || step == 0) ? 0 : 30 * engineMathAbs(engineMathSin(3.1415 * 2 * step / 60));
				engineMathMat4Copy(&tempmat, &this->worldMat);
				engineMathMat4Translate(&tempmat, x, (dataField->map[index].height - 1) * 35.0 + jump * 1.5 + 0.02, y);
				engineMathMat4RotateY(&tempmat, -this->rotv);
				engineMathMat4RotateX(&tempmat, -this->roth);
				if(this->enemyData[i].step < 0){
					double s = engineMathSin(3.1415 * this->enemyData[i].step * 0.05);
					engineMathMat4Scale(&tempmat, 1 + 0.5 * s, 1 - 0.5 * s, 1);
				}
				engineGraphicEngineBindTexture(e3dIdEnemyImage);
				engineGraphicEngineSetMatrix(&tempmat);
				engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
				e3dImageDraw(&this->fieldEnemy);
				if(this->enemyData[i].step >= 0 && engineGraphicTextureIsComplete(e3dIdEnemyImage)){
					// エネミー体力ゲージ描画
					engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
					engineMathMat4Translate(&tempmat, 0.0, 60.0, 0.1);
					engineGraphicEngineSetMatrix(&tempmat);
					engineGraphicEngineSetColorRgba(0.5, 0.0, 0.0, 1.0);
					e3dImageDraw(&this->fieldGauge);
					engineMathMat4Translate(&tempmat, 25, 0, 0.1);
					engineMathMat4Scale(&tempmat, (double)storageField->enemy[i].healthPercent / 100, 1, 1);
					engineMathMat4Translate(&tempmat, -25, 0, 0);
					engineGraphicEngineSetMatrix(&tempmat);
					engineGraphicEngineSetColorRgba(1.0, 1.0, 0.0, 1.0);
					e3dImageDraw(&this->fieldGauge);
				}
			}
		}

		// チュートリアル矢印描画
		if(this->stepTutorial11 > 0){
			int index = 9;
			int fieldx = index % dataField->xsize;
			int fieldy = engineMathFloor(index / dataField->xsize);
			int step = this->mapData[index].step;
			double x = (fieldx + fieldy * 0.5) * (80 + 5);
			double y = fieldy * (80 + 5) * 0.86602540378;
			double jump = (step < -30 || step == 0) ? 0 : 30 * engineMathAbs(engineMathSin(3.1415 * 2 * step / 60));
			engineMathMat4Copy(&tempmat, &this->worldMat);
			engineMathMat4Translate(&tempmat, x, (dataField->map[index].height - 1) * 35.0 + jump * 1.5 + 0.04, y);
			engineMathMat4RotateY(&tempmat, -this->rotv);
			engineMathMat4RotateX(&tempmat, -this->roth);
			engineMathMat4Translate(&tempmat, -engineMathAbs(10 * engineMathSin(3.1415 * this->stepTutorial11 * 0.05)) - 10, 30, 0);
			engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
			engineGraphicEngineSetMatrix(&tempmat);
			engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
			e3dImageDraw(&this->tutorialArrow1);
		}
	}

	// 2D描画準備
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);
	engineGraphicEngineSetMatrix(&e3dMatOrtho);

	if(this->toggleField.step != 0){
		// フィールド遷移マスク
		engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, engineMathAbs(toggleFieldNum));
		e3dImageDraw(&this->fieldMask);
	}

	if(this->toggleEnemy.step > 0){
		// エネミー遷移マスク
		engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, toggleEnemyNum);
		e3dImageDraw(&this->fieldMask);
	}

	// プレイヤーウインドウ描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
	e3dImageDraw(&this->playerWindow);
	e3dImageDraw(&this->playerImage);
	engineGraphicEngineSetColorRgba(0.5, 0.0, 0.0, 1.0);
	e3dImageDraw(&this->playerGauge);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, 1.0);
	e3dImageDraw(&this->playerName);
	e3dImageDraw(&this->playerHealthMax);
	e3dNumberDraw(&this->playerHealth, &e3dMatOrtho, storageStatusGet()->health);
	if(storageStatusGet()->isTension){
		// プレイヤーテンション描画
		engineGraphicEngineSetColorRgba(0.0, 1.0, 1.0, 1.0);
		for(int i = 0; i < PAGEFIELDMAIN_TENSIONNUM; i++){
			engineMathMat4Copy(&tempmat, &e3dMatOrtho);
			engineMathMat4Translate(&tempmat, this->playerData.tensionEffect[i].x, this->playerData.tensionEffect[i].y, 0);
			engineGraphicEngineSetMatrix(&tempmat);
			e3dImageDraw(&this->playerTension);
		}
	}
	// ゲージ描画
	int health = storageStatusGet()->health;
	int healthMax = parameterPlayerHealthMax();
	int healthPrev = this->playerData.healthPrev;
	engineMathMat4Copy(&tempmat, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat, 55, 0, 0);
	engineMathMat4Scale(&tempmat, (double)health / healthMax, 1, 1);
	engineMathMat4Translate(&tempmat, -55, 0, 0);
	engineGraphicEngineSetMatrix(&tempmat);
	engineGraphicEngineSetColorRgba(1.0, 0.0, 0.0, 1.0);
	e3dImageDraw(&this->playerGauge);
	engineMathMat4Copy(&tempmat, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat, 55, 0, 0);
	engineMathMat4Scale(&tempmat, (double)healthPrev / healthMax, 1, 1);
	engineMathMat4Translate(&tempmat, -55, 0, 0);
	engineGraphicEngineSetMatrix(&tempmat);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 0.0, 1.0);
	e3dImageDraw(&this->playerGauge);

	// フィールド名描画
	engineMathMat4Copy(&tempmat, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat, -100 * toggleFieldNum * toggleFieldNum * toggleFieldNum, 0, 0);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1 - engineMathAbs(toggleFieldNum));
	engineGraphicEngineSetMatrix(&tempmat);
	e3dImageDraw(&this->fieldWindow);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, 1 - engineMathAbs(toggleFieldNum));
	e3dImageDraw(&this->fieldName);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);

	// ボタン描画
	partsButtonBoxDraw(&this->btnCheck, &e3dMatOrtho, 1.0);
	partsButtonBoxDraw(&this->btnItem, &e3dMatOrtho, 1.0);
	partsButtonBoxDraw(&this->btnBack, &e3dMatOrtho, 1.0);

	// チュートリアル矢印描画
	if(this->stepTutorial12 > 0){
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, -engineMathAbs(10 * engineMathSin(3.1415 * this->stepTutorial12 * 0.05)), 0, 0);
		engineGraphicEngineSetMatrix(&tempmat);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
		e3dImageDraw(&this->tutorialArrow2);
	}

	// ポップアップ描画
	if(this->popupOpen != NULL){this->popupOpen->draw(this->popupOpen);}
	if(this->popupInfo != NULL){this->popupInfo->draw(this->popupInfo);}
	if(this->popupItemGet != NULL){this->popupItemGet->draw(this->popupItemGet);}
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
static void pause(struct pageCartridgeFieldMain *this){
}

// 破棄
static void dispose(struct pageCartridgeFieldMain *this){
	if(this->popupOpen != NULL){this->popupOpen->dispose(this->popupOpen);}
	if(this->popupInfo != NULL){this->popupInfo->dispose(this->popupInfo);}
	if(this->popupItemGet != NULL){this->popupItemGet->dispose(this->popupItemGet);}
	if(this->popupItemSelect != NULL){this->popupItemSelect->dispose(this->popupItemSelect);}
	if(this->popupItemUse1 != NULL){this->popupItemUse1->dispose(this->popupItemUse1);}
	if(this->popupItemUse2 != NULL){this->popupItemUse2->dispose(this->popupItemUse2);}
	if(this->popupTutorial != NULL){this->popupTutorial->dispose(this->popupTutorial);}
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページカートリッジ作成
struct pageCartridge *pageFieldMain(struct pageFieldStatus *stat){
	struct pageCartridgeFieldMain *this = (struct pageCartridgeFieldMain*)engineUtilMemoryCalloc(1, sizeof(struct pageCartridgeFieldMain));
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

