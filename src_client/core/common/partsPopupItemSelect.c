#include "../library.h"
#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ポップアップカートリッジ構造体
struct partsPopupCartridgeItemSelect{
	struct popupCartridge super;

	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage window;
	struct e3dImage labelTitle;
	struct e3dImage border[STORAGESTATUS0001_ITEMKIND + 1];
	struct e3dImage moneyWindow;
	struct e3dImage moneyLabel;

	struct{
		struct partsButton btn;
		struct e3dImage box;
		struct e3dImage image;
		struct e3dImage name;
		struct e3dImage num;
		bool inactive;
	} item[STORAGESTATUS0001_ITEMKIND];

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	int result;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct partsPopupCartridgeItemSelect *this, unsigned int active){
	// 項目ボタン
	for(int i = 0; i < STORAGESTATUS0001_ITEMKIND; i++){
		partsButtonInit(&this->item[i].btn);
		this->item[i].inactive = !((active & (1 << i)) && (storageStatusGet()->itemNum[i] > 0));
	}
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct partsPopupCartridgeItemSelect *this){
	// 体力回復アイテムの使用条件
	int itemKind = DATAITEMKIND_HEALTH;
	bool inactive = this->item[itemKind].inactive;
	if(!inactive){
		int health = storageStatusGet()->health;
		int healthMax = parameterPlayerHealthMax();
		inactive = (health >= healthMax);
	}
	this->item[itemKind].btn.inactive = inactive;

	// テンション上昇アイテムの使用条件
	itemKind = DATAITEMKIND_TENSION;
	inactive = this->item[itemKind].inactive;
	if(!inactive){
		inactive = storageStatusGet()->isTension;
	}
	this->item[itemKind].btn.inactive = inactive;

	// エネミー移動アイテムの使用条件
	itemKind = DATAITEMKIND_MONSTERMOVE;
	inactive = this->item[itemKind].inactive;
	if(!inactive){
		inactive = true;
		struct storageFieldList *storageField = &storageFieldGet()->list[storageStatusGet()->fieldKind];
		for(int i = 0; i < STORAGEFIELD0001_ENEMYMAX; i++){if(storageField->enemy[i].enemyKind >= 0){inactive = false; break;}}
	}
	this->item[itemKind].btn.inactive = inactive;

	// キャンバス充填アイテムの使用条件
	itemKind = DATAITEMKIND_CANVASFULL;
	inactive = this->item[itemKind].inactive;
	if(!inactive){
		bool isPuppetFull = true;
		bool isMeisterEmpty = true;
		for(int i = 0; i < storageAtelierGet()->canvasSize; i++){for(int j = 0; j < STORAGEATELIER0001_CANVASYMAX; j++){if(storageAtelierGet()->canvas[i][j].puppetKind < 0){isPuppetFull = false;}}}
		for(int i = 0; i < storageAtelierGet()->workspaceSize; i++){if(storageAtelierGet()->workspace[i].meisterKind >= 0){isMeisterEmpty = false; break;}}
		inactive = isPuppetFull || isMeisterEmpty;
	}
	this->item[itemKind].btn.inactive = inactive;

	// キャンバス拡張アイテムの使用条件
	itemKind = DATAITEMKIND_CANVASOPEN;
	inactive = this->item[itemKind].inactive;
	if(!inactive){inactive = storageAtelierGet()->canvasSize >= STORAGEATELIER0001_CANVASXMAX;}
	this->item[itemKind].btn.inactive = inactive;

	// ワークスペース拡張アイテムの使用条件
	itemKind = DATAITEMKIND_WORKSPACEOPEN;
	inactive = this->item[itemKind].inactive;
	if(!inactive){inactive = storageAtelierGet()->workspaceSize >= STORAGEATELIER0001_WORKSPACEXMAX;}
	this->item[itemKind].btn.inactive = inactive;
}

