#include "../library.h"
#include "../includeAll.h"
#include "pageField.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

enum popupFieldInfoType{
	POPUPFIELDINFOTYPE_HEALTH,
	POPUPFIELDINFOTYPE_PUPPET,
	POPUPFIELDINFOTYPE_TRIAL
};

// ポップアップカートリッジ構造体
struct popupCartridgeFieldInfo{
	struct popupCartridge super;
	struct pageFieldStatus *stat;

	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage box;
	struct e3dImage label;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	enum popupFieldInfoType type;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct popupCartridgeFieldInfo *this){
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct popupCartridgeFieldInfo *this){
}

// 主計算
static int popupCalcMain(struct popupCartridgeFieldInfo *this){
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
static void createBuffer(struct popupCartridgeFieldInfo *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		int lineNum = 1;
		switch(this->type){
			case POPUPFIELDINFOTYPE_HEALTH: lineNum = 3; break;
			case POPUPFIELDINFOTYPE_PUPPET: lineNum = 2; break;
			case POPUPFIELDINFOTYPE_TRIAL: lineNum = 2; break;
		}
		int ww = 190;
		int wh = 50 + 15 * lineNum;
		int wx = (global.screen.w - ww) * 0.5;
		int wy = (global.screen.h - wh) * 0.5;

		// ウインドウ外ボタン位置
		partsButtonPosition(&this->btnOuter, wx, wy, ww, wh);

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ウインドウとタイトル作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->box, wx, wy, ww, wh);
		switch(this->type){
			case POPUPFIELDINFOTYPE_HEALTH: e3dImageTextCreateArray(&this->label, wx + ww * 0.5, wy + 10, "いちど たおれたら\nタイリョクが かんぜんに\nかいふく するまで たたかえません", 1.0, 0, 1); break;
			case POPUPFIELDINFOTYPE_PUPPET: e3dImageTextCreateArray(&this->label, wx + ww * 0.5, wy + 10, "パペットを もっていないので\nたたかえません", 1.0, 0, 1); break;
			case POPUPFIELDINFOTYPE_TRIAL: e3dImageTextCreateArray(&this->label, wx + ww * 0.5, wy + 10, "webたいけんばんは ここまで.\nつぎは アプリで あいましょう.", 1.0, 0, 1); break;
		}

		// 閉じるボタン
		partsButtonBoxCreateArray(&this->btnClose, wx + (ww - 60) * 0.5, wy + wh - 5 - 30, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct popupCartridgeFieldInfo *this){
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
	e3dImageDraw(&this->box);
	// ラベル描画
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->label);
	// ボタン描画
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct popupCartridgeFieldInfo *this){
	return 0;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct popupCartridgeFieldInfo *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *popupFieldInfo(struct pageFieldStatus *stat, enum popupFieldInfoType type){
	struct popupCartridgeFieldInfo *this = (struct popupCartridgeFieldInfo*)engineUtilMemoryCalloc(1, sizeof(struct popupCartridgeFieldInfo));
	this->stat = stat;
	this->type = type;
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

struct popupCartridge *popupFieldInfoHealth(struct pageFieldStatus *stat){return popupFieldInfo(stat, POPUPFIELDINFOTYPE_HEALTH);}
struct popupCartridge *popupFieldInfoPuppet(struct pageFieldStatus *stat){return popupFieldInfo(stat, POPUPFIELDINFOTYPE_PUPPET);}
struct popupCartridge *popupFieldInfoTrial(struct pageFieldStatus *stat){return popupFieldInfo(stat, POPUPFIELDINFOTYPE_TRIAL);}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

