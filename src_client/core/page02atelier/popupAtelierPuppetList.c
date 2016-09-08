#include "../library.h"
#include "../includeAll.h"
#include "pageAtelier.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#define POPUPATELIERPUPPETLIST_BY 75
#define POPUPATELIERPUPPETLIST_BH 295
#define POPUPATELIERPUPPETLIST_ROWNUM 4

// ポップアップカートリッジ構造体
struct popupCartridgeAtelierPuppetList{
	struct popupCartridge super;
	struct pageAtelierStatus *stat;
	struct popupCartridge *popupProfile;

	struct buttonAtelierPuppet *btnPuppet[STORAGEPUPPET0001_KIND];
	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage belt;
	struct e3dImage titleWindow;
	struct e3dImage titleLabel;
	struct e3dImage countWindow;
	struct e3dImage countLabel;
	struct e3dNumber countNum;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	// 画面移動
	double slide;
	double slideNext;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct popupCartridgeAtelierPuppetList *this){
	// パペットボタン作成
	for(int i = 0; i < STORAGEPUPPET0001_KIND; i++){
		int x = engineMathFloor(i / POPUPATELIERPUPPETLIST_ROWNUM) * 125;
		int y = POPUPATELIERPUPPETLIST_BY + 10 + (i % POPUPATELIERPUPPETLIST_ROWNUM) * 70;
		this->btnPuppet[i] = buttonAtelierPuppetInit(i);
		buttonAtelierPuppetPosition(this->btnPuppet[i], x, y);
	}
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;

	// 画面移動
	this->slideNext = this->stat->slidePuppetList;
	this->slide = this->slideNext + 200;
}

// ----------------------------------------------------------------

