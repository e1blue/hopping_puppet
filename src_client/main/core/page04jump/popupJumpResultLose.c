#include "../includeAll.h"
#include "pageJump.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#define POPUPJUMPRESULTLOSE_PUPPETNUM 2

// ポップアップカートリッジ構造体
struct popupCartridgeJumpResultLose{
	struct popupCartridge super;
	struct pageJumpStatus *stat;

	struct partsButton btnSkip;
	struct partsButtonBox btnClose;

	struct e3dImage back;
	struct e3dImage messageWindow;
	struct e3dImage messageLabel;
	struct e3dImage resultWindow;
	struct e3dImage resultLabel;
	struct e3dImage image;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	struct{
		int puppetKind;
		double x;
		double y;
	} puppetList[2];

	int step;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct popupCartridgeJumpResultLose *this){
	// スキップボタン
	partsButtonInit(&this->btnSkip);
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);

	// パペット初期化
	int index = 0;
	for(int i = 0; i < POPUPJUMPRESULTLOSE_PUPPETNUM; i++){this->puppetList[i].puppetKind = -1;}
	for(int i = 0; i < 256; i++){
		// パペットタイプ選出
		int puppetKind = engineMathRandomGet() % STORAGEPUPPET0001_KIND;
		int puppetNum = storagePuppetGet()->list[puppetKind].num;
		if(puppetNum <= 0){continue;}
		for(int j = 0; j < POPUPJUMPRESULTLOSE_PUPPETNUM; j++){if(puppetKind == this->puppetList[j].puppetKind){puppetNum--;}}
		if(puppetNum <= 0){continue;}
		// パペット作成
		this->puppetList[index].puppetKind = puppetKind;
		this->puppetList[index].x = (signed int)(20 + engineMathRandomGet() % 50) * ((index == 0) ? 1 : -1);
		this->puppetList[index].y = 70;
		if(++index >= POPUPJUMPRESULTLOSE_PUPPETNUM){break;}
	}
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct popupCartridgeJumpResultLose *this){
}

