#include "../library.h"
#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

enum partsPopupItemInfoType{
	PARTSPOPUPITEMINFOTYPE_GET,
	PARTSPOPUPITEMINFOTYPE_USE
};

// ポップアップカートリッジ構造体
struct partsPopupCartridgeItemInfo{
	struct popupCartridge super;

	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage box;
	struct e3dImage image;
	struct e3dImage name;
	struct e3dImage label;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	enum partsPopupItemInfoType type;
	enum dataItemKind itemKind;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct partsPopupCartridgeItemInfo *this){
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct partsPopupCartridgeItemInfo *this){
}

// 主計算
static int popupCalcMain(struct partsPopupCartridgeItemInfo *this){
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
static void createBuffer(struct partsPopupCartridgeItemInfo *this){
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
		char *nameList[STORAGESTATUS0001_ITEMKIND] = {ITEMNAME_HEALTH, ITEMNAME_TENSION, ITEMNAME_MONSTERMOVE, ITEMNAME_CANVASFULL, ITEMNAME_CANVASOPEN, ITEMNAME_WORKSPACEOPEN};
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->box, wx, wy, ww, wh);
		e3dImageCreateArray(&this->image, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, wx + 3 + 5, wy + 3 + 5, 30, 30, TEXPOS_SYSTEM_IMAGEITEM_X + (TEXPOS_SYSTEM_IMAGEITEM_W + TEXPOS_SPACE) * this->itemKind, TEXPOS_SYSTEM_IMAGEITEM_Y, TEXPOS_SYSTEM_IMAGEITEM_W, TEXPOS_SYSTEM_IMAGEITEM_H);
		int labelx = wx + 3 + 5 + 30 + 5;
		int labely = wy + 3 + 5 + 15 * 0;
		switch(this->type){
			case PARTSPOPUPITEMINFOTYPE_GET: e3dImageTextCreateArray(&this->label, labelx, labely, "アイテムを てにいれました", 1.0, 1, 1); break;
			case PARTSPOPUPITEMINFOTYPE_USE: e3dImageTextCreateArray(&this->label, labelx, labely, "アイテムを つかいました", 1.0, 1, 1); break;
		}
		e3dImageTextCreateArray(&this->name, wx + 3 + 5 + 30 + 5, wy + 3 + 5 + 15 * 1, nameList[this->itemKind], 1.0, 1, 1);

		// 閉じるボタン
		partsButtonBoxCreateArray(&this->btnClose, wx + (ww - 60) * 0.5, wy + wh - 5 - 30, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct partsPopupCartridgeItemInfo *this){
	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempmat1;
	double num = (double)this->super.openStep / this->super.openStepMax;
	double alpha = 1 - num * num;
	bool isOpening = (this->super.openStep < 0 || this->super.openStepMax <= this->super.openStep);
	bool isClosing = (0 < this->super.openStep && this->super.openStep < this->super.openStepMax);
	double alphaBack = (this->type == PARTSPOPUPITEMINFOTYPE_USE && isOpening) ? 1.0 : alpha;

	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
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
	e3dImageDraw(&this->box);
	e3dImageDraw(&this->image);
	// ラベル描画
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->name);
	e3dImageDraw(&this->label);
	// ボタン描画
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct partsPopupCartridgeItemInfo *this){
	return 0;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct partsPopupCartridgeItemInfo *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *partsPopupItemInfo(enum partsPopupItemInfoType type, int itemKind){
	struct partsPopupCartridgeItemInfo *this = (struct partsPopupCartridgeItemInfo*)engineUtilMemoryCalloc(1, sizeof(struct partsPopupCartridgeItemInfo));
	this->type = type;
	this->itemKind = itemKind;
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

struct popupCartridge *partsPopupItemInfoGet(int itemKind){return partsPopupItemInfo(PARTSPOPUPITEMINFOTYPE_GET, itemKind);}
struct popupCartridge *partsPopupItemInfoUse(int itemKind){return partsPopupItemInfo(PARTSPOPUPITEMINFOTYPE_USE, itemKind);}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

