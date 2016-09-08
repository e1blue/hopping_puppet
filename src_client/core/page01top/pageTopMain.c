#include "../library.h"
#include "../includeAll.h"
#include "pageTop.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#define PAGETOPMAIN_MAPSIZEX 8
#define PAGETOPMAIN_MAPSIZEY 8
#define PAGETOPMAIN_MAPSIZEZ 5
#define PAGETOPMAIN_PUPPETNUM 16

enum pageTopMainPuppetActionType{
	PAGETOPMAINPUPPETACTIONTYPE_NONE = 0,
	PAGETOPMAINPUPPETACTIONTYPE_WALK,
	PAGETOPMAINPUPPETACTIONTYPE_JUMP
};

// ページカートリッジ構造体
struct pageCartridgeTopMain{
	struct pageCartridge super;
	struct pageTopStatus *stat;
	struct popupCartridge *popupOption;
	struct popupCartridge *popupManual;
	struct popupCartridge *popupCredit;
	struct popupCartridge *popupTutorial;
	int stepTutorial01;
	int stepTutorial02;
	int stepTutorial11;
	int stepTutorial12;

	struct hknwMap *map;
	int obstacleMap[PAGETOPMAIN_MAPSIZEY][PAGETOPMAIN_MAPSIZEX];

	struct partsButtonBox btn1;
	struct partsButtonBox btn2;
	struct partsButtonBox btn3;

	struct e3dImage title;
	struct e3dImage countWindow;
	struct e3dImage countLabel;
	struct e3dNumber countNumPuppet;
	struct e3dNumber countNumMoney;
	struct e3dImage tutorialArrow1;
	struct e3dImage tutorialArrow2;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	int puppetNum;
	struct{
		int puppetKind;
		enum pageTopMainPuppetActionType type;
		double x;
		double y;
		double vx;
		double vy;
		double r;
		double rNext;
		int step;
	} puppetList[PAGETOPMAIN_PUPPETNUM];

