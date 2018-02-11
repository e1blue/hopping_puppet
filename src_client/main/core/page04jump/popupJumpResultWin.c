#include "../includeAll.h"
#include "pageJump.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#define POPUPJUMPRESULTWIN_PUPPETNUM 2

// ポップアップカートリッジ構造体
struct popupCartridgeJumpResultWin{
	struct popupCartridge super;
	struct pageJumpStatus *stat;
	struct popupCartridge *popupTutorial;
	int stepTutorial01;

	struct partsButton btnSkip;
	struct partsButtonBox btnClose;

	struct e3dImage back;
	struct e3dImage messageWindow;
	struct e3dImage messageLabel;
	struct e3dImage messageName;
	struct e3dImage statusWindow;
	struct e3dImage levelImageUp;
	struct e3dImage levelLabelBefore;
	struct e3dImage levelLabelAfter;
	struct e3dImage moneyLabelBefore;
	struct e3dImage moneyLabelAfter;
	struct e3dImage resultWindow;
	struct e3dImage resultLabel;
	struct e3dImage resultExperience;
	struct e3dImage resultMoney;
	struct e3dImage itemWindow;
	struct e3dImage itemImage;
	struct e3dImage itemName;
	struct e3dImage itemLabel;
	struct e3dImage image;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	// にぎやかしパペットデータ
	struct{
		int puppetKind;
		double x;
		double y;
	} puppetList[2];

	int step;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct popupCartridgeJumpResultWin *this){
	// スキップボタン
	partsButtonInit(&this->btnSkip);
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);

	// パペット初期化
	int index = 0;
	for(int i = 0; i < POPUPJUMPRESULTWIN_PUPPETNUM; i++){this->puppetList[i].puppetKind = -1;}
	for(int i = 0; i < 256; i++){
		// パペットタイプ選出
		int puppetKind = engineMathRandomGet() % STORAGEPUPPET0001_KIND;
		int puppetNum = storagePuppetGet()->list[puppetKind].num;
		if(puppetNum <= 0){continue;}
		for(int j = 0; j < POPUPJUMPRESULTWIN_PUPPETNUM; j++){if(puppetKind == this->puppetList[j].puppetKind){puppetNum--;}}
		if(puppetNum <= 0){continue;}
		// パペット作成
		this->puppetList[index].puppetKind = puppetKind;
		this->puppetList[index].x = (signed int)(20 + engineMathRandomGet() % 35) * ((index == 0) ? 1 : -1);
		this->puppetList[index].y = 70;
		if(++index >= POPUPJUMPRESULTWIN_PUPPETNUM){break;}
	}

}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct popupCartridgeJumpResultWin *this){
}

