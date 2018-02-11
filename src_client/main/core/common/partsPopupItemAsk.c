#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

enum partsPopupItemAskType{
	PARTSPOPUPITEMASKTYPE_USE
};

// ポップアップカートリッジ構造体
struct partsPopupCartridgeItemAsk{
	struct popupCartridge super;

	struct partsButtonBox btnOk;
	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage box;
	struct e3dImage image;
	struct e3dImage name;
	struct e3dImage profile;
	struct e3dImage label;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	enum partsPopupItemAskType type;
	enum dataItemKind itemKind;
	bool result;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct partsPopupCartridgeItemAsk *this){
	// 確定ボタン
	partsButtonBoxInit(&this->btnOk);
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct partsPopupCartridgeItemAsk *this){
	switch(this->type){
		case PARTSPOPUPITEMASKTYPE_USE:
			switch(this->itemKind){
				case DATAITEMKIND_HEALTH:
					{
						// 体力回復アイテムの使用条件
						int health = storageStatusGet()->health;
						int healthMax = parameterPlayerHealthMax();
						this->btnOk.super.inactive = (health >= healthMax);
					} break;
				case DATAITEMKIND_CANVASFULL:
					{
						// キャンバス充填アイテムの使用条件
						bool isPuppetFull = true;
						bool isMeisterEmpty = true;
						for(int i = 0; i < storageAtelierGet()->canvasSize; i++){for(int j = 0; j < STORAGEATELIER0001_CANVASYMAX; j++){if(storageAtelierGet()->canvas[i][j].puppetKind < 0){isPuppetFull = false;}}}
						for(int i = 0; i < storageAtelierGet()->workspaceSize; i++){if(storageAtelierGet()->workspace[i].meisterKind >= 0){isMeisterEmpty = false; break;}}
						this->btnOk.super.inactive = isPuppetFull || isMeisterEmpty;
					} break;
				default: break;
			}
			break;
	}
}

// 主計算
static int popupCalcMain(struct partsPopupCartridgeItemAsk *this){
	// タッチ処理
	partsButtonBoxCalc(&this->btnOk);
	partsButtonBoxCalc(&this->btnClose);
	partsButtonCalc(&this->btnOuter);

	// ボタン押下確認
	if(this->btnOk.super.trigger){
		this->btnOk.super.trigger = false;
		switch(this->type){
			case PARTSPOPUPITEMASKTYPE_USE: soundSePlayUse(); break;
			default: soundSePlayOK(); break;
		}
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
static void createBuffer(struct partsPopupCartridgeItemAsk *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		int ww = 206;
		int wh = 113;
		int wx = (global.screen.w - ww) * 0.5;
		int wy = (global.screen.h - wh) * 0.5;

		// ウインドウ外ボタン位置
		partsButtonPosition(&this->btnOuter, wx, wy, ww, wh);

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ウインドウとタイトル作成
		char *nameList[STORAGESTATUS0001_ITEMKIND] = {ITEMNAME_HEALTH, ITEMNAME_TENSION, ITEMNAME_MONSTERMOVE, ITEMNAME_CANVASFULL, ITEMNAME_CANVASOPEN, ITEMNAME_WORKSPACEOPEN};
		char *profileList[STORAGESTATUS0001_ITEMKIND] = {
			"たいりょくを かいふくします",
			"テンションを あげていきます",
			"モンスターを うごかします",
			"すぐに パペットを いっぱいに します",
			"パペットスペースを おおきく します",
			"マイスタースペースを おおきく します"
		};
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->box, wx, wy, ww, wh);
		e3dImageCreateArray(&this->image, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, wx + 3 + 5, wy + 3 + 5, 30, 30, TEXPOS_SYSTEM_IMAGEITEM_X + (TEXPOS_SYSTEM_IMAGEITEM_W + TEXPOS_SPACE) * this->itemKind, TEXPOS_SYSTEM_IMAGEITEM_Y, TEXPOS_SYSTEM_IMAGEITEM_W, TEXPOS_SYSTEM_IMAGEITEM_H);
		e3dImageTextCreateArray(&this->name, wx + 3 + 5 + 30 + 5, wy + 3 + 5 + 30 * 0.5, nameList[this->itemKind], 1.0, 1, 0);
		e3dImageTextCreateArray(&this->profile, wx + ww * 0.5, wy + 3 + 5 + 30 + 5, profileList[this->itemKind], 1.0, 0, 1);
		int labelx = wx + ww * 0.5;
		int labely = wy + 3 + 5 + 30 + 5 + 15;
		switch(this->type){
			case PARTSPOPUPITEMASKTYPE_USE: e3dImageTextCreateArray(&this->label, labelx, labely, "このアイテムを つかいますか?", 1.0, 0, 1); break;
		}

		// 確定ボタン
		partsButtonBoxCreateArray(&this->btnOk, (global.screen.w - 60) * 0.5 - (ww + 60) / 6, wy + wh - 5 - 30, 60, 30, "つかう", 1.0);
		// 閉じるボタン
		partsButtonBoxCreateArray(&this->btnClose, (global.screen.w - 60) * 0.5 + (ww + 60) / 6, wy + wh - 5 - 30, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct partsPopupCartridgeItemAsk *this){
	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempmat1;
	double num = (double)this->super.openStep / this->super.openStepMax;
	double alpha = 1 - num * num;
	bool isOpening = (this->super.openStep < 0 || this->super.openStepMax <= this->super.openStep);
	bool isClosing = (0 < this->super.openStep && this->super.openStep < this->super.openStepMax);
	double alphaBack = (this->type == PARTSPOPUPITEMASKTYPE_USE && (isOpening || (isClosing && this->result))) ? 1.0 : alpha;

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
	e3dImageDraw(&this->profile);
	e3dImageDraw(&this->label);
	// ボタン描画
	partsButtonBoxDraw(&this->btnOk, &tempmat1, alpha);
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct partsPopupCartridgeItemAsk *this){
	return this->result ? this->itemKind : -1;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct partsPopupCartridgeItemAsk *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *partsPopupItemAsk(enum partsPopupItemAskType type, int itemKind){
	struct partsPopupCartridgeItemAsk *this = (struct partsPopupCartridgeItemAsk*)engineUtilMemoryCalloc(1, sizeof(struct partsPopupCartridgeItemAsk));
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

struct popupCartridge *partsPopupItemAskUse(int itemKind){return partsPopupItemAsk(PARTSPOPUPITEMASKTYPE_USE, itemKind);}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