	// ワールド行列
	struct engineMathMatrix44 worldMat;
	// 画面回転
	double rotv;
	double roth;
	double dist;
	double rotvNext;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageCartridgeTopMain *this){
	// マップチップ作成
	int mapchip[PAGETOPMAIN_MAPSIZEZ][PAGETOPMAIN_MAPSIZEY][PAGETOPMAIN_MAPSIZEX] = {{
		{ 4,  4,  4,  2,  2,  2,  4,  4},
		{ 4,  4,  2,  2,  2,  4,  4,  4},
		{ 4,  4,  2,  2,  2,  4,  4,  4},
		{ 4,  4,  2,  2,  2,  4,  4,  4},
		{ 4,  4,  2,  2,  2,  4,  4,  4},
		{ 4,  4,  4,  2,  2,  2,  4,  4},
		{ 4,  4,  4,  2,  2,  2,  4,  4},
		{ 4,  4,  4,  4,  2,  2,  2,  4}
	},{
		{10, 10,  0,  0,  0,  0,  0,  0},
		{10,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0, 13,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0}
	},{
		{10,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0, 14,  0},
		{ 0,  0,  0,  0,  0, 14, 13, 14},
		{ 0,  0,  0,  0,  0,  0, 14,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0}
	},{
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0, 14,  0},
		{ 0,  0,  0,  0,  0, 14, 13, 14},
		{ 0,  0,  0,  0,  0,  0, 14,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0}
	},{
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0, 14,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0}
	}};
	this->map = hknwMapInit(PAGETOPMAIN_MAPSIZEX, PAGETOPMAIN_MAPSIZEY, PAGETOPMAIN_MAPSIZEZ, (int*)mapchip);
	// マップチップの障害物位置記憶
	int obstacleMap[PAGETOPMAIN_MAPSIZEY][PAGETOPMAIN_MAPSIZEX] = {
		{ 1,  1,  0,  0,  0,  0,  0,  0},
		{ 1,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  1,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0,  0,  0,  0}
	};
	memcpy(this->obstacleMap, obstacleMap, sizeof(this->obstacleMap));

	// ボタン
	partsButtonBoxInit(&this->btn1);
	partsButtonBoxInit(&this->btn2);
	partsButtonBoxInit(&this->btn3);

	// パペット数計算
	this->puppetNum = parameterPlayerPuppetNum();
	this->btn2.super.inactive = (this->puppetNum <= 0);

	// パペット初期化
	int index = 0;
	int puppetNum[STORAGEPUPPET0001_KIND];
	for(int i = 0; i < STORAGEPUPPET0001_KIND; i++){puppetNum[i] = storagePuppetGet()->list[i].num;}
	for(int i = 0; i < 256; i++){
		// パペット位置選出
		int x = engineMathRandomGet() % PAGETOPMAIN_MAPSIZEX;
		int y = engineMathRandomGet() % PAGETOPMAIN_MAPSIZEY;
		if(obstacleMap[y][x] != 0){continue;}
		// パペット抽選
		int puppetKind = -1;
		int totalFrequency = 0;
		for(int j = 0; j < STORAGEPUPPET0001_KIND; j++){if(puppetNum[j] > 0){totalFrequency += puppetNum[j];}}
		if(totalFrequency <= 0){break;}
		int randomValue = engineMathRandomGet() % totalFrequency;
		for(int j = 0; j < STORAGEPUPPET0001_KIND; j++){
			if(randomValue < puppetNum[j]){
				puppetKind = j;
				break;
			}else{randomValue -= puppetNum[j];}
		}
		if(puppetKind < 0){continue;}
		// パペット作成
		obstacleMap[y][x] = -1;
		this->puppetList[index].puppetKind = puppetKind;
		this->puppetList[index].type = PAGETOPMAINPUPPETACTIONTYPE_WALK;
		this->puppetList[index].x = 0.5 + x;
		this->puppetList[index].y = 0.5 + y;
		this->puppetList[index].vx = 0;
		this->puppetList[index].vy = 0;
		this->puppetList[index].r = 3.1415 / 180 * (engineMathRandomGet() % 320);
		this->puppetList[index].rNext = 0;
		puppetNum[puppetKind]--;
		if(++index >= PAGETOPMAIN_PUPPETNUM){break;}
	}

	// 画面回転
	this->rotvNext = 3.1415 / 180 *  0;
	this->rotv = this->rotvNext + 3.1415 / 180 * 30;
	this->roth = 3.1415 / 180 * 30;
	this->dist = 10;
}

// ----------------------------------------------------------------

// 画面回転の計算
static void calcRotate(struct pageCartridgeTopMain *this){
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
			if(!t->mv){
				// タッチ処理
				int touchIndex = -1;
				double touchDepth = 1;
				for(int i = 0; i < PAGETOPMAIN_PUPPETNUM; i++){
					if(this->puppetList[i].type != PAGETOPMAINPUPPETACTIONTYPE_WALK){continue;}
					// パペット中心の画面座標上位置を計算
					struct engineMathMatrix44 tempmat;
					engineMathMat4Copy(&tempmat, &this->worldMat);
					engineMathMat4Translate(&tempmat, this->puppetList[i].x, 1.0, this->puppetList[i].y);
					double x0 = (1 + tempmat.m30 / tempmat.m33) * 0.5 * global.window.w;
					double y0 = (1 - tempmat.m31 / tempmat.m33) * 0.5 * global.window.h;
					double z0 = tempmat.m32 / tempmat.m33;
					// パペット半径を求める計算
					engineMathMat4Translate(&tempmat, 0, 0.8, 0);
					double r = y0 - (1 - tempmat.m31 / tempmat.m33) * 0.5 * global.window.h;
					double rr = r * r;
					// タッチ位置と比較
					double x = t->window.x - x0;
					double y = t->window.y - y0;
					double xxyy = x * x + y * y;
					if(xxyy < rr && touchDepth > z0){
						touchIndex = i;
						touchDepth = z0;
					}
				}

				if(touchIndex >= 0){
					// ジャンプ開始
					soundSePlayJump();
					this->puppetList[touchIndex].type = PAGETOPMAINPUPPETACTIONTYPE_JUMP;
					this->puppetList[touchIndex].step = 0;
				}
			}
			// タッチ終了時
			engineCtrlTouchFree();
		}
	}
}