// 主計算
static int popupCalcMain(struct popupCartridgeJumpResultLose *this){
	// タッチ処理
	bool isSkipable = (this->step < 120);
	if(isSkipable){partsButtonCalc(&this->btnSkip);}
	if(!isSkipable){partsButtonBoxCalc(&this->btnClose);}

	// 効果音
	if(this->step == 85){soundSePlayPenalty();}

	// スキップ閉じる戻るボタン押下確認
	if(this->btnSkip.trigger || this->btnClose.super.trigger || global.key.bk.triggerInactive){
		this->btnSkip.trigger = false;
		this->btnClose.super.trigger = false;
		global.key.bk.triggerInactive = false;
		if(isSkipable){
			if(this->step < 85){soundSePlayPenalty();}
			this->step = 150;
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
static void createBuffer(struct popupCartridgeJumpResultLose *this){
	// 状態確認
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// スキップボタン位置
		partsButtonPosition(&this->btnSkip, 0, 0, global.screen.w, global.screen.h);

		// バッファ作成開始
		engineGraphicBufferBegin();

		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		// メッセージ作成
		e3dImageBoxDefaultCreateArray(&this->messageWindow, (global.screen.w - 166) * 0.5, 20, 166, 31);
		e3dImageTextCreateArray(&this->messageLabel, global.screen.w * 0.5, 20 + 3 + 5, "ハイボクした...", 1.0, 0, 1);
		e3dImageBoxDefaultCreateArray(&this->resultWindow, (global.screen.w - 176) * 0.5, (global.screen.h - 61) * 0.5 + 80, 176, 61);
		e3dImageTextCreateArray(&this->resultLabel, global.screen.w * 0.5, (global.screen.h - 61) * 0.5 + 80 + 3 + 5, "ペナルティ\nタイリョクが かんぜんに\nかいふくするまで たたかえません", 1.0, 0, 1);
		// プレイヤー画像
		e3dImageCreateArray(&this->image, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, (global.screen.w - 120) * 0.5, (global.screen.h - 120) * 0.5 - 80, 120, 120, TEXPOS_SYSTEM_IMAGERESULT_X + (TEXPOS_SYSTEM_IMAGERESULT_W + TEXPOS_SPACE) * 1, TEXPOS_SYSTEM_IMAGERESULT_Y, TEXPOS_SYSTEM_IMAGERESULT_W, TEXPOS_SYSTEM_IMAGERESULT_H);

		// 閉じるボタン
		partsButtonBoxCreateArray(&this->btnClose, (global.screen.w - 120) * 0.5, global.screen.h - 60 - 5, 120, 60, "にげる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct popupCartridgeJumpResultLose *this){
	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempmat1;
	struct engineMathMatrix44 tempmat2;
	double num = (double)this->super.openStep / this->super.openStepMax;
	double alpha = 1 - num * num;
	bool isOpening = (this->super.openStep < 0 || this->super.openStepMax <= this->super.openStep);
	bool isClosing = (0 < this->super.openStep && this->super.openStep < this->super.openStepMax);
	double alphaBack = isClosing ? 1.0 : alpha;

	double alphaAppear1 = (double)(this->step -  60) / 30; if(alphaAppear1 < 0){alphaAppear1 = 0;}else if(alphaAppear1 > 1){alphaAppear1 = 1;}
	double alphaAppear2 = (double)(this->step - 120) / 30; if(alphaAppear2 < 0){alphaAppear2 = 0;}else if(alphaAppear2 > 1){alphaAppear2 = 1;}

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
	engineGraphicEngineSetMatrix(&tempmat1);
	// いろいろ描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	e3dImageDraw(&this->messageWindow);
	e3dImageDraw(&this->image);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->messageLabel);
	// 結果ウインドウ描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha * alphaAppear1);
	e3dImageDraw(&this->resultWindow);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * alphaAppear1);
	e3dImageDraw(&this->resultLabel);

	// ボタン描画
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha * alphaAppear2);

	// パペット描画
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_NORMAL);
	engineGraphicEngineClearDepth();
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	hknwCharaListBind();
	for(int i = 0; i < POPUPJUMPRESULTLOSE_PUPPETNUM; i++){
		if(this->puppetList[i].puppetKind < 0){continue;}
		int e3dIdPuppetImage;
		struct dataPuppet *dataPuppet = dataPuppetGet(this->puppetList[i].puppetKind, &e3dIdPuppetImage);
		engineGraphicEngineBindTexture(e3dIdPuppetImage);
		// ハコニワキャラクタ行列作成
		double rotv = 3.1415 / 180 *  0;
		double roth = 3.1415 / 180 * 30;
		engineMathMat4Copy(&tempmat2, &tempmat1);
		engineMathMat4Translate(&tempmat2, global.screen.w * 0.5 + this->puppetList[i].x, global.screen.h * 0.5 - 80 + this->puppetList[i].y, 0);
		engineMathMat4Scale(&tempmat2, 1.0, -1.0, 0.01);
		engineMathMat4RotateX(&tempmat2, roth);
		engineMathMat4RotateY(&tempmat2, rotv);
		// ハコニワキャラクタ描画
		struct hknwChara *chara = hknwCharaListGet(dataPuppet->charaKind);
		hknwCharaDrawChara(chara, &tempmat2, rotv, roth, 0, 0, 0, 3.1415 / 180 * (270 - 45 * (double)this->puppetList[i].x / 60), 50.0, HKNWCHARAPOSESID_STAND, 0);
	}
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct popupCartridgeJumpResultLose *this){
	return 0;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct popupCartridgeJumpResultLose *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *popupJumpResultLose(struct pageJumpStatus *stat){
	struct popupCartridgeJumpResultLose *this = (struct popupCartridgeJumpResultLose*)engineUtilMemoryCalloc(1, sizeof(struct popupCartridgeJumpResultLose));
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