// 主計算
static int popupCalcMain(struct popupCartridgeJumpResultWin *this){
	if(this->popupTutorial != NULL){
		// チュートリアルポップアップ計算
		if(!this->popupTutorial->calc(this->popupTutorial)){return 0;}
		// チュートリアルポップアップ完了時処理
		if(this->popupTutorial->result(this->popupTutorial)){}
		this->popupTutorial->dispose(this->popupTutorial);
		this->popupTutorial = NULL;
	}else if(!storageStatusGet()->tutorial.jump03){
		// チュートリアル計算
		if(this->stepTutorial01 == 30){soundSePlayOK(); this->popupTutorial = partsPopupTutorialJump03();}
		if(this->stepTutorial01 == 30){storageStatusGet()->tutorial.jump03 = true; storageStatusUpdate();}
		this->stepTutorial01++;
		return 0;
	}

	// タッチ処理
	bool isItem = (this->stat->resultData.itemKind >= 0);
	bool isSkipable = (this->step < (isItem ? 300 : 240));
	if(isSkipable){partsButtonCalc(&this->btnSkip);}
	if(!isSkipable){partsButtonBoxCalc(&this->btnClose);}

	// 効果音
	if(this->step == 175){soundSePlayGet();}
	if(this->step == 205){soundSePlayGet();}
	if(isItem && this->step == 275){soundSePlayGet();}

	// スキップ閉じる戻るボタン押下確認
	if(this->btnSkip.trigger || this->btnClose.super.trigger || global.key.bk.triggerInactive){
		this->btnSkip.trigger = false;
		this->btnClose.super.trigger = false;
		global.key.bk.triggerInactive = false;
		if(isSkipable){
			if(this->step < (isItem ? 275 : 205)){soundSePlayGet();}
			this->step = 330 + (30 + (this->step % 60));
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
static void createBuffer(struct popupCartridgeJumpResultWin *this){
	// 状態確認
	struct dataEnemy *dataEnemy = dataEnemyGet(this->stat->resultData.enemyKind, NULL);
	int status = 1 | ((this->stat->resultData.enemyKind & 0x7fff) << 1);
	if(dataEnemy == NULL){status = -1;}
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// スキップボタン位置
		partsButtonPosition(&this->btnSkip, 0, 0, global.screen.w, global.screen.h);

		// バッファ作成開始
		engineGraphicBufferBegin();

		char str[96];
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		// メッセージ作成
		e3dImageBoxDefaultCreateArray(&this->messageWindow, (global.screen.w - 166) * 0.5, 20, 166, 46);
		e3dImageTextCreateArray(&this->messageLabel, global.screen.w * 0.5, 20 + 3 + 5, "モンスターに ショウリっ!!", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->messageName, global.screen.w * 0.5, 20 + 3 + 20, (dataEnemy == NULL) ? "" : dataEnemy->fullname, 1.0, 0, 1);
		int sw = 196;
		int sh = 46;
		int sx = (global.screen.w - sw) * 0.5;
		int sy = (global.screen.h - sh) * 0.5 + 20;
		e3dImageBoxDefaultCreateArray(&this->statusWindow, sx, sy, sw, sh);
		e3dImageTextCreateArray(&this->levelImageUp, sx + 48, sy + 15, "LvUP!", 1.0, 0, 0);
		sprintf(str, "Lv.%-3dつぎまで%6dPt", this->stat->resultData.prevLevel, this->stat->resultData.prevExperience);
		e3dImageTextCreateArray(&this->levelLabelBefore, sx + sw * 0.5, sy + 3 + 5 + 15 * 0, str, 1.0, 0, 1);
		sprintf(str, "Lv.%-3dつぎまで%6dPt", this->stat->resultData.nextLevel, this->stat->resultData.nextExperience);
		e3dImageTextCreateArray(&this->levelLabelAfter, sx + sw * 0.5, sy + 3 + 5 + 15 * 0, str, 1.0, 0, 1);
		sprintf(str, "おかね%13dゲソ", this->stat->resultData.prevMoney);
		e3dImageTextCreateArray(&this->moneyLabelBefore, sx + sw * 0.5, sy + 3 + 5 + 15 * 1, str, 1.0, 0, 1);
		sprintf(str, "おかね%13dゲソ", this->stat->resultData.nextMoney);
		e3dImageTextCreateArray(&this->moneyLabelAfter, sx + sw * 0.5, sy + 3 + 5 + 15 * 1, str, 1.0, 0, 1);
		int rw = 156;
		int rh = 61;
		int rx = (global.screen.w - rw) * 0.5;
		int ry = (global.screen.h - rh) * 0.5 + 85;
		e3dImageBoxDefaultCreateArray(&this->resultWindow, rx, ry, rw, rh);
		e3dImageTextCreateArray(&this->resultLabel, rx + rw * 0.5, ry + 3 + 5 + 15 * 0, "しょうりボーナス", 1.0, 0, 1);
		sprintf(str, "けいけん%8dPt", this->stat->resultData.experience);
		e3dImageTextCreateArray(&this->resultExperience, rx + 3 + 5, ry + 3 + 5 + 15 * 1, str, 1.0, 1, 1);
		sprintf(str, "おかね%9dゲソ", this->stat->resultData.money);
		e3dImageTextCreateArray(&this->resultMoney, rx + 3 + 5, ry + 3 + 5 + 15 * 2, str, 1.0, 1, 1);
		if(this->stat->resultData.itemKind >= 0){
			// アイテムウインドウ作成
			int iw = 186;
			int ih = 46;
			int ix = (global.screen.w - iw) * 0.5;
			int iy = (global.screen.h - ih) * 0.5 + 145;
			char *nameList[STORAGESTATUS0001_ITEMKIND] = {ITEMNAME_HEALTH, ITEMNAME_TENSION, ITEMNAME_MONSTERMOVE, ITEMNAME_CANVASFULL, ITEMNAME_CANVASOPEN, ITEMNAME_WORKSPACEOPEN};
			e3dImageBoxDefaultCreateArray(&this->itemWindow, ix, iy, iw, ih);
			e3dImageCreateArray(&this->itemImage, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, ix + 3 + 5, iy + 3 + 5, 30, 30, TEXPOS_SYSTEM_IMAGEITEM_X + (TEXPOS_SYSTEM_IMAGEITEM_W + TEXPOS_SPACE) * this->stat->resultData.itemKind, TEXPOS_SYSTEM_IMAGEITEM_Y, TEXPOS_SYSTEM_IMAGEITEM_W, TEXPOS_SYSTEM_IMAGEITEM_H);
			sprintf(str, "アイテムを てにいれました\n%s", nameList[this->stat->resultData.itemKind]);
			e3dImageTextCreateArray(&this->itemLabel, ix + 3 + 5 + 30 + 5, iy + 3 + 5, str, 1.0, 1, 1);
		}
		// プレイヤー画像
		e3dImageCreateArray(&this->image, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, (global.screen.w - 120) * 0.5, (global.screen.h - 120) * 0.5 - 80, 120, 120, TEXPOS_SYSTEM_IMAGERESULT_X + (TEXPOS_SYSTEM_IMAGERESULT_W + TEXPOS_SPACE) * 0, TEXPOS_SYSTEM_IMAGERESULT_Y, TEXPOS_SYSTEM_IMAGERESULT_W, TEXPOS_SYSTEM_IMAGERESULT_H);

		// 閉じるボタン
		partsButtonBoxCreateArray(&this->btnClose, (global.screen.w - 120) * 0.5, global.screen.h - 60 - 5, 120, 60, "もどる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct popupCartridgeJumpResultWin *this){
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
	double alphaAppear3 = (double)(this->step - 150) / 30; if(alphaAppear3 < 0){alphaAppear3 = 0;}else if(alphaAppear3 > 1){alphaAppear3 = 1;}
	double alphaAppear4 = (double)(this->step - 180) / 30; if(alphaAppear4 < 0){alphaAppear4 = 0;}else if(alphaAppear4 > 1){alphaAppear4 = 1;}
	double alphaAppear5 = (double)(this->step - 240) / 30; if(alphaAppear5 < 0){alphaAppear5 = 0;}else if(alphaAppear5 > 1){alphaAppear5 = 1;}
	double alphaAppear6 = (double)(this->step - 300) / 30; if(alphaAppear6 < 0){alphaAppear6 = 0;}else if(alphaAppear6 > 1){alphaAppear6 = 1;}

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
	e3dImageDraw(&this->messageName);
	// 結果ウインドウ描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha * alphaAppear1);
	e3dImageDraw(&this->statusWindow);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * alphaAppear1 * (1 - alphaAppear3));
	e3dImageDraw(&this->levelLabelBefore);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * alphaAppear3);
	e3dImageDraw(&this->levelLabelAfter);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * alphaAppear1 * (1 - alphaAppear4));
	e3dImageDraw(&this->moneyLabelBefore);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * alphaAppear4);
	e3dImageDraw(&this->moneyLabelAfter);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha * alphaAppear2);
	e3dImageDraw(&this->resultWindow);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * alphaAppear2);
	e3dImageDraw(&this->resultLabel);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * alphaAppear3);
	e3dImageDraw(&this->resultExperience);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * alphaAppear4);
	e3dImageDraw(&this->resultMoney);
	if(this->stat->resultData.itemKind >= 0){
		// アイテム獲得ウインドウ描画
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha * alphaAppear5);
		e3dImageDraw(&this->itemWindow);
		e3dImageDraw(&this->itemImage);
		engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * alphaAppear5);
		e3dImageDraw(&this->itemLabel);
	}

	if(this->stat->resultData.prevLevel < this->stat->resultData.nextLevel){
		// レベルアップ演出
		engineMathMat4Copy(&tempmat2, &tempmat1);
		engineMathMat4Translate(&tempmat2, 0, -15 * alphaAppear3, 0);
		engineGraphicEngineSetMatrix(&tempmat2);
		engineGraphicEngineSetColorRgba(1.0, 0.0, 0.0, alpha * alphaAppear3);
		e3dImageDraw(&this->levelImageUp);
	}

	// ボタン描画
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha * ((this->stat->resultData.itemKind < 0) ? alphaAppear5 : alphaAppear6));

	// パペット描画
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_NORMAL);
	engineGraphicEngineClearDepth();
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	hknwCharaListBind();
	for(int i = 0; i < POPUPJUMPRESULTWIN_PUPPETNUM; i++){
		if(this->puppetList[i].puppetKind < 0){continue;}
		int e3dIdPuppetImage;
		struct dataPuppet *dataPuppet = dataPuppetGet(this->puppetList[i].puppetKind, &e3dIdPuppetImage);
		engineGraphicEngineBindTexture(e3dIdPuppetImage);
		// ハコニワキャラクタ行列作成
		double rotv = 3.1415 / 180 *  0;
		double roth = 3.1415 / 180 * 30;
		int jumpStep = (this->step + i * 30) % 60;
		double jump = (jumpStep > 20) ? 0 : 30 * engineMathSin(3.1415 * jumpStep / 20);
		engineMathMat4Copy(&tempmat2, &tempmat1);
		engineMathMat4Translate(&tempmat2, global.screen.w * 0.5 + this->puppetList[i].x, global.screen.h * 0.5 - 80 + this->puppetList[i].y - jump, 0);
		engineMathMat4Scale(&tempmat2, 1.0, -1.0, 0.01);
		engineMathMat4RotateX(&tempmat2, roth);
		engineMathMat4RotateY(&tempmat2, rotv);
		// ハコニワキャラクタ描画
		struct hknwChara *chara = hknwCharaListGet(dataPuppet->charaKind);
		hknwCharaDrawChara(chara, &tempmat2, rotv, roth, 0, 0, 0, 3.1415 / 180 * (90 - 45 * (double)this->puppetList[i].x / 60), 50.0, HKNWCHARAPOSESID_STAND, 0);
	}

	// ポップアップ描画
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
	if(this->popupTutorial != NULL){this->popupTutorial->draw(this->popupTutorial);}
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct popupCartridgeJumpResultWin *this){
	return 0;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct popupCartridgeJumpResultWin *this){
	if(this->popupTutorial != NULL){this->popupTutorial->dispose(this->popupTutorial);}
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *popupJumpResultWin(struct pageJumpStatus *stat){
	struct popupCartridgeJumpResultWin *this = (struct popupCartridgeJumpResultWin*)engineUtilMemoryCalloc(1, sizeof(struct popupCartridgeJumpResultWin));
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

