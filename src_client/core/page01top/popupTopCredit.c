#include "../library.h"
#include "../includeAll.h"
#include "pageTop.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#define POPUPTOPCREDIT_IMAGENUM 1
#define POPUPTOPCREDIT_LABELNUM 16

// ポップアップカートリッジ構造体
struct popupCartridgeTopCredit{
	struct popupCartridge super;
	struct pageTopStatus *stat;

	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage box;
	struct e3dImage titleLabel;
	struct e3dImage headerBorder;
	struct e3dImage footerBorder;
	struct e3dImage image[POPUPTOPCREDIT_IMAGENUM];
	struct e3dImage label[POPUPTOPCREDIT_LABELNUM];

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct popupCartridgeTopCredit *this){
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct popupCartridgeTopCredit *this){
}

// 主計算
static int popupCalcMain(struct popupCartridgeTopCredit *this){
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
static void createBuffer(struct popupCartridgeTopCredit *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		int ww = 310;
		int wh = 70 + (5 + 10 + 15 * 8 + 10 + 5);
		int wx = (global.screen.w - ww) * 0.5;
		int wy = (global.screen.h - wh) * 0.5;

		// ウインドウ外ボタン位置
		partsButtonPosition(&this->btnOuter, wx, wy, ww, wh);

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ウインドウとタイトル作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->box, wx, wy, ww, wh);
		e3dImageTextCreateArray(&this->titleLabel, wx + ww * 0.5, wy + 30 * 0.5, "クレジット", 1.0, 0, 0);
		e3dImageWhiteCreateArray(&this->headerBorder, wx + 3, wy + 30 - 2, ww - 6, 2);
		e3dImageWhiteCreateArray(&this->footerBorder, wx + 3, wy + wh - 40, ww - 6, 2);

		int imageNum = 0;
		int labelNum = 0;
		int labely = wy;
		e3dImageTextCreateArray(&this->label[labelNum++], wx + ww * 0.5, (labely += 35), "ホッピンパペット", 1.0, 0, 1);

		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 65, (labely += 25), "きかく:", 1.0, -1, 1);
		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 75, (labely += 0), "ぽろゆき@totetero", 1.0, 1, 1);

		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 65, (labely += 15), "プログラム:", 1.0, -1, 1);
		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 75, (labely += 0), "ぽろゆき@totetero", 1.0, 1, 1);

		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 65, (labely += 15), "デザイン:", 1.0, -1, 1);
		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 75, (labely += 0), "ぽろゆき@totetero", 1.0, 1, 1);

		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 65, (labely += 15), "イラスト:", 1.0, -1, 1);
		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 75, (labely += 0), "ぽろゆき@totetero", 1.0, 1, 1);

		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 65, (labely += 15), "BGM:", 1.0, -1, 1);
		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 75, (labely += 0), "フリー音楽素材 Senses Circuit", 1.0, 1, 1);

		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 65, (labely += 15), "SE:", 1.0, -1, 1);
		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 75, (labely += 0), "フリー音楽素材 Senses Circuit", 1.0, 1, 1);

		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 65, (labely += 25), "せいさく:", 1.0, -1, 1);
		e3dImageTextCreateArray(&this->label[labelNum++], wx + 5 + 75, (labely += 0), "2014 totetero.com", 1.0, 1, 1);

		e3dImageCreateArray(&this->image[imageNum++], TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, wx + 225, wy + 50, 60, 60, TEXPOS_SYSTEM_IMAGEPUPPETCAGE_X, TEXPOS_SYSTEM_IMAGEPUPPETCAGE_Y, TEXPOS_SYSTEM_IMAGEPUPPETCAGE_W, TEXPOS_SYSTEM_IMAGEPUPPETCAGE_H);

		// 閉じるボタン
		partsButtonBoxCreateArray(&this->btnClose, wx + (ww - 60) * 0.5, wy + wh - 5 - 30, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct popupCartridgeTopCredit *this){
	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempmat1;
	double num = (double)this->super.openStep / this->super.openStepMax;
	double alpha = 1 - num * num;
	bool isOpening = (this->super.openStep < 0 || this->super.openStepMax <= this->super.openStep);
	bool isClosing = (0 < this->super.openStep && this->super.openStep < this->super.openStepMax);
	double alphaBack = (isOpening) ? 1.0 : alpha;

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
	// ラベル描画
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->titleLabel);
	e3dImageDraw(&this->headerBorder);
	e3dImageDraw(&this->footerBorder);
	// ボタン描画
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);

	engineGraphicEngineSetMatrix(&tempmat1);
	// ウインドウ描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	for(int i = 0; i < POPUPTOPCREDIT_IMAGENUM; i++){e3dImageDraw(&this->image[i]);}
	// ラベル描画
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	for(int i = 0; i < POPUPTOPCREDIT_LABELNUM; i++){e3dImageDraw(&this->label[i]);}
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct popupCartridgeTopCredit *this){
	return 0;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct popupCartridgeTopCredit *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *popupTopCredit(struct pageTopStatus *stat){
	struct popupCartridgeTopCredit *this = (struct popupCartridgeTopCredit*)engineUtilMemoryCalloc(1, sizeof(struct popupCartridgeTopCredit));
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