// 減衰計算
static void calcDamper(struct pageCartridgeTopMain *this){
	// 画面回転角度の減衰計算
	double drv = this->rotv - this->rotvNext;
	while(drv < -3.1415){drv += 3.1415 * 2;}
	while(drv >  3.1415){drv -= 3.1415 * 2;}
	if(engineMathAbs(drv) > 0.01){this->rotv -= drv * 0.2;}
}

// 計算
static int calc(struct pageCartridgeTopMain *this){
	// メモリートレースボタン計算
	partsButtonMemoryCalc();
	
	calcDamper(this);

	// チュートリアルステップカウント
	if(!storageStatusGet()->tutorial.atelier05 && storageStatusGet()->tutorial.top01){this->stepTutorial11++;}else{this->stepTutorial11 = 0;}
	if(!storageStatusGet()->tutorial.jump03 && storageStatusGet()->tutorial.top02 && this->puppetNum > 0){this->stepTutorial12++;}else{this->stepTutorial12 = 0;}

	if(this->popupOption != NULL){
		// オプションポップアップ計算
		if(!this->popupOption->calc(this->popupOption)){return 0;}
		// オプションポップアップ完了時処理
		enum popupTopOptionResult result = this->popupOption->result(this->popupOption);
		this->popupOption->dispose(this->popupOption);
		this->popupOption = NULL;
		// ポップアップ展開
		switch(result){
			case POPUPTOPOPTIONRESULT_NONE: break;
			case POPUPTOPOPTIONRESULT_MANUAL: this->popupManual = popupTopManual(this->stat); return 0;
			case POPUPTOPOPTIONRESULT_CREDIT: this->popupCredit = popupTopCredit(this->stat); return 0;
		}
	}

	if(this->popupManual != NULL){
		// マニュアルポップアップ計算
		if(!this->popupManual->calc(this->popupManual)){return 0;}
		// マニュアルポップアップ完了時処理
		if(this->popupManual->result(this->popupManual)){}
		this->popupManual->dispose(this->popupManual);
		this->popupManual = NULL;
	}

	if(this->popupCredit != NULL){
		// クレジットポップアップ計算
		if(!this->popupCredit->calc(this->popupCredit)){return 0;}
		// クレジットポップアップ完了時処理
		if(this->popupCredit->result(this->popupCredit)){}
		this->popupCredit->dispose(this->popupCredit);
		this->popupCredit = NULL;
	}

	if(this->popupTutorial != NULL){
		// チュートリアルポップアップ計算
		if(!this->popupTutorial->calc(this->popupTutorial)){return 0;}
		// チュートリアルポップアップ完了時処理
		if(this->popupTutorial->result(this->popupTutorial)){}
		this->popupTutorial->dispose(this->popupTutorial);
		this->popupTutorial = NULL;
	}else if(!storageStatusGet()->tutorial.top01){
		// チュートリアル計算
		if(this->stepTutorial01 == 60){soundSePlayOK(); this->popupTutorial = partsPopupTutorialTop01A();}
		if(this->stepTutorial01 == 120){soundSePlayOK(); this->popupTutorial = partsPopupTutorialTop01B();}
		if(this->stepTutorial01 == 120){storageStatusGet()->tutorial.top01 = true; storageStatusUpdate();}
		this->stepTutorial01++;
		return 0;
	}else if(!storageStatusGet()->tutorial.top02 && storageStatusGet()->tutorial.atelier05){
		// チュートリアル計算
		if(this->stepTutorial02 == 60){soundSePlayOK(); this->popupTutorial = partsPopupTutorialTop02();}
		if(this->stepTutorial02 == 60){storageStatusGet()->tutorial.top02 = true; storageStatusUpdate();}
		this->stepTutorial02++;
		return 0;
	}

	// タッチ処理
	partsButtonBoxCalc(&this->btn1);
	partsButtonBoxCalc(&this->btn2);
	partsButtonBoxCalc(&this->btn3);
	calcRotate(this);

	// パペット相互作用計算
	double timeScale = 0.001;
	double rmax0 = 1.5;
	double rmax1 = 2.0;
	double rmax2 = 0.5;
	double maxspeed = 0.030;
	double minspeed = 0.005;
	for(int i = 0; i < PAGETOPMAIN_PUPPETNUM; i++){
		if(this->puppetList[i].type == PAGETOPMAINPUPPETACTIONTYPE_NONE){continue;}
		// ランダム速度
		if(this->puppetList[i].type == PAGETOPMAINPUPPETACTIONTYPE_WALK && engineMathRandomGet() % 160 == 0){
			double r = 10 * (double)(engineMathRandomGet() % 100) / 100;
			double t = 3.1415 * 2 * (double)(engineMathRandomGet() % 100) / 100;
			this->puppetList[i].vx += r * engineMathCos(t);
			this->puppetList[i].vy += r * engineMathSin(t);
		}
		// パペット間相互作用計算
		for(int j = 0; j < PAGETOPMAIN_PUPPETNUM; j++){
			if(i == j){continue;}
			if(this->puppetList[j].type == PAGETOPMAINPUPPETACTIONTYPE_NONE){continue;}
			double dx = this->puppetList[i].x - this->puppetList[j].x;
			double dy = this->puppetList[i].y - this->puppetList[j].y;
			double rr = dx * dx + dy * dy;
			double irr = (engineMathAbs(rr) < DBL_EPSILON) ? 0 : (rmax0 * rmax0 / rr);
			double irrrrrr = irr * irr * irr;
			double lj = (engineMathAbs(rr) < DBL_EPSILON || irrrrrr < 1) ? 0 : ((irrrrrr * irrrrrr - irrrrrr) / rr);
			double dvx = timeScale * lj * dx;
			double dvy = timeScale * lj * dy;
			if(this->puppetList[i].type == PAGETOPMAINPUPPETACTIONTYPE_WALK){
				this->puppetList[i].vx += dvx;
				this->puppetList[i].vy += dvy;
			}
			if(this->puppetList[j].type == PAGETOPMAINPUPPETACTIONTYPE_WALK){
				this->puppetList[j].vx -= dvx;
				this->puppetList[j].vy -= dvy;
			}
		}
		// 障害物との相互作用計算
		for(int j = 0; j < PAGETOPMAIN_MAPSIZEY; j++){
			for(int k = 0; k < PAGETOPMAIN_MAPSIZEX; k++){
				if(this->obstacleMap[j][k] > 0){
					double dx = this->puppetList[i].x - k;
					double dy = this->puppetList[i].y - j;
					double rr = dx * dx + dy * dy;
					double irr = (engineMathAbs(rr) < DBL_EPSILON) ? 0 : (rmax1 * rmax1 / rr);
					double irrrrrr = irr * irr * irr;
					double lj = (engineMathAbs(rr) < DBL_EPSILON || irrrrrr < 1) ? 0 : ((irrrrrr * irrrrrr - irrrrrr) / rr);
					double dvx = timeScale * lj * dx;
					double dvy = timeScale * lj * dy;
					this->puppetList[i].vx += dvx;
					this->puppetList[i].vy += dvy;
				}
			}
		}
		// 壁との相互作用計算
		double dx0 = this->puppetList[i].x - 0.0;
		double dx1 = this->puppetList[i].x - 8.0;
		double dy0 = this->puppetList[i].y - 0.0;
		double dy1 = this->puppetList[i].y - 8.0;
		double irrx0 = (engineMathAbs(dx0) < DBL_EPSILON) ? 0 : (rmax2 * rmax2 / (dx0 * dx0));
		double irrx1 = (engineMathAbs(dx1) < DBL_EPSILON) ? 0 : (rmax2 * rmax2 / (dx1 * dx1));
		double irry0 = (engineMathAbs(dy0) < DBL_EPSILON) ? 0 : (rmax2 * rmax2 / (dy0 * dy0));
		double irry1 = (engineMathAbs(dy1) < DBL_EPSILON) ? 0 : (rmax2 * rmax2 / (dy1 * dy1));
		double irrrrrrx0 = irrx0 * irrx0 * irrx0;
		double irrrrrrx1 = irrx1 * irrx1 * irrx1;
		double irrrrrry0 = irry0 * irry0 * irry0;
		double irrrrrry1 = irry1 * irry1 * irry1;
		double dvx0 = (dx0 <= 0) ?  maxspeed : (irrrrrrx0 < 1) ? 0 : (timeScale * ((irrrrrrx0 * irrrrrrx0 - irrrrrrx0) / (dx0 * dx0)) * dx0);
		double dvx1 = (dx1 >= 0) ? -maxspeed : (irrrrrrx1 < 1) ? 0 : (timeScale * ((irrrrrrx1 * irrrrrrx1 - irrrrrrx1) / (dx1 * dx1)) * dx1);
		double dvy0 = (dy0 <= 0) ?  maxspeed : (irrrrrry0 < 1) ? 0 : (timeScale * ((irrrrrry0 * irrrrrry0 - irrrrrry0) / (dy0 * dy0)) * dy0);
		double dvy1 = (dy1 >= 0) ? -maxspeed : (irrrrrry1 < 1) ? 0 : (timeScale * ((irrrrrry1 * irrrrrry1 - irrrrrry1) / (dy1 * dy1)) * dy1);
		this->puppetList[i].vx += dvx0;
		this->puppetList[i].vx += dvx1;
		this->puppetList[i].vy += dvy0;
		this->puppetList[i].vy += dvy1;
	}
	// パペット位置計算
	for(int i = 0; i < PAGETOPMAIN_PUPPETNUM; i++){
		if(this->puppetList[i].type == PAGETOPMAINPUPPETACTIONTYPE_NONE){continue;}
		if(this->puppetList[i].type == PAGETOPMAINPUPPETACTIONTYPE_JUMP){
			if(this->puppetList[i].step >= 30){
				// ジャンプ完了
				this->puppetList[i].type = PAGETOPMAINPUPPETACTIONTYPE_WALK;
				this->puppetList[i].step = 0;
			}
		}

		if(this->puppetList[i].vx > maxspeed){this->puppetList[i].vx = maxspeed;}else if(this->puppetList[i].vx < -maxspeed){this->puppetList[i].vx = -maxspeed;}
		if(this->puppetList[i].vy > maxspeed){this->puppetList[i].vy = maxspeed;}else if(this->puppetList[i].vy < -maxspeed){this->puppetList[i].vy = -maxspeed;}
		this->puppetList[i].vx *= 0.99;
		this->puppetList[i].vy *= 0.99;
		if(this->puppetList[i].vx * this->puppetList[i].vx + this->puppetList[i].vy * this->puppetList[i].vy < minspeed * minspeed){this->puppetList[i].vx = this->puppetList[i].vy = 0;}
		else{this->puppetList[i].rNext = engineMathAtan2(this->puppetList[i].vy, this->puppetList[i].vx);}

		// 回転角度の減衰計算
		double dr = this->puppetList[i].r - this->puppetList[i].rNext;
		while(dr < -3.1415){dr += 3.1415 * 2;}
		while(dr >  3.1415){dr -= 3.1415 * 2;}
		if(engineMathAbs(dr) > 0.01){this->puppetList[i].r -= dr * 0.1;}
		// 位置計算
		this->puppetList[i].x += this->puppetList[i].vx;
		this->puppetList[i].y += this->puppetList[i].vy;
		this->puppetList[i].step++;
	}

	// アトリエボタン押下確認
	if(this->btn1.super.trigger){
		this->btn1.super.trigger = false;
		soundSePlayOK();
		// ページ遷移
		pageCartridgePush(pageAtelierInit());
		pageTopStatusDispose(this->stat);
		return 1;
	}

	// 討伐ボタン押下確認
	if(this->btn2.super.trigger){
		this->btn2.super.trigger = false;
		soundSePlayOK();
		// ページ遷移
		pageCartridgePush(pageFieldInit());
		pageTopStatusDispose(this->stat);
		return 1;
	}

	// オプションボタン押下確認
	if(this->btn3.super.trigger){
		this->btn3.super.trigger = false;
		soundSePlayOK();
		// ポップアップ展開
		this->popupOption = popupTopOption(this->stat);
		return 0;
	}

	// 戻るボタン押下確認
	if(global.key.bk.triggerInactive){
		global.key.bk.triggerInactive = false;
		soundSePlayNG();
		// 終了
		platformMainSurfaceExit();
	}

	return 0;
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct pageCartridgeTopMain *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// バッファ作成開始
		engineGraphicBufferBegin();

		// タイトル
		e3dImageCreateArray(&this->title, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, 10, 10, 120, 100, TEXPOS_SYSTEM_IMAGETITLE_X, TEXPOS_SYSTEM_IMAGETITLE_Y, TEXPOS_SYSTEM_IMAGETITLE_W, TEXPOS_SYSTEM_IMAGETITLE_H);
		// カウントウインドウ作成
		e3dImageBoxDefaultCreateArray(&this->countWindow, 5, global.screen.h - 5 - 46, 140 + 16, 30 + 16);
		e3dImageTextCreateArray(&this->countLabel, 5 + 3 + 5, global.screen.h - 5 - 8 - 30, "パペット        たい\nおかね         ゲソ", 1.0, 1, 1);
		e3dNumberDefaultCreateArray(&this->countNumPuppet, 5 + 8 + 120, global.screen.h - 5 - 8 - 30, 1.0, -1, 1);
		e3dNumberDefaultCreateArray(&this->countNumMoney, 5 + 8 + 120, global.screen.h - 5 - 8 - 15, 1.0, -1, 1);
		// チュートリアル矢印作成
		e3dImageCreateArray(&this->tutorialArrow1, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, global.screen.w - 5 - 100 + 20 - 40, global.screen.h - 105 + (30 - 40) * 0.5, 40, 40, TEXPOS_SYSTEM_IMAGETUTORIALARROW_X, TEXPOS_SYSTEM_IMAGETUTORIALARROW_Y, TEXPOS_SYSTEM_IMAGETUTORIALARROW_W, TEXPOS_SYSTEM_IMAGETUTORIALARROW_H);
		e3dImageCreateArray(&this->tutorialArrow2, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, global.screen.w - 5 - 100 + 20 - 40, global.screen.h -  70 + (30 - 40) * 0.5, 40, 40, TEXPOS_SYSTEM_IMAGETUTORIALARROW_X, TEXPOS_SYSTEM_IMAGETUTORIALARROW_Y, TEXPOS_SYSTEM_IMAGETUTORIALARROW_W, TEXPOS_SYSTEM_IMAGETUTORIALARROW_H);

		// ボタン作成
		partsButtonBoxCreateArray(&this->btn1, global.screen.w - 5 - 100, global.screen.h - 105, 100, 30, "アトリエ", 1.0);
		partsButtonBoxCreateArray(&this->btn2, global.screen.w - 5 - 100, global.screen.h -  70, 100, 30, "とうばつ", 1.0);
		partsButtonBoxCreateArray(&this->btn3, global.screen.w - 5 - 100, global.screen.h -  35, 100, 30, "オプション", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct pageCartridgeTopMain *this){
	// 描画準備
	createBuffer(this);

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
	engineMathMat4Translate(&this->worldMat, -4.0, -1.0, -4.0);

	engineGraphicEngineClearAll();

	// 背景描画
	dataBackgroundDraw(0);

	// ハコニワ描画
	engineGraphicEngineBindTexture(e3dIdSystemImageNearest);
	hknwMapDraw(this->map, &this->worldMat);

	// パペット描画
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_NORMAL);
	hknwCharaListBind();
	for(int i = 0; i < PAGETOPMAIN_PUPPETNUM; i++){
		if(this->puppetList[i].type == PAGETOPMAINPUPPETACTIONTYPE_NONE){continue;}
		int e3dIdPuppetImage;
		struct dataPuppet *dataPuppet = dataPuppetGet(this->puppetList[i].puppetKind, &e3dIdPuppetImage);
		if(dataPuppet != NULL){
			engineGraphicEngineBindTexture(e3dIdPuppetImage);
			// ハコニワキャラクタ描画
			struct hknwChara *chara = hknwCharaListGet(dataPuppet->charaKind);
			enum hknwCharaPosesId pose = HKNWCHARAPOSESID_STAND;
			double vx = this->puppetList[i].vx;
			double vy = this->puppetList[i].vy;
			double z = 1.0;
			if(vx * vx + vy * vy > 0){pose = HKNWCHARAPOSESID_WALK;}
			if(this->puppetList[i].type == PAGETOPMAINPUPPETACTIONTYPE_JUMP){z = 1.0 + 1.0 * engineMathSin(3.1415 * this->puppetList[i].step / 30);}
			int step = (int)engineMathFloor(this->puppetList[i].step / 10) % hknwCharaGetPoseStepNum(chara, pose);
			hknwCharaDrawChara(chara, &this->worldMat, this->rotv, this->roth, this->puppetList[i].x, this->puppetList[i].y, z, this->puppetList[i].r, 1.5, pose, step);
			hknwCharaDrawShadow(chara, &this->worldMat, this->puppetList[i].x, this->puppetList[i].y, 1.0, 1.5);
		}
	}

	// 2D描画準備
	engineGraphicEngineSetStencilMode(ENGINEGRAPHICENGINEMODESTENCIL_NONE);
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);
	engineGraphicEngineSetMatrix(&e3dMatOrtho);

	// タイトル他描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
	e3dImageDraw(&this->title);
	// ウインドウ描画
	e3dImageDraw(&this->countWindow);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, 1.0);
	e3dImageDraw(&this->countLabel);
	// 数値描画
	e3dNumberDraw(&this->countNumPuppet, &e3dMatOrtho, this->puppetNum);
	e3dNumberDraw(&this->countNumMoney, &e3dMatOrtho, storageStatusGet()->money);

	// ボタン描画
	partsButtonBoxDraw(&this->btn1, &e3dMatOrtho, 1.0);
	partsButtonBoxDraw(&this->btn2, &e3dMatOrtho, 1.0);
	partsButtonBoxDraw(&this->btn3, &e3dMatOrtho, 1.0);

	// チュートリアル矢印描画
	if(this->stepTutorial11 > 0){
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, -engineMathAbs(10 * engineMathSin(3.1415 * this->stepTutorial11 * 0.05)), 0, 0);
		engineGraphicEngineSetMatrix(&tempmat);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
		e3dImageDraw(&this->tutorialArrow1);
	}
	if(this->stepTutorial12 > 0){
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, -engineMathAbs(10 * engineMathSin(3.1415 * this->stepTutorial12 * 0.05)), 0, 0);
		engineGraphicEngineSetMatrix(&tempmat);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
		e3dImageDraw(&this->tutorialArrow2);
	}

	// ポップアップ描画
	if(this->popupOption != NULL){this->popupOption->draw(this->popupOption);}
	if(this->popupManual != NULL){this->popupManual->draw(this->popupManual);}
	if(this->popupCredit != NULL){this->popupCredit->draw(this->popupCredit);}
	if(this->popupTutorial != NULL){this->popupTutorial->draw(this->popupTutorial);}

	// メモリートレースボタン描画
	partsButtonMemoryDraw();
	
	engineGraphicEngineFlush();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageCartridgeTopMain *this){
}

// 破棄
static void dispose(struct pageCartridgeTopMain *this){
	if(this->popupOption != NULL){this->popupOption->dispose(this->popupOption);}
	if(this->popupManual != NULL){this->popupManual->dispose(this->popupManual);}
	if(this->popupCredit != NULL){this->popupCredit->dispose(this->popupCredit);}
	if(this->popupTutorial != NULL){this->popupTutorial->dispose(this->popupTutorial);}
	hknwMapDispose(this->map);
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページカートリッジ作成
struct pageCartridge *pageTopMain(struct pageTopStatus *stat){
	struct pageCartridgeTopMain *this = (struct pageCartridgeTopMain*)engineUtilMemoryCalloc(1, sizeof(struct pageCartridgeTopMain));
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