// ベルト移動の計算
static void calcSlide(struct popupCartridgeAtelierPuppetList *this){
	static int prevsx;
	// タッチ状態の確認
	struct engineCtrlTouch *t = engineCtrlTouchGet(ENGINECTRLTOUCHID_SCREEN);
	// ベルト内ボタンの確認
	int mv = (t != NULL && !t->mv);
	for(int i = 0; i < STORAGEPUPPET0001_KIND; i++){buttonAtelierPuppetSubCalc(this->btnPuppet[i], t, mv);}
	// ベルト移動の確認
	if(t != NULL){
		if(t->dn){
			if(!t->active && POPUPATELIERPUPPETLIST_BY < t->screen.y && t->screen.y < POPUPATELIERPUPPETLIST_BY + POPUPATELIERPUPPETLIST_BH){
				// タッチ開始時
				prevsx = t->screen.x;
				engineCtrlTouchOwn();
			}

			// 画面移動処理
			if(t->mv){
				this->slideNext += 1.0 * (prevsx - t->screen.x);
				double min = 0;
				double max = 125 * engineMathFloor((STORAGEPUPPET0001_KIND - 1) / POPUPATELIERPUPPETLIST_ROWNUM);
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
static void calcDamper(struct popupCartridgeAtelierPuppetList *this){
	// ベルト移動の減衰計算
	double ds = this->slide - this->slideNext;
	if(engineMathAbs(ds) > 0.01){this->slide -= ds * 0.2;}
	for(int i = 0; i < STORAGEPUPPET0001_KIND; i++){
		buttonAtelierPuppetMove(this->btnPuppet[i], global.screen.w * 0.5 - 60 - this->slide, 0);
	}
}

// 前方計算
static void popupCalcBefore(struct popupCartridgeAtelierPuppetList *this){
	calcDamper(this);

	// パペットボタン使用可否
	for(int i = 0; i < STORAGEPUPPET0001_KIND; i++){this->btnPuppet[i]->super.inactive = (storagePuppetGet()->list[i].level <= 0);}
}

// 主計算
static int popupCalcMain(struct popupCartridgeAtelierPuppetList *this){
	if(this->popupProfile != NULL){
		// パペットリストポップアップ計算
		if(!this->popupProfile->calc(this->popupProfile)){return 0;}
		// ポップアップ完了時処理
		if(this->popupProfile->result(this->popupProfile)){}
		for(int i = 0; i < STORAGEPUPPET0001_KIND; i++){this->btnPuppet[i]->select = 0;}
		this->popupProfile->dispose(this->popupProfile);
		this->popupProfile = NULL;
	}

	// タッチ処理
	calcSlide(this);
	partsButtonBoxCalc(&this->btnClose);
	partsButtonCalc(&this->btnOuter);

	// パペットボタン押下確認
	for(int i = 0; i < STORAGEPUPPET0001_KIND; i++){
		if(this->btnPuppet[i]->super.trigger){
			this->btnPuppet[i]->super.trigger = false;
			soundSePlayOK();
			// 隠れていた場合見える位置にスクロール
			int x = this->btnPuppet[i]->super.x + this->btnPuppet[i]->super.movex;
			int min = 20;
			int max = global.screen.w - this->btnPuppet[i]->super.w - 20;
			if(x < min){this->slideNext += x - min;}
			if(x > max){this->slideNext += x - max;}
			// ポップアップ展開
			this->btnPuppet[i]->select = 1;
			this->popupProfile = partsPopupPuppetProfile(i);
			return 0;
		}
	}

	// 閉じるボタン押下確認
	if(this->btnClose.super.trigger || this->btnOuter.trigger || global.key.bk.triggerInactive){
		this->btnClose.super.trigger = false;
		this->btnOuter.trigger = false;
		global.key.bk.triggerInactive = false;
		soundSePlayNG();
		// ポップアップ閉じる
		return 1;
	}

	return 0; 
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct popupCartridgeAtelierPuppetList *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// ウインドウ外ボタン位置
		partsButtonPosition(&this->btnOuter, 0, POPUPATELIERPUPPETLIST_BY, global.screen.w, POPUPATELIERPUPPETLIST_BH + 40);

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ベルトとタイトル作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageWhiteCreateArray(&this->belt, 0, POPUPATELIERPUPPETLIST_BY, global.screen.w, POPUPATELIERPUPPETLIST_BH);
		e3dImageBoxDefaultCreateArray(&this->titleWindow, (global.screen.w - 200) * 0.5, POPUPATELIERPUPPETLIST_BY - 30 - 5, 200, 30);
		e3dImageTextCreateArray(&this->titleLabel, global.screen.w * 0.5, POPUPATELIERPUPPETLIST_BY - 30 * 0.5 - 5, "パペットかくにん", 1.0, 0, 0);
		// カウントウインドウ作成
		e3dImageBoxDefaultCreateArray(&this->countWindow, 5, global.screen.h - 5 - 31, 110 + 16, 15 + 16);
		e3dImageTextCreateArray(&this->countLabel, 5 + 8, global.screen.h - 5 - 8 - 15, "パペット     たい", 1.0, 1, 1);
		e3dNumberDefaultCreateArray(&this->countNum, 5 + 8 + 90, global.screen.h - 5 - 8 - 15, 1.0, -1, 1);

		// 閉じるボタン作成
		partsButtonBoxCreateArray(&this->btnClose, global.screen.w - 60 - 5, POPUPATELIERPUPPETLIST_BY + POPUPATELIERPUPPETLIST_BH + 5, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct popupCartridgeAtelierPuppetList *this){
	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempmat1;
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
	// ウインドウ描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	e3dImageDraw(&this->titleWindow);
	e3dImageDraw(&this->countWindow);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->titleLabel);
	e3dImageDraw(&this->countLabel);
	// 数値描画
	e3dNumberDraw(&this->countNum, &tempmat1, parameterPlayerPuppetNum());
	// ボタン描画
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);

	engineMathMat4Copy(&tempmat1, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat1, 0, (POPUPATELIERPUPPETLIST_BY + POPUPATELIERPUPPETLIST_BH * 0.5), 0);
	engineMathMat4Scale(&tempmat1, 1.0, 1 - num * num, 1.0);
	engineMathMat4Translate(&tempmat1, 0, -(POPUPATELIERPUPPETLIST_BY + POPUPATELIERPUPPETLIST_BH * 0.5), 0);
	engineGraphicEngineSetMatrix(&tempmat1);

	// ベルトマスク設定
	engineGraphicEngineClearStencil();
	engineGraphicEngineSetStencilMode(ENGINEGRAPHICENGINEMODESTENCIL_MASK_1);
	e3dImageDraw(&this->belt);
	engineGraphicEngineSetStencilMode(ENGINEGRAPHICENGINEMODESTENCIL_READ_EQ1);

	// ベルト描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha * 0.5);
	e3dImageDraw(&this->belt);

	// パペットボタンデータ描画
	for(int i = 0; i < STORAGEPUPPET0001_KIND; i++){
		buttonAtelierPuppetDrawData(this->btnPuppet[i], &tempmat1, alpha);
	}

	// パペットボタンキャラクタ描画
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_NORMAL);
	engineGraphicEngineClearDepth();
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	hknwCharaListBind();
	for(int i = 0; i < STORAGEPUPPET0001_KIND; i++){
		buttonAtelierPuppetDrawChara(this->btnPuppet[i], &tempmat1);
	}

	// 色々リセット
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
	engineGraphicEngineSetStencilMode(ENGINEGRAPHICENGINEMODESTENCIL_NONE);
	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);

	// ポップアップ描画
	if(this->popupProfile != NULL){this->popupProfile->draw(this->popupProfile);}
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct popupCartridgeAtelierPuppetList *this){
	return 0;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct popupCartridgeAtelierPuppetList *this){
	this->stat->slidePuppetList = this->slideNext;

	if(this->popupProfile != NULL){this->popupProfile->dispose(this->popupProfile);}
	for(int i = 0; i < STORAGEPUPPET0001_KIND; i++){buttonAtelierPuppetDispose(this->btnPuppet[i]);}
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *popupAtelierPuppetList(struct pageAtelierStatus *stat){
	struct popupCartridgeAtelierPuppetList *this = (struct popupCartridgeAtelierPuppetList*)engineUtilMemoryCalloc(1, sizeof(struct popupCartridgeAtelierPuppetList));
	this->stat = stat;
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

