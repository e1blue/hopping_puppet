#ifndef __cartridge_h_
#define __cartridge_h_

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジ構造体
struct pageCartridge{
	struct pageCartridge *next;
	void (*init)(struct pageCartridge *this);
	int (*calc)(struct pageCartridge *this);
	void (*draw)(struct pageCartridge *this);
	void (*pause)(struct pageCartridge *this);
	void (*dispose)(struct pageCartridge *this);
};

void pageCartridgeGameInit();
void pageCartridgeGameCalc();
void pageCartridgeGamePause();
void pageCartridgeGameFinal();

void pageCartridgePush(struct pageCartridge *cartridge);
// 各ページカートリッジのコンストラクタ
struct pageCartridge *pageTestInit();
struct pageCartridge *pageTopInit();
struct pageCartridge *pageAtelierInit();
struct pageCartridge *pageFieldInit();
struct pageCartridge *pageJumpInit();

// 最初のページカートリッジ
#define FIRSTCARTRIDGE pageTopInit
//#define FIRSTCARTRIDGE pageTestInit

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ポップアップカートリッジ構造体
struct popupCartridge{
	int (*calc)(struct popupCartridge *this);
	void (*popupCalcBefore)(struct popupCartridge *this);
	int (*popupCalcMain)(struct popupCartridge *this);
	void (*draw)(struct popupCartridge *this);
	int (*result)(struct popupCartridge *this);
	void (*dispose)(struct popupCartridge *this);
	int openStep;
	int openStepMax;
};

void popupCartridgeInit(struct popupCartridge *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#endif