// 主計算
static int popupCalcMain(struct partsPopupCartridgeItemSelect *this){
	// タッチ処理
	for(int i = 0; i < STORAGESTATUS0001_ITEMKIND; i++){partsButtonBoxCalc(&this->item[i].btn);}
	partsButtonBoxCalc(&this->btnClose);
	partsButtonCalc(&this->btnOuter);

	// 選択ボタン押下確認
	for(int i = 0; i < STORAGESTATUS0001_ITEMKIND; i++){
		if(this->item[i].btn.trigger){
			this->item[i].btn.trigger = false;
			soundSePlayOK();
			// ポップアップ閉じる
			this->result = i;
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
static void createBuffer(struct partsPopupCartridgeItemSelect *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;
			
		int sw = 165;
		int sh = 40;
		int ww = sw + 6;
		int wh = 33 + (sh + 2) * STORAGESTATUS0001_ITEMKIND + 2 + 43;
		int wx = (global.screen.w - ww) * 0.5;
		int wy = (global.screen.h - wh) * 0.5;

		// 項目ボタン位置
		for(int i = 0; i < STORAGESTATUS0001_ITEMKIND; i++){partsButtonPosition(&this->item[i].btn, wx + 3, wy + 33 + 2 + (sh + 2) * i, sw, sh);}
		// ウインドウ外ボタン位置
		partsButtonPosition(&this->btnOuter, wx, wy, ww, wh);

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ウインドウとタイトル作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->window, wx, wy, ww, wh);
		e3dImageTextCreateArray(&this->labelTitle, wx + ww * 0.5, wy + 3 + 30 * 0.5, "アイテム", 1.0, 0, 0);
		// お金ウインドウ作成
		char str[32];
		e3dImageBoxDefaultCreateArray(&this->moneyWindow, 5, global.screen.h - 5 - 31, 140 + 16, 15 + 16);
		sprintf(str, "おかね%9dゲソ", storageStatusGet()->money);
		e3dImageTextCreateArray(&this->moneyLabel, 5 + 8, global.screen.h - 5 - 8 - 15, str, 1.0, 1, 1);

		// 各選択項目作成
		char *nameList[STORAGESTATUS0001_ITEMKIND] = {ITEMNAME_HEALTH, ITEMNAME_TENSION, ITEMNAME_MONSTERMOVE, ITEMNAME_CANVASFULL, ITEMNAME_CANVASOPEN, ITEMNAME_WORKSPACEOPEN};
		for(int i = 0; i < STORAGESTATUS0001_ITEMKIND; i++){
			char num[16];
			sprintf(num, "%dコ", storageStatusGet()->itemNum[i]);
			int sx = this->item[i].btn.x;
			int sy = this->item[i].btn.y;
			int borderOffset = (i == 0) ? 0 : 10;
			e3dImageWhiteCreateArray(&this->border[i], sx + borderOffset, sy - 2, sw - borderOffset * 2, 2);
			e3dImageWhiteCreateArray(&this->item[i].box, sx, sy, sw, sh);
			e3dImageCreateArray(&this->item[i].image, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, sx + 5, sy + 5, 30, 30, TEXPOS_SYSTEM_IMAGEITEM_X + (TEXPOS_SYSTEM_IMAGEITEM_W + TEXPOS_SPACE) * i, TEXPOS_SYSTEM_IMAGEITEM_Y, TEXPOS_SYSTEM_IMAGEITEM_W, TEXPOS_SYSTEM_IMAGEITEM_H);
			e3dImageTextCreateArray(&this->item[i].name, sx + 40, sy + 5, nameList[i], 1.0, 1, 1);
			e3dImageTextCreateArray(&this->item[i].num, sx + sw - 5, sy + 20, num, 1.0, -1, 1);
		}
		// 最後のボーター作成
		e3dImageWhiteCreateArray(&this->border[STORAGESTATUS0001_ITEMKIND], wx + 3, wy + 33 + (sh + 2) * STORAGESTATUS0001_ITEMKIND, sw, 2);

		// 閉じるボタン作成
		partsButtonBoxCreateArray(&this->btnClose, wx + (ww - 60) * 0.5, wy + wh - 3 - 5 - 30, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct partsPopupCartridgeItemSelect *this){
	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempmat1;
	double num = (double)this->super.openStep / this->super.openStepMax;
	double alpha = 1 - num * num;
	bool isOpening = (this->super.openStep < 0 || this->super.openStepMax <= this->super.openStep);
	bool isClosing = (0 < this->super.openStep && this->super.openStep < this->super.openStepMax);
	double alphaBack = (isClosing && this->result >= 0) ? 1.0 : alpha;

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
	e3dImageDraw(&this->window);
	e3dImageDraw(&this->moneyWindow);
	// ラベル描画
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->moneyLabel);

	// 各項目描画
	for(int i = 0; i < STORAGESTATUS0001_ITEMKIND; i++){
		if(this->item[i].btn.active){
			engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * 0.5);
			e3dImageDraw(&this->item[i].box);
		}
		if(this->item[i].btn.inactive){engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha * 0.5);}else{engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);}
		e3dImageDraw(&this->item[i].image);
		if(this->item[i].btn.inactive){engineGraphicEngineSetColorRgba(0.5, 0.5, 0.5, alpha);}else{engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);}
		e3dImageDraw(&this->item[i].name);
		e3dImageDraw(&this->item[i].num);
	}

	// タイトルと区切り線描画
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->labelTitle);
	for(int i = 0; i < STORAGESTATUS0001_ITEMKIND; i++){e3dImageDraw(&this->border[i]);}
	e3dImageDraw(&this->border[STORAGESTATUS0001_ITEMKIND]);

	// 閉じるボタン描画
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct partsPopupCartridgeItemSelect *this){
	return this->result;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct partsPopupCartridgeItemSelect *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// アイテム選択ポップアップカートリッジ初期化
struct popupCartridge *partsPopupItemSelect(unsigned int active){
	struct partsPopupCartridgeItemSelect *this = (struct partsPopupCartridgeItemSelect*)engineUtilMemoryCalloc(1, sizeof(struct partsPopupCartridgeItemSelect));
	init(this, active);

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

