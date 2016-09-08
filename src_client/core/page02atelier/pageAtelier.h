#ifndef __pageAtelier_h_
#define __pageAtelier_h_

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページ状態構造体
struct pageAtelierStatus{
	// スライド量記憶
	int slideMeisterList;
	int slidePuppetList;
};

void pageAtelierStatusDispose(struct pageAtelierStatus *this);

// ----------------------------------------------------------------
// ページコンストラクタ

struct pageCartridge *pageAtelierMain(struct pageAtelierStatus *stat);

// ----------------------------------------------------------------
// ポップアップコンストラクタ

struct popupCartridge *popupAtelierMeisterList(struct pageAtelierStatus *stat, int meisterKind);
struct popupCartridge *popupAtelierPuppetList(struct pageAtelierStatus *stat);
struct popupCartridge *popupAtelierGacha(struct pageAtelierStatus *stat);
struct popupCartridge *popupAtelierGachaResult(struct pageAtelierStatus *stat, int meisterKind, int typeNew);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// マイスターボタン構造体
struct buttonAtelierMeister{
	struct partsButton super;
	bool select;
	int meisterKind;
	int meisterLevel;
	struct e3dImage box[4];
	struct e3dImage labelName;
	struct e3dImage labelLevel;
	struct e3dImage image;
	struct e3dImage gauge;
	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;
};

struct buttonAtelierMeister *buttonAtelierMeisterInit(int meisterKind);
void buttonAtelierMeisterPosition(struct buttonAtelierMeister *this, int x, int y);
#define buttonAtelierMeisterMove(this, x, y) partsButtonMove((struct partsButton*)(this), x, y)
#define buttonAtelierMeisterCalc(this) partsButtonCalc((struct partsButton*)(this))
#define buttonAtelierMeisterSubCalc(this, t, clickable) partsButtonSubCalc((struct partsButton*)(this), t, clickable)
void buttonAtelierMeisterDraw(struct buttonAtelierMeister *this, struct engineMathMatrix44 *mat, int charge, double alpha);
void buttonAtelierMeisterDispose(struct buttonAtelierMeister *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// パペットボタン構造体
struct buttonAtelierPuppet{
	struct partsButton super;
	bool select;
	int puppetKind;
	struct e3dImage box[4];
	struct e3dImage labelData;
	struct e3dNumber numberNum;
	struct e3dNumber numberLevel;
	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;
};

struct buttonAtelierPuppet *buttonAtelierPuppetInit(int puppetKind);
void buttonAtelierPuppetPosition(struct buttonAtelierPuppet *this, int x, int y);
#define buttonAtelierPuppetMove(this, x, y) partsButtonMove((struct partsButton*)(this), x, y)
#define buttonAtelierPuppetCalc(this) partsButtonCalc((struct partsButton*)(this))
#define buttonAtelierPuppetSubCalc(this, t, clickable) partsButtonSubCalc((struct partsButton*)(this), t, clickable)
void buttonAtelierPuppetDrawData(struct buttonAtelierPuppet *this, struct engineMathMatrix44 *mat, double alpha);
void buttonAtelierPuppetDrawChara(struct buttonAtelierPuppet *this, struct engineMathMatrix44 *mat);
void buttonAtelierPuppetDispose(struct buttonAtelierPuppet *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ガチャボタン構造体
struct buttonAtelierGacha{
	struct partsButton super;
	bool select;
	int gachaKind;
	struct e3dImage box[4];
	struct e3dImage labelName;
	struct e3dImage labelMoney;
	struct e3dImage image;
	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;
};

struct buttonAtelierGacha *buttonAtelierGachaInit(int gachaKind);
void buttonAtelierGachaPosition(struct buttonAtelierGacha *this, int x, int y);
#define buttonAtelierGachaMove(this, x, y) partsButtonMove((struct partsButton*)(this), x, y)
#define buttonAtelierGachaCalc(this) partsButtonCalc((struct partsButton*)(this))
#define buttonAtelierGachaSubCalc(this, t, clickable) partsButtonSubCalc((struct partsButton*)(this), t, clickable)
void buttonAtelierGachaDraw(struct buttonAtelierGacha *this, struct engineMathMatrix44 *mat, double alpha);
void buttonAtelierGachaDispose(struct buttonAtelierGacha *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#endif

