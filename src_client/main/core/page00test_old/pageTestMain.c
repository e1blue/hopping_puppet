#include "../includeAll.h"
#include "pageTest.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体
struct pageCartridgeTestMain{
	struct pageCartridge super;
	struct pageTestStatus *stat;
	struct popupCartridge *popupTest;

	struct hknwMap *map;

	struct partsButtonBox btnPopup;
	struct partsButtonBox btnBack;

	int faceIndexCtrlBtnArrow;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	// パペット情報
	int puppetKind;
	// キャラクター
	double px;
	double py;
	double pz;
	double pr;
	int pmove;
	int pstep;

	// 画面回転
	double rotv;
	double roth;
	double dist;
	double rotvNext;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageCartridgeTestMain *this){
	// マップチップ作成
	int mapchip[6][5][5] = {{
		{ 4,  4,  4,  4,  4},
		{ 4,  4,  4,  4,  4},
		{ 4,  4,  4,  4,  4},
		{ 4,  4,  4,  4,  4},
		{ 4,  4,  4,  4,  4}
	 },{
		{ 0,  0,  0, 10, 10},
		{ 0, 13,  0,  0, 10},
		{ 0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0}
	},{
		{ 0, 14,  0,  0, 10},
		{14, 13, 14,  0,  0},
		{ 0, 14,  0,  0,  0},
		{ 0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0}
	},{
		{ 0, 14,  0,  0,  0},
		{14, 13, 14,  0,  0},
		{ 0, 14,  0,  0,  0},
		{ 0,  0,  0, 17,  0},
		{ 0,  0,  0,  0,  0}
	},{
		{ 0,  0,  0,  0,  0},
		{ 0, 14,  0,  0,  0},
		{ 0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0}
	},{
		{ 0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0},
		{ 0,  0,  0,  0,  0}
	}};
	this->map = hknwMapInit(5, 5, 6, (int*)mapchip);

	// ボタン作成
	partsButtonBoxInit(&this->btnPopup);
	partsButtonBoxInit(&this->btnBack);

	// パペット情報
	this->puppetKind = 0;
	// キャラクタ
	this->px = 2.5;
	this->py = 2.5;
	this->pz = 1.0;
	this->pr = 3.1415 / 180 * 90;
	this->pmove = 0;
	this->pstep = 0;

	// 画面回転
	this->rotv = 3.1415 / 180 *  0;
	this->roth = 3.1415 / 180 * 30;
	this->dist = 10;
	this->rotvNext = this->rotv;
}

// ----------------------------------------------------------------

// 画面回転の計算
static void calcRotate(struct pageCartridgeTestMain *this){
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
				// 画面回転処理
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
					this->rotvNext += 0.8 * engineMathAcos(cos) * ((x1 * y0 - y1 * x0 > 0) ? 1 : -1);
				}
			}
			prevsx = t->screen.x;
			prevsy = t->screen.y;
		}else{
			// タッチ終了時
			engineCtrlTouchFree();
		}
	}
}

// 減衰計算
static void calcDamper(struct pageCartridgeTestMain *this){
	// 画面回転角度の減衰計算
	double drv = this->rotv - this->rotvNext;
	while(drv < -3.1415){drv += 3.1415 * 2;}
	while(drv >  3.1415){drv -= 3.1415 * 2;}
	if(engineMathAbs(drv) > 0.01){this->rotv -= drv * 0.2;}
}

