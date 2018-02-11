#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

enum partsPopupMoneyInfoType{
	PARTSPOPUPMONEYINFOTYPE_GET
};

// ポップアップカートリッジ構造体
struct partsPopupCartridgeMoneyInfo{
	struct popupCartridge super;

	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage window;
	struct e3dImage image;
	struct e3dImage name;
	struct e3dImage label;
	struct e3dImage moneyWindow;
	struct e3dImage moneyLabel;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	enum partsPopupMoneyInfoType type;
	int money;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct partsPopupCartridgeMoneyInfo *this){
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct partsPopupCartridgeMoneyInfo *this){
}

// 主計算
static int popupCalcMain(struct partsPopupCartridgeMoneyInfo *this){
	// タッチ処理
	partsButtonBoxCalc(&this->btnClose);
	partsButtonCalc(&this->btnOuter);

	// ボタン押下確認
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
static void createBuffer(struct partsPopupCartridgeMoneyInfo *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		int ww = 186;
		int wh = 78;
		int wx = (global.screen.w - ww) * 0.5;
		int wy = (global.screen.h - wh) * 0.5;

		// ウインドウ外ボタン位置
		partsButtonPosition(&this->btnOuter, wx, wy, ww, wh);

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ウインドウとタイトル作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->window, wx, wy, ww, wh);
		e3dImageCreateArray(&this->image, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, wx + 3 + 5, wy + 3 + 5, 30, 30, TEXPOS_SYSTEM_IMAGEMONEY_X, TEXPOS_SYSTEM_IMAGEMONEY_Y, TEXPOS_SYSTEM_IMAGEMONEY_W, TEXPOS_SYSTEM_IMAGEMONEY_H);
		int labelx = wx + 3 + 5 + 30 + 5;
		int labely = wy + 3 + 5 + 15 * 0;
		switch(this->type){
			case PARTSPOPUPMONEYINFOTYPE_GET: e3dImageTextCreateArray(&this->label, labelx, labely, "おかねを てにいれました", 1.0, 1, 1); break;
		}
		char str[32];
		sprintf(str, "%dゲソ", this->money);
		e3dImageTextCreateArray(&this->name, wx + 3 + 5 + 30 + 5, wy + 3 + 5 + 15 * 1, str, 1.0, 1, 1);
		// お金ウインドウ作成
		e3dImageBoxDefaultCreateArray(&this->moneyWindow, 5, global.screen.h - 5 - 31, 140 + 16, 15 + 16);
		sprintf(str, "おかね%9dゲソ", storageStatusGet()->money);
		e3dImageTextCreateArray(&this->moneyLabel, 5 + 8, global.screen.h - 5 - 8 - 15, str, 1.0, 1, 1);

		// 閉じるボタン
		partsButtonBoxCreateArray(&this->btnClose, wx + (ww - 60) * 0.5, wy + wh - 5 - 30, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct partsPopupCartridgeMoneyInfo *this){
	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempmat1;
	float num = (float)this->super.openStep / this->super.openStepMax;
	float alpha = 1 - num * num;

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
	e3dImageDraw(&this->image);
	e3dImageDraw(&this->moneyWindow);
	// ラベル描画
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->name);
	e3dImageDraw(&this->label);
	e3dImageDraw(&this->moneyLabel);
	// ボタン描画
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct partsPopupCartridgeMoneyInfo *this){
	return 0;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct partsPopupCartridgeMoneyInfo *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *partsPopupMoneyInfo(enum partsPopupMoneyInfoType type, int money){
	struct partsPopupCartridgeMoneyInfo *this = (struct partsPopupCartridgeMoneyInfo*)engineUtilMemoryCalloc(1, sizeof(struct partsPopupCartridgeMoneyInfo));
	this->type = type;
	this->money = money;
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

struct popupCartridge *partsPopupMoneyInfoGet(int money){return partsPopupMoneyInfo(PARTSPOPUPMONEYINFOTYPE_GET, money);}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

