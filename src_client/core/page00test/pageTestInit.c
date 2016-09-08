#include "../library.h"
#include "../includeAll.h"
#include "pageTest.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページ状態初期化
static struct pageTestStatus *pageTestStatusInit(){
	struct pageTestStatus *this = (struct pageTestStatus*)engineUtilMemoryCalloc(1, sizeof(struct pageTestStatus));

	// データ使用申請
	dataPuppetActive(true);
	dataMeisterActive(false);
	dataGachaActive(false);
	dataFieldActive(false);
	dataEnemyActive(false);
	dataBackgroundActive(false);
	storageStatusActive(false);
	storagePuppetActive(false);
	storageMeisterActive(false);
	storageAtelierActive(false);
	storageFieldActive(false);
	hknwCharaListActive(true);
	storageStatusWrite();
	storagePuppetWrite();
	storageMeisterWrite();
	storageAtelierWrite();
	storageFieldWrite();
	storageCommit();

	return this;
}

// ページ状態破棄
void pageTestStatusDispose(struct pageTestStatus *this){
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体
struct pageCartridgeTestInit{
	struct pageCartridge super;
	struct pageTestStatus *stat;

	struct e3dImage label;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	int action;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageCartridgeTestInit *this){
	this->stat = pageTestStatusInit();
}

// ----------------------------------------------------------------

// 計算
static int calc(struct pageCartridgeTestInit *this){
	if(this->action++ > 10){
		pageCartridgePush(pageTestMain(this->stat));
		return 1;
	}

	// 戻るボタン押下確認
	if(global.key.bk.triggerInactive){
		global.key.bk.triggerInactive = false;
		// 何も起きない
	}

	return 0;
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct pageCartridgeTestInit *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ラベル作成
		e3dImageTextCreateArray(&this->label, global.screen.w * 0.5, global.screen.h * 0.5, "loading...", 1.0, 0, 0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct pageCartridgeTestInit *this){
	// 描画準備
	createBuffer(this);

	// 射影行列作成
	engineMathMat4Ortho(&e3dMatOrtho, -global.screen.offset.x, global.screen.w + global.screen.offset.x, global.screen.h + global.screen.offset.y, -global.screen.offset.y, -1, 1);

	engineGraphicEngineClearAll();

	// 2D描画準備
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);
	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
	engineGraphicEngineSetMatrix(&e3dMatOrtho);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);

	// ラベル描画
	e3dImageDraw(&this->label);

	engineGraphicEngineFlush();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageCartridgeTestInit *this){
}

// 破棄
static void dispose(struct pageCartridgeTestInit *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectTexDispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページカートリッジ作成
struct pageCartridge *pageTestInit(){
	struct pageCartridgeTestInit *this = (struct pageCartridgeTestInit*)engineUtilMemoryCalloc(1, sizeof(struct pageCartridgeTestInit));

	struct pageCartridge *cartridge = (struct pageCartridge*)this;
	cartridge->init = (void(*)(struct pageCartridge*))init;
	cartridge->calc = (int(*)(struct pageCartridge*))calc;
	cartridge->draw = (void(*)(struct pageCartridge*))draw;
	cartridge->pause = (void(*)(struct pageCartridge*))pause;
	cartridge->dispose = (void(*)(struct pageCartridge*))dispose;
	return cartridge;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

