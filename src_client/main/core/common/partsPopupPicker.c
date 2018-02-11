#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ポップアップカートリッジ構造体
struct partsPopupCartridgePicker{
	struct popupCartridge super;

	struct partsButton *btnSelect;
	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage window;
	struct e3dImage labelTitle;
	struct e3dImage *border;
	struct e3dImage *selectBox;
	struct e3dImage *selectLabel;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	// 項目数5個、文字数10文字の制限
	char title[32];
	char textList[5][32];

	int length;
	int select;
	int result;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct partsPopupCartridgePicker *this){
	this->btnSelect = (struct partsButton*)engineUtilMemoryCalloc(this->length, sizeof(struct partsButton));
	this->border = (struct e3dImage*)engineUtilMemoryCalloc(this->length + 1, sizeof(struct e3dImage));
	this->selectBox = (struct e3dImage*)engineUtilMemoryCalloc(this->length, sizeof(struct e3dImage));
	this->selectLabel = (struct e3dImage*)engineUtilMemoryCalloc(this->length, sizeof(struct e3dImage));

	// 項目ボタン
	for(int i = 0; i < this->length; i++){partsButtonInit(this->btnSelect + i);}
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct partsPopupCartridgePicker *this){
}

// 主計算
static int popupCalcMain(struct partsPopupCartridgePicker *this){
	// タッチ処理
	for(int i = 0; i < this->length; i++){partsButtonBoxCalc(this->btnSelect + i);}
	partsButtonBoxCalc(&this->btnClose);
	partsButtonCalc(&this->btnOuter);

	// 選択ボタン押下確認
	for(int i = 0; i < this->length; i++){
		if(this->btnSelect[i].trigger){
			this->btnSelect[i].trigger = false;
			soundSePlayOK();
			// ポップアップ閉じる
			this->result = (i != this->select) ? i : -1;
			return 1;
		}
	}

	// 閉じるボタン押下確認
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
static void createBuffer(struct partsPopupCartridgePicker *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		int sw = 114;
		int sh = 28;
		int ww = sw + 6;
		int wh = 33 + (sh + 2) * this->length + 2 + 43;
		int wx = (global.screen.w - ww) * 0.5;
		int wy = (global.screen.h - wh) * 0.5;

		// 項目ボタン位置
		for(int i = 0; i < this->length; i++){partsButtonPosition(this->btnSelect + i, wx + 3, wy + 33 + 2 + (sh + 2) * i, sw, sh);}
		// ウインドウ外ボタン位置
		partsButtonPosition(&this->btnOuter, wx, wy, ww, wh);

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ウインドウとタイトル作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->window, wx, wy, ww, wh);
		e3dImageTextCreateArray(&this->labelTitle, wx + ww * 0.5, wy + 3 + 30 * 0.5, this->title, 1.0, 0, 0);

		// 各選択項目作成
		for(int i = 0; i < this->length; i++){
			int sx = this->btnSelect[i].x;
			int sy = this->btnSelect[i].y;
			int borderOffset = (i == 0) ? 0 : 10;
			e3dImageWhiteCreateArray(this->border + i, sx + borderOffset, sy - 2, sw - borderOffset * 2, 2);
			e3dImageWhiteCreateArray(this->selectBox + i, sx, sy, sw, sh);
			e3dImageTextCreateArray(this->selectLabel + i, sx + sw * 0.5, sy + sh * 0.5, this->textList[i], 1.0, 0, 0);
		}
		// 最後のボーター作成
		e3dImageWhiteCreateArray(this->border + this->length, wx + 3, wy + 33 + (sh + 2) * this->length, sw, 2);

		// 閉じるボタン作成
		partsButtonBoxCreateArray(&this->btnClose, wx + (ww - 60) * 0.5, wy + wh - 3 - 5 - 30, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct partsPopupCartridgePicker *this){
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

	// 各項目背景色描画
	for(int i = 0; i < this->length; i++){
		if((this->btnSelect + i)->active){engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * 0.5);}
		else if(i == this->select){engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha * 0.5);}
		else{continue;}
		e3dImageDraw(this->selectBox + i);
	}

	// ラベル描画
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->labelTitle);
	// 各項目描画
	for(int i = 0; i < this->length; i++){
		e3dImageDraw(this->border + i);
		e3dImageDraw(this->selectLabel + i);
	}
	e3dImageDraw(this->border + this->length);

	// 閉じるボタン描画
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct partsPopupCartridgePicker *this){
	return this->result;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct partsPopupCartridgePicker *this){
	engineUtilMemoryFree(this->btnSelect);
	engineUtilMemoryFree(this->border);
	engineUtilMemoryFree(this->selectBox);
	engineUtilMemoryFree(this->selectLabel);
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ピッカーポップアップカートリッジ初期化
struct popupCartridge *partsPopupPicker(char *title, char **textList, int textListLength, int select){
	struct partsPopupCartridgePicker *this = (struct partsPopupCartridgePicker*)engineUtilMemoryCalloc(1, sizeof(struct partsPopupCartridgePicker));
	strcpy(this->title, title);
	for(int i = 0; i < textListLength; i++){strcpy(this->textList[i], textList[i]);}
	this->length = textListLength;
	this->select = select;
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