// 計算
static int calc(struct pageCartridgeTestMain *this){
	// メモリートレースボタン計算
	partsButtonMemoryCalc();
	
	calcDamper(this);

	if(this->popupTest != NULL){
		// テストポップアップ計算
		if(!this->popupTest->calc(this->popupTest)){return 0;}
		// テストポップアップ完了時処理
		if(this->popupTest->result(this->popupTest)){}
		this->popupTest->dispose(this->popupTest);
		this->popupTest = NULL;
	}

	// タッチ処理
	ctrlBtnCalc();
	partsButtonBoxCalc(&this->btnPopup);
	partsButtonBoxCalc(&this->btnBack);
	calcRotate(this);

	// ポップアップボタン押下確認
	if(this->btnPopup.super.trigger){
		this->btnPopup.super.trigger = false;
		// ポップアップ展開
		this->popupTest = popupTestTest(this->stat);
		return 0;
	}

	// もどるボタン押下確認
	if(this->btnBack.super.trigger || global.key.bk.triggerInactive){
		this->btnBack.super.trigger = false;
		global.key.bk.triggerInactive = false;
		// 終了
		engineUtilSystemExit();
	}

	// キャラクター状態更新
	this->pstep++;
	this->pmove = 1;
	double dir = 0;
	if(0){}
	else if(ctrlBtnKrt && ctrlBtnKup){dir = 1.74;}
	else if(ctrlBtnKlt && ctrlBtnKup){dir = 1.26;}
	else if(ctrlBtnKlt && ctrlBtnKdn){dir = 0.74;}
	else if(ctrlBtnKrt && ctrlBtnKdn){dir = 0.26;}
	else if(ctrlBtnKrt){dir = 0.00;}
	else if(ctrlBtnKup){dir = 1.50;}
	else if(ctrlBtnKlt){dir = 1.00;}
	else if(ctrlBtnKdn){dir = 0.50;}
	else{this->pmove = 0;}
	if(this->pmove){
		this->pr = dir * 3.1415 + this->rotv;
		this->px += 0.05 * engineMathCos(this->pr);
		this->py += 0.05 * engineMathSin(this->pr);
	}

	return 0;
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct pageCartridgeTestMain *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ボタン作成
		partsButtonBoxCreateArray(&this->btnPopup, global.screen.w - 5 - 100, global.screen.h - 5 - 30 - 5 - 30, 100, 30, "テスト", 1.0);
		partsButtonBoxCreateArray(&this->btnBack, global.screen.w - 5 - 100, global.screen.h - 5 - 30, 100, 30, "もどる", 1.0);

		// 十字キー作成
		ctrlBtnArrowCreateArray(&this->faceIndexCtrlBtnArrow);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct pageCartridgeTestMain *this){
	// 描画準備
	createBuffer(this);

	// 射影行列作成
	double xFrustum = 0.050 * global.screen.aspect.x * ((global.screen.h < global.screen.w) ? ((double)global.screen.w / global.screen.h) : 1);
	double yFrustum = 0.050 * global.screen.aspect.y * ((global.screen.h > global.screen.w) ? ((double)global.screen.h / global.screen.w) : 1);
	engineMathMat4Frustum(&e3dMatFrustum, -xFrustum, xFrustum, -yFrustum, yFrustum, 0.1, 100);
	engineMathMat4Ortho(&e3dMatOrtho, -global.screen.offset.x, global.screen.w + global.screen.offset.x, global.screen.h + global.screen.offset.y, -global.screen.offset.y, -1, 1);
	// ワールド行列作成
	struct engineMathMatrix44 worldMat;
	engineMathMat4Copy(&worldMat, &e3dMatFrustum);
	engineMathMat4Translate(&worldMat, 0, 0, -this->dist);
	engineMathMat4RotateX(&worldMat, this->roth);
	engineMathMat4RotateY(&worldMat, this->rotv);
	engineMathMat4Translate(&worldMat, -this->px, -1.0, -this->py);

	engineGraphicEngineClearAll();

	// ハコニワ描画
	engineGraphicEngineBindTexture(e3dIdSystemImageNearest);
	hknwMapDraw(this->map, &worldMat);

	// キャラクタ描画
	int e3dIdPuppetImage;
	struct dataPuppet *dataPuppet = dataPuppetGet(this->puppetKind, &e3dIdPuppetImage);
	if(dataPuppet != NULL){
		engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_NORMAL);
		hknwCharaListBind();
		engineGraphicEngineBindTexture(e3dIdPuppetImage);
		struct hknwChara *chara = hknwCharaListGet(dataPuppet->charaKind);
		enum hknwCharaPosesId pose = this->pmove ? HKNWCHARAPOSESID_WALK : HKNWCHARAPOSESID_STAND;
		int step = (int)engineMathFloor(this->pstep / 10) % hknwCharaGetPoseStepNum(chara, pose);
		hknwCharaDrawChara(chara, &worldMat, this->rotv, this->roth, this->px, this->py, this->pz, this->pr, 1.5, pose, step);
		hknwCharaDrawShadow(chara, &worldMat, this->px, this->py, this->pz, 1.5);
		// 大量に並べるテスト
		int num = 0;
		for(int i = 0; i < num; i++){
			double pos = ((num > 1) ? ((double)i / (num - 1)) : 0.0);
			hknwCharaDrawChara(chara, &worldMat, this->rotv, this->roth, 0.5 + 4.0 * pos, 4.5, 1.0, 3.1415 / 180 * 90, 1.5, HKNWCHARAPOSESID_STAND, 0);
		}
	}

	// 2D描画準備
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);
	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
	engineGraphicEngineSetMatrix(&e3dMatOrtho);

	// ボタン描画
	partsButtonBoxDraw(&this->btnPopup, &e3dMatOrtho, 1.0);
	partsButtonBoxDraw(&this->btnBack, &e3dMatOrtho, 1.0);

	// コントローラボタン描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
	ctrlBtnDraw(this->faceIndexCtrlBtnArrow);

	// ポップアップ描画
	if(this->popupTest != NULL){this->popupTest->draw(this->popupTest);}

	// メモリートレースボタン描画
	partsButtonMemoryDraw();
	
	engineGraphicEngineFlush();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageCartridgeTestMain *this){
}

// 破棄
static void dispose(struct pageCartridgeTestMain *this){
	if(this->popupTest != NULL){this->popupTest->dispose(this->popupTest);}
	hknwMapDispose(this->map);
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページカートリッジ作成
struct pageCartridge *pageTestMain(struct pageTestStatus *stat){
	struct pageCartridgeTestMain *this = (struct pageCartridgeTestMain*)engineUtilMemoryCalloc(1, sizeof(struct pageCartridgeTestMain));
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

