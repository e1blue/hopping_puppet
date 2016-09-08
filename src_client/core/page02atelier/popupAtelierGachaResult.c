#include "../library.h"
#include "../includeAll.h"
#include "pageAtelier.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ポップアップカートリッジ構造体
struct popupCartridgeAtelierGachaResult{
	struct popupCartridge super;
	struct pageAtelierStatus *stat;

	struct partsButton btnSkip;
	struct partsButtonBox btnClose;

	struct e3dImage back;
	struct e3dImage messageWindow;
	struct e3dImage messageLabel;
	struct e3dImage messageName;
	struct e3dImage meisterWindow;
	struct e3dImage meisterBackImage;
	struct e3dImage meisterFrontImage;
	struct e3dImage meisterFrontLabelName;
	struct e3dImage meisterFrontLabelLevel;
	struct e3dImage effect;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	int meisterKind;
	int typeNew;
	int step;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct popupCartridgeAtelierGachaResult *this){
	// スキップボタン
	partsButtonInit(&this->btnSkip);
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct popupCartridgeAtelierGachaResult *this){
}

// 主計算
static int popupCalcMain(struct popupCartridgeAtelierGachaResult *this){
	// タッチ処理
	if(this->step < 30){partsButtonCalc(&this->btnSkip);}
	else{partsButtonBoxCalc(&this->btnClose);}

	// 効果音
	if(this->step == 35){soundSePlayGacha();}

	// スキップ閉じる戻るボタン押下確認
	if(this->btnSkip.trigger || this->btnClose.super.trigger || global.key.bk.triggerInactive){
		this->btnSkip.trigger = false;
		this->btnClose.super.trigger = false;
		global.key.bk.triggerInactive = false;
		if(this->step < 60){
			if(this->step < 35){soundSePlayGacha();}
			this->step = 60;
			return 0;
		}else{
			soundSePlayOK();
			// ポップアップ閉じる
			return 1;
		}
	}

	this->step++;
	return 0; 
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct popupCartridgeAtelierGachaResult *this){
	// 状態確認
	struct dataMeister *dataMeister = dataMeisterGet(this->meisterKind, NULL);
	int status = 1 | ((this->meisterKind & 0x7fff) << 1);
	if(dataMeister == NULL){status = -1;}
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// スキップボタン位置
		partsButtonPosition(&this->btnSkip, 0, 0, global.screen.w, global.screen.h);

		// バッファ作成開始
		engineGraphicBufferBegin();

		// メッセージ作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->messageWindow, (global.screen.w - 186) * 0.5, 20, 186, 46);
		if(this->typeNew > 0){e3dImageTextCreateArray(&this->messageLabel, global.screen.w * 0.5, 20 + 3 + 5, "マイスターを てにいれた", 1.0, 0, 1);}
		else if(this->typeNew < 0){e3dImageTextCreateArray(&this->messageLabel, global.screen.w * 0.5, 20 + 3 + 5, "マイスターが ダブった", 1.0, 0, 1);}
		else{e3dImageTextCreateArray(&this->messageLabel, global.screen.w * 0.5, 20 + 3 + 5, "マイスターの レベルが あがった", 1.0, 0, 1);}
		// マイスターカード作成
		int mw = 70;
		int mh = 98;
		int mx = (global.screen.w - mw) * 0.5;
		int my = (global.screen.h - mh) * 0.5;
		e3dImageBoxDefaultCreateArray(&this->meisterWindow, mx, my, mw, mh);
		e3dImageCreateArray(&this->meisterBackImage, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, mx + 3, my + 3, mw - 6, mh - 6, TEXPOS_SYSTEM_IMAGECARD_X, TEXPOS_SYSTEM_IMAGECARD_Y, TEXPOS_SYSTEM_IMAGECARD_W, TEXPOS_SYSTEM_IMAGECARD_H);

		if(this->bufferStatus > 0){
			char level[16];
			sprintf(level, "Lv.%d", storageMeisterGet()->list[this->meisterKind].level);
			// ラベル
			e3dImageTextCreateArray(&this->messageName, global.screen.w * 0.5, 20 + 3 + 20, dataMeister->fullname, 1.0, 0, 1);
			e3dImageTextCreateArray(&this->meisterFrontLabelName, mx + mw * 0.5, my + 8 + 20 * 0.5, dataMeister->name, 1.0, 0, 0);
			e3dImageTextCreateArray(&this->meisterFrontLabelLevel, mx + mw * 0.5, my + mh - 8 - 20 * 0.5, level, 1.0, 0, 0);
			// イメージ
			e3dImageCreateArray(&this->meisterFrontImage, TEXSIZ_MEISTER_W, TEXSIZ_MEISTER_H, mx + 3, my + 3, mw - 6, mh - 6, TEXPOS_MEISTER_WORKSPACE_X, TEXPOS_MEISTER_WORKSPACE_Y, TEXPOS_MEISTER_WORKSPACE_W, TEXPOS_MEISTER_WORKSPACE_H);
		}

		// エフェクト作成
		int vertIndex = engineGraphicBufferVretIndexGet();
		int faceIndex = engineGraphicBufferFaceIndexGet();
		int tetraNum = (this->typeNew > 0) ? 32 : (this->typeNew < 0) ? 12 : 24;
		double r1 = (this->typeNew < 0) ? 100 : 300;
		double r2 = 50;
		for(int j = 0; j < tetraNum; j++){
			double t1 = 3.1415 * 2 * j / tetraNum - 0.1 * (0.2 + 0.8 * (engineMathRandomGet() % 100) / 100);
			double t2 = t1 + ((this->typeNew > 0) ? 0.3 : 0.1) * (engineMathRandomGet() % 100) / 100;
			double c1 = engineMathCos(t1); double s1 = engineMathSin(t1);
			double c2 = engineMathCos(t2); double s2 = engineMathSin(t2);
			engineGraphicBufferPushTetraFace(engineGraphicBufferVretIndexGet());
			engineGraphicBufferPushVert(r1 * c1, r1 * s1, 0.0);
			engineGraphicBufferPushVert(r2 * c1, r2 * s1, 0.0);
			engineGraphicBufferPushVert(r2 * c2, r2 * s2, 0.0);
			engineGraphicBufferPushVert(r1 * c2, r1 * s2, 0.0);
			engineGraphicBufferPushTetraTexc(TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, 16, 16, 0, 0);
		}
		this->effect.faceIndex = faceIndex;
		this->effect.faceNum = tetraNum * 2;

		// 閉じるボタン
		partsButtonBoxCreateArray(&this->btnClose, (global.screen.w - 120) * 0.5, global.screen.h - 60 - 20, 120, 60, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct popupCartridgeAtelierGachaResult *this){
	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempmat1;
	struct engineMathMatrix44 tempmat2;
	double num = (double)this->super.openStep / this->super.openStepMax;
	double alpha = 1 - num * num;
	bool isOpening = (this->super.openStep < 0 || this->super.openStepMax <= this->super.openStep);
	bool isClosing = (0 < this->super.openStep && this->super.openStep < this->super.openStepMax);
	double alphaBack = isOpening ? 1.0 : alpha;

	double alphaAppear1 = (double)(this->step - 30) / 30; if(alphaAppear1 < 0){alphaAppear1 = 0;}else if(alphaAppear1 > 1){alphaAppear1 = 1;}

	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);

	// 背景半透明幕描画
	engineMathMat4Identity(&tempmat1);
	engineGraphicEngineSetMatrix(&tempmat1);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alphaBack * 0.5);
	e3dImageDraw(&this->back);

	// ポップアップ展開行列作成
	engineMathMat4Copy(&tempmat1, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat1, global.screen.w * 0.5, global.screen.h * 0.5, 0);
	engineMathMat4Scale(&tempmat1, 1 + 0.5 * num * num, 1 + 0.5 * num * num, 1.0);
	engineMathMat4Translate(&tempmat1, -global.screen.w * 0.5, -global.screen.h * 0.5, 0);

	// 背景エフェクト描画
	engineMathMat4Copy(&tempmat2, &tempmat1);
	engineMathMat4Translate(&tempmat2, global.screen.w * 0.5, global.screen.h * 0.5, 0);
	engineMathMat4RotateZ(&tempmat2, this->step * 0.01);
	engineGraphicEngineSetMatrix(&tempmat2);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 0.5 * alpha * alphaAppear1);
	e3dImageDraw(&this->effect);

	// メッセージ描画
	engineGraphicEngineSetMatrix(&tempmat1);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha * alphaAppear1);
	e3dImageDraw(&this->messageWindow);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * alphaAppear1);
	e3dImageDraw(&this->messageLabel);
	e3dImageDraw(&this->messageName);

	// ボタン描画
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha * alphaAppear1);

	// マイスターカード描画
	int turnStep = 60;
	if(this->step < turnStep){
		// くるりんちょ
		engineMathMat4Copy(&tempmat2, &tempmat1);
		engineMathMat4Translate(&tempmat2, global.screen.w * 0.5, global.screen.h * 0.5, 0);
		engineMathMat4Scale(&tempmat2, engineMathAbs(engineMathCos(3.1415 * this->step / turnStep)), 1.0, 1.0);
		engineMathMat4Translate(&tempmat2, -global.screen.w * 0.5, -global.screen.h * 0.5, 0);
		engineGraphicEngineSetMatrix(&tempmat2);
	}else{engineGraphicEngineSetMatrix(&tempmat1);}
	// ウインドウ描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	e3dImageDraw(&this->meisterWindow);
	if(this->step < turnStep * 0.5){
		e3dImageDraw(&this->meisterBackImage);
	}else if(this->bufferStatus > 0){
		int e3dIdMeisterImage;
		struct dataMeister *dataMeister = dataMeisterGet(this->meisterKind, &e3dIdMeisterImage);
		// イメージ描画
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha * 0.4);
		engineGraphicEngineBindTexture(e3dIdMeisterImage);
		e3dImageDraw(&this->meisterFrontImage);
		engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
		// ラベル描画
		engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
		e3dImageDraw(&this->meisterFrontLabelName);
		e3dImageDraw(&this->meisterFrontLabelLevel);
	}
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct popupCartridgeAtelierGachaResult *this){
	return 0;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct popupCartridgeAtelierGachaResult *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *popupAtelierGachaResult(struct pageAtelierStatus *stat, int meisterKind, int typeNew){
	struct popupCartridgeAtelierGachaResult *this = (struct popupCartridgeAtelierGachaResult*)engineUtilMemoryCalloc(1, sizeof(struct popupCartridgeAtelierGachaResult));
	this->stat = stat;
	this->meisterKind = meisterKind;
	this->typeNew = typeNew;
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

