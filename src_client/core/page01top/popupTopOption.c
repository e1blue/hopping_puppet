#include "../library.h"
#include "../includeAll.h"
#include "pageTop.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ポップアップカートリッジ構造体
struct popupCartridgeTopOption{
	struct popupCartridge super;
	struct pageTopStatus *stat;
	struct popupCartridge *popupVolume;

	struct partsButtonPicker btnBgm;
	struct partsButtonPicker btnSe;
	struct partsButtonBox btnManual;
	struct partsButtonBox btnCredit;
	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage box;
	struct e3dImage labelTitle;
	struct e3dImage labelBgm;
	struct e3dImage labelSe;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	enum popupTopOptionResult result;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct popupCartridgeTopOption *this){
	// ボタン
	partsButtonPickerInit(&this->btnBgm, 4 - settingBgmVolumeGet() - 1);
	partsButtonPickerInit(&this->btnSe, 4 - settingSeVolumeGet() - 1);
	partsButtonBoxInit(&this->btnManual);
	partsButtonBoxInit(&this->btnCredit);
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct popupCartridgeTopOption *this){
}

// 主計算
static int popupCalcMain(struct popupCartridgeTopOption *this){
	if(this->popupVolume != NULL){
		// 音量ポップアップ計算
		if(!this->popupVolume->calc(this->popupVolume)){return 0;}
		// 音量ポップアップ完了時処理
		int result = this->popupVolume->result(this->popupVolume);
		if(result >= 0){
			if(this->btnBgm.select){settingBgmVolumeSet(4 - (this->btnBgm.picked = result) - 1);}
			if(this->btnSe.select){settingSeVolumeSet(4 - (this->btnSe.picked = result) - 1);}
		}
		this->btnBgm.select = this->btnSe.select = 0;
		this->popupVolume->dispose(this->popupVolume);
		this->popupVolume = NULL;
	}

	// タッチ処理
	partsButtonPickerCalc(&this->btnBgm);
	partsButtonPickerCalc(&this->btnSe);
	partsButtonBoxCalc(&this->btnManual);
	partsButtonBoxCalc(&this->btnCredit);
	partsButtonBoxCalc(&this->btnClose);
	partsButtonCalc(&this->btnOuter);

	// BGMボタン押下確認
	if(this->btnBgm.super.trigger){
		this->btnBgm.super.trigger = false;
		soundSePlayOK();
		// ポップアップ展開
		this->btnBgm.select = 1;
		char *textList[4] = {"おおきめ", "ちゅうくらい", "ちいさめ", "なし"};
		this->popupVolume = partsPopupPicker("BGMおんりょう", textList, 4, 4 - settingBgmVolumeGet() - 1);
		return 0;
	}

	// SEボタン押下確認
	if(this->btnSe.super.trigger){
		this->btnSe.super.trigger = false;
		soundSePlayOK();
		// ポップアップ展開
		this->btnSe.select = 1;
		char *textList[4] = {"おおきめ", "ちゅうくらい", "ちいさめ", "なし"};
		this->popupVolume = partsPopupPicker("SEおんりょう", textList, 4, 4 - settingSeVolumeGet() - 1);
		return 0;
	}

	// マニュアルボタン押下確認
	if(this->btnManual.super.trigger){
		this->btnManual.super.trigger = false;
		soundSePlayOK();
		// ポップアップ閉じる
		this->result = POPUPTOPOPTIONRESULT_MANUAL;
		return 1;
	}

	// クレジットボタン押下確認
	if(this->btnCredit.super.trigger){
		this->btnCredit.super.trigger = false;
		soundSePlayOK();
		// ポップアップ閉じる
		this->result = POPUPTOPOPTIONRESULT_CREDIT;
		return 1;
	}

	// 閉じるボタン押下確認
	if(this->btnClose.super.trigger || this->btnOuter.trigger || global.key.bk.triggerInactive){
		this->btnClose.super.trigger = false;
		this->btnOuter.trigger = false;
		global.key.bk.triggerInactive = false;
		soundSePlayNG();
		// ポップアップ閉じる
		this->result = POPUPTOPOPTIONRESULT_NONE;
		return 1;
	}

	return 0; 
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct popupCartridgeTopOption *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		int ww = 214;
		int wh = 211;
		int wx = (global.screen.w - ww) * 0.5;
		int wy = (global.screen.h - wh) * 0.5;

		// ウインドウ外ボタン位置
		partsButtonPosition(&this->btnOuter, wx, wy, ww, wh);

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ウインドウ作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->box, wx, wy, ww, wh);

		// ラベル作成
		e3dImageTextCreateArray(&this->labelTitle, global.screen.w * 0.5, wy + 30 * 0.5 + 3, "オプション", 1.0, 0, 0);
		e3dImageTextCreateArray(&this->labelBgm, global.screen.w * 0.5 - 15, wy + 33 + 35 * 0 + 30 * 0.5, "BGMおんりょう", 1.0, -1, 0);
		e3dImageTextCreateArray(&this->labelSe, global.screen.w * 0.5 - 15, wy + 33 + 35 * 1 + 30 * 0.5, "SEおんりょう", 1.0, -1, 0);

		// ボタン作成
		char *textList[4] = {"おおきめ", "ちゅうくらい", "ちいさめ", "なし"};
		partsButtonPickerCreateArray(&this->btnBgm, global.screen.w * 0.5 - 15 + 5, wy + 33 + 35 * 0, 28 + 5 + 10 * 6 + 13, textList, 4, 1.0);
		partsButtonPickerCreateArray(&this->btnSe, global.screen.w * 0.5 - 15 + 5, wy + 33 + 35 * 1, 28 + 5 + 10 * 6 + 13, textList, 4, 1.0);
		partsButtonBoxCreateArray(&this->btnManual, wx + (ww - 140) * 0.5, wy + 33 + 35 * 2, 140, 30, "マニュアル", 1.0);
		partsButtonBoxCreateArray(&this->btnCredit, wx + (ww - 140) * 0.5, wy + 33 + 35 * 3, 140, 30, "クレジット", 1.0);
		// 閉じるボタン作成
		partsButtonBoxCreateArray(&this->btnClose, (global.screen.w - 60) * 0.5, wy + wh - 8 - 30, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct popupCartridgeTopOption *this){
	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempmat1;
	double num = (double)this->super.openStep / this->super.openStepMax;
	double alpha = 1 - num * num;
	bool isOpening = (this->super.openStep < 0 || this->super.openStepMax <= this->super.openStep);
	bool isClosing = (0 < this->super.openStep && this->super.openStep < this->super.openStepMax);
	double alphaBack = (isClosing && this->result != POPUPTOPOPTIONRESULT_NONE) ? 1.0 : alpha;

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
	e3dImageDraw(&this->labelTitle);
	e3dImageDraw(&this->labelBgm);
	e3dImageDraw(&this->labelSe);
	// ボタン描画
	partsButtonPickerDraw(&this->btnBgm, &tempmat1, alpha);
	partsButtonPickerDraw(&this->btnSe, &tempmat1, alpha);
	partsButtonBoxDraw(&this->btnManual, &tempmat1, alpha);
	partsButtonBoxDraw(&this->btnCredit, &tempmat1, alpha);
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);

	// ポップアップ描画
	if(this->popupVolume != NULL){this->popupVolume->draw(this->popupVolume);}
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct popupCartridgeTopOption *this){
	return this->result;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct popupCartridgeTopOption *this){
	if(this->popupVolume != NULL){this->popupVolume->dispose(this->popupVolume);}
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *popupTopOption(struct pageTopStatus *stat){
	struct popupCartridgeTopOption *this = (struct popupCartridgeTopOption*)engineUtilMemoryCalloc(1, sizeof(struct popupCartridgeTopOption));
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

