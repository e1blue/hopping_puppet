#include "../library.h"
#include "../includeAll.h"
#include "pageField.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ポップアップカートリッジ構造体
struct popupCartridgeFieldOpen{
	struct popupCartridge super;
	struct pageFieldStatus *stat;

	struct partsButtonBox btnOk;
	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage window;
	struct e3dImage label;
	struct e3dImage moneyWindow;
	struct e3dImage moneyLabel;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	unsigned int cost;
	bool result;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct popupCartridgeFieldOpen *this){
	// 確定ボタン
	partsButtonBoxInit(&this->btnOk);
	this->btnOk.super.inactive = (storageStatusGet()->money < this->cost);
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct popupCartridgeFieldOpen *this){
}

// 主計算
static int popupCalcMain(struct popupCartridgeFieldOpen *this){
	// タッチ処理
	partsButtonBoxCalc(&this->btnOk);
	partsButtonBoxCalc(&this->btnClose);
	partsButtonCalc(&this->btnOuter);

	// ボタン押下確認
	if(this->btnOk.super.trigger){
		this->btnOk.super.trigger = false;
		soundSePlayOK();
		// ポップアップ閉じる
		this->result = true;
		return 1;
	}

	// ボタン押下確認
	if(this->btnClose.super.trigger || this->btnOuter.trigger || global.key.bk.triggerInactive){
		this->btnClose.super.trigger = false;
		this->btnOuter.trigger = false;
		global.key.bk.triggerInactive = false;
		soundSePlayNG();
		// ポップアップ閉じる
		this->result = false;
		return 1;
	}

	return 0; 
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct popupCartridgeFieldOpen *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		int ww = 180;
		int wh = 80;
		int wx = (global.screen.w - ww) * 0.5;
		int wy = (global.screen.h - wh) * 0.5;

		// ウインドウ外ボタン位置
		partsButtonPosition(&this->btnOuter, wx, wy, ww, wh);

		// バッファ作成開始
		engineGraphicBufferBegin();

		char str[80];
		// ウインドウとタイトル作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->window, wx, wy, ww, wh);
		sprintf(str, "%dゲソを つかって\nしゅういを かいほう しますか?", this->cost);
		e3dImageTextCreateArray(&this->label, wx + 10, wy + 10, str, 1.0, 1, 1);
		// お金ウインドウ作成
		e3dImageBoxDefaultCreateArray(&this->moneyWindow, 5, global.screen.h - 5 - 31, 140 + 16, 15 + 16);
		sprintf(str, "おかね%9dゲソ", storageStatusGet()->money);
		e3dImageTextCreateArray(&this->moneyLabel, 5 + 8, global.screen.h - 5 - 8 - 15, str, 1.0, 1, 1);

		// 確定ボタン
		partsButtonBoxCreateArray(&this->btnOk, (global.screen.w - 60) * 0.5 - (ww + 60) / 6, wy + wh - 5 - 30, 60, 30, "かいほう", 1.0);
		// 閉じるボタン
		partsButtonBoxCreateArray(&this->btnClose, (global.screen.w - 60) * 0.5 + (ww + 60) / 6, wy + wh - 5 - 30, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct popupCartridgeFieldOpen *this){
	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempmat1;
	double num = (double)this->super.openStep / this->super.openStepMax;
	double alpha = 1 - num * num;

	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
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
	e3dImageDraw(&this->window);
	e3dImageDraw(&this->moneyWindow);
	// ラベル描画
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->label);
	e3dImageDraw(&this->moneyLabel);
	// ボタン描画
	partsButtonBoxDraw(&this->btnOk, &tempmat1, alpha);
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct popupCartridgeFieldOpen *this){
	return this->result;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct popupCartridgeFieldOpen *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *popupFieldOpen(struct pageFieldStatus *stat, int cost){
	struct popupCartridgeFieldOpen *this = (struct popupCartridgeFieldOpen*)engineUtilMemoryCalloc(1, sizeof(struct popupCartridgeFieldOpen));
	this->stat = stat;
	this->cost = cost;
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

