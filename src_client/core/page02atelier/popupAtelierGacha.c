#include "../library.h"
#include "../includeAll.h"
#include "pageAtelier.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#define POPUPATELIERGACHA_BTNGACHALENGTH 3

// ポップアップカートリッジ構造体
struct popupCartridgeAtelierGacha{
	struct popupCartridge super;
	struct pageAtelierStatus *stat;

	struct buttonAtelierGacha *btnGacha[POPUPATELIERGACHA_BTNGACHALENGTH];
	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage window;
	struct e3dImage titleLabel;
	struct e3dImage moneyWindow;
	struct e3dImage moneyLabel;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	int result;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct popupCartridgeAtelierGacha *this){
	// ガチャボタン作成
	for(int i = 0; i < POPUPATELIERGACHA_BTNGACHALENGTH; i++){
		this->btnGacha[i] = buttonAtelierGachaInit(i);
	}
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct popupCartridgeAtelierGacha *this){
}

// 主計算
static int popupCalcMain(struct popupCartridgeAtelierGacha *this){
	// タッチ処理
	for(int i = 0; i < POPUPATELIERGACHA_BTNGACHALENGTH; i++){buttonAtelierGachaCalc(this->btnGacha[i]);}
	partsButtonBoxCalc(&this->btnClose);
	partsButtonCalc(&this->btnOuter);

	// ガチャボタン押下確認
	for(int i = 0; i < POPUPATELIERGACHA_BTNGACHALENGTH; i++){
		if(this->btnGacha[i]->super.trigger){
			this->btnGacha[i]->super.trigger = false;
			soundSePlayOK();
			// ポップアップ閉じる
			this->result = i;
			return 1;
		}
	}

	// ボタン押下確認
	if(this->btnClose.super.trigger || this->btnOuter.trigger || global.key.bk.triggerInactive){
		this->btnClose.super.trigger = false;
		this->btnOuter.trigger = false;
		global.key.bk.triggerInactive = false;
		soundSePlayNG();
		// ポップアップ閉じる
		this->result = -1;
		return 1;
	}

	return 0;
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct popupCartridgeAtelierGacha *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		int ww = 240;
		int wh = 266 - 65 * 3 + 65 * POPUPATELIERGACHA_BTNGACHALENGTH;
		int wx = (global.screen.w - ww) * 0.5;
		int wy = (global.screen.h - wh) * 0.5;

		// ガチャボタン位置
		for(int i = 0; i < POPUPATELIERGACHA_BTNGACHALENGTH; i++){buttonAtelierGachaPosition(this->btnGacha[i], wx + 10, wy + 33 + i * 65);}
		// ウインドウ外ボタン位置
		partsButtonPosition(&this->btnOuter, wx, wy, ww, wh);

		// バッファ作成開始
		engineGraphicBufferBegin();

		char str[32];
		// ウインドウとタイトル作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->window, wx, wy, ww, wh);
		e3dImageTextCreateArray(&this->titleLabel, wx + ww * 0.5, wy + 30 * 0.5 + 3, "マイスターガチャ", 1.0, 0, 0);
		// お金ウインドウ作成
		e3dImageBoxDefaultCreateArray(&this->moneyWindow, 5, global.screen.h - 5 - 31, 140 + 16, 15 + 16);
		sprintf(str, "おかね%9dゲソ", storageStatusGet()->money);
		e3dImageTextCreateArray(&this->moneyLabel, 5 + 8, global.screen.h - 5 - 8 - 15, str, 1.0, 1, 1);

		// 閉じるボタン
		partsButtonBoxCreateArray(&this->btnClose, wx + (ww - 60) * 0.5, wy + wh - 8 - 30, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct popupCartridgeAtelierGacha *this){
	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempmat1;
	double num = (double)this->super.openStep / this->super.openStepMax;
	double alpha = 1 - num * num;
	bool isOpening = (this->super.openStep < 0 || this->super.openStepMax <= this->super.openStep);
	bool isClosing = (0 < this->super.openStep && this->super.openStep < this->super.openStepMax);
	double alphaBack = (isClosing && this->result >= 0) ? 1.0 : alpha;

	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);

	// 背景半透明幕描画
	engineMathMat4Identity(&tempmat1);
	engineGraphicEngineSetMatrix(&tempmat1);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alphaBack * 0.5);
	e3dImageDraw(&this->back);

	engineMathMat4Copy(&tempmat1, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat1, global.screen.w * 0.5, global.screen.h * 0.5, 0);
	engineMathMat4Scale(&tempmat1, 1 + 0.5 * num * num, 1 + 0.5 * num * num, 1.0);
	engineMathMat4Translate(&tempmat1, -global.screen.w * 0.5, -global.screen.h * 0.5, 0);
	engineGraphicEngineSetMatrix(&tempmat1);
	// ウインドウ描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	e3dImageDraw(&this->window);
	e3dImageDraw(&this->moneyWindow);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->titleLabel);
	e3dImageDraw(&this->moneyLabel);
	// ボタン描画
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);

	// ガチャボタン
	for(int i = 0; i < POPUPATELIERGACHA_BTNGACHALENGTH; i++){
		buttonAtelierGachaDraw(this->btnGacha[i], &tempmat1, alpha);
		engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
	}
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct popupCartridgeAtelierGacha *this){
	return this->result;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct popupCartridgeAtelierGacha *this){
	for(int i = 0; i < POPUPATELIERGACHA_BTNGACHALENGTH; i++){buttonAtelierGachaDispose(this->btnGacha[i]);}
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *popupAtelierGacha(struct pageAtelierStatus *stat){
	struct popupCartridgeAtelierGacha *this = (struct popupCartridgeAtelierGacha*)engineUtilMemoryCalloc(1, sizeof(struct popupCartridgeAtelierGacha));
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

