#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ポップアップカートリッジ構造体
struct partsPopupCartridgePuppetProfile{
	struct popupCartridge super;

	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage window;
	struct e3dImage image;
	struct e3dImage labelData;
	struct e3dImage labelName;
	struct e3dImage labelText;
	struct e3dNumber labelPower;

	int charax;
	int charay;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	int puppetKind;
	int puppetPower;
	enum dataAttributionKind attributionKind;
	int step;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct partsPopupCartridgePuppetProfile *this){
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct partsPopupCartridgePuppetProfile *this){
}

// 主計算
static int popupCalcMain(struct partsPopupCartridgePuppetProfile *this){
	// タッチ処理
	partsButtonBoxCalc(&this->btnClose);
	partsButtonCalc(&this->btnOuter);

	// ステップ進行
	this->step++;

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
static void createBuffer(struct partsPopupCartridgePuppetProfile *this){
	// 状態確認
	signed char puppetNum = storagePuppetGet()->list[this->puppetKind].num;
	unsigned char puppetLevel = storagePuppetGet()->list[this->puppetKind].level;
	int status = 1 | ((this->puppetKind & 0x7fff) << 1) | ((puppetNum & 0xff) << 16) | ((puppetLevel & 0xff) << 24);
	struct dataPuppet *dataPuppet = dataPuppetGet(this->puppetKind, NULL);
	if(dataPuppet == NULL){
		// ロード中
		status = -1;
	}
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		int ww = 300;
		int wh = 238;
		int wx = (global.screen.w - ww) * 0.5;
		int wy = (global.screen.h - wh) * 0.5;

		// ウインドウ外ボタン位置
		partsButtonPosition(&this->btnOuter, wx, wy, ww, wh);
		// ハコニワキャラクタ位置
		this->charax = wx + 120;
		this->charay = wy + 220;

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ウインドウ作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->window, wx, wy, ww, wh);

		// 閉じるボタン作成
		partsButtonBoxCreateArray(&this->btnClose, wx + ww - 3 - 5 - 60, wy + wh - 3 - 5 - 30, 60, 30, "とじる", 1.0);

		if(this->bufferStatus > 0){
			// パワー計算
			double levelRatio = (storagePuppetGet()->list[this->puppetKind].level * 0.01) * (dataPuppet->levelRatio10 * 0.1 - 1);
			this->puppetPower = dataPuppet->attackMain * (1 + levelRatio);
			// パワー桁数確認
			int digit = 0;
			int number = this->puppetPower;
			while(number > 0){digit++; number = number / 10;}
			// 属性記憶
			this->attributionKind = dataPuppet->attributionKind;

			char data[32];
			sprintf(data, "Lv.%d\n%*sパワー\n%dたい", puppetLevel, digit, "", puppetNum);
			e3dImageCreateArray(&this->image, TEXSIZ_PUPPET_W, TEXSIZ_PUPPET_H, wx + 10, wy + 10, 120, 160, TEXPOS_PUPPET_PROFILE_X, TEXPOS_PUPPET_PROFILE_Y, TEXPOS_PUPPET_PROFILE_W, TEXPOS_PUPPET_PROFILE_H);
			e3dImageTextCreateArray(&this->labelData, wx + 10, wy + wh - 10 - 45, data, 1.0, 1, 1);
			e3dImageTextCreateArray(&this->labelName, wx + 140, wy + 10, dataPuppet->fullname, 1.0, 1, 1);
			e3dImageTextCreateArray(&this->labelText, wx + 140, wy + 40, dataPuppet->profile, 1.0, 1, 1);
			e3dNumberOutlineCreateArray(&this->labelPower, wx + 10, wy + wh - 10 - 30, 1.0, 1, 1);
		}

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct partsPopupCartridgePuppetProfile *this){
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
	// 閉じるボタン描画
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);

	if(this->bufferStatus > 0){
		// ラベル描画
		engineGraphicEngineSetMatrix(&tempmat1);
		engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
		e3dImageDraw(&this->labelData);
		e3dImageDraw(&this->labelName);
		e3dImageDraw(&this->labelText);
		// パワーラベル描画
		switch(this->attributionKind){
			case DATAATTRIBUTIONKIND_WHITE: engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha); break;
			case DATAATTRIBUTIONKIND_RED: engineGraphicEngineSetColorRgba(1.0, 0.0, 0.0, alpha); break;
			case DATAATTRIBUTIONKIND_GREEN: engineGraphicEngineSetColorRgba(0.0, 1.0, 0.0, alpha); break;
			case DATAATTRIBUTIONKIND_BLUE: engineGraphicEngineSetColorRgba(0.0, 0.0, 1.0, alpha); break;
			case DATAATTRIBUTIONKIND_YELLOW: engineGraphicEngineSetColorRgba(1.0, 1.0, 0.0, alpha); break;
		}
		double powerNum = (double)this->step / 60;
		if(powerNum > 1){powerNum = 1;}
		e3dNumberDraw(&this->labelPower, &tempmat1, (int)(this->puppetPower * powerNum));
		// パペット画像描画
		int e3dIdPuppetImage;
		struct dataPuppet *dataPuppet = dataPuppetGet(this->puppetKind, &e3dIdPuppetImage);
		engineGraphicEngineBindTexture(e3dIdPuppetImage);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
		e3dImageDraw(&this->image);

		// ハコニワキャラクタ行列作成
		double rotv = 3.1415 / 180 *  0;
		double roth = 3.1415 / 180 * 30;
		engineMathMat4Translate(&tempmat1, this->charax, this->charay, 0);
		engineMathMat4Scale(&tempmat1, 1.0, -1.0, 0.01);
		engineMathMat4RotateX(&tempmat1, roth);
		engineMathMat4RotateY(&tempmat1, rotv);
		// ハコニワキャラクタ描画
		engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_NORMAL);
		engineGraphicEngineClearDepth();
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
		hknwCharaListBind();
		struct hknwChara *chara = hknwCharaListGet(dataPuppet->charaKind);
		hknwCharaDrawChara(chara, &tempmat1, rotv, roth, 0, 0, 0, 3.1415 / 180 * (90 + this->step), 50.0, HKNWCHARAPOSESID_STAND, 0);
		engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
	}
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct partsPopupCartridgePuppetProfile *this){
	return 0;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct partsPopupCartridgePuppetProfile *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *partsPopupPuppetProfile(int puppetKind){
	struct partsPopupCartridgePuppetProfile *this = (struct partsPopupCartridgePuppetProfile*)engineUtilMemoryCalloc(1, sizeof(struct partsPopupCartridgePuppetProfile));
	this->puppetKind = puppetKind;
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

