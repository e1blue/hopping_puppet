#ifndef __parts_h_
#define __parts_h_

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ボタン構造体
struct partsButton{
	int movex;
	int movey;
	int x;
	int y;
	int w;
	int h;
	bool active;
	bool inactive;
	bool trigger;
	struct partsButton *child;
	bool zKey;
	bool xKey;
	bool cKey;
	bool vKey;
	bool inner;
};

void partsButtonInit(struct partsButton *this);
void partsButtonPosition(struct partsButton *this, int x, int y, int w, int h);
void partsButtonMove(struct partsButton *this, int x, int y);
void partsButtonCalc(struct partsButton *this);
void partsButtonSubCalc(struct partsButton *this, struct engineCtrlTouch *t, bool clickable);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示箱付きボタン構造体
struct partsButtonBox{
	struct partsButton super;
	bool select;
	int faceIndex;
	int faceNum;
};

void partsButtonBoxInit(struct partsButtonBox *this);
void partsButtonBoxCreateArray(struct partsButtonBox *this, int x, int y, int w, int h, char *text, double scale);
#define partsButtonBoxMove(this, x, y) partsButtonMove((struct partsButton*)(this), x, y)
#define partsButtonBoxCalc(this) partsButtonCalc((struct partsButton*)(this))
#define partsButtonBoxSubCalc(this, t, clickable) partsButtonSubCalc((struct partsButton*)(this), t, clickable)
void partsButtonBoxDraw(struct partsButtonBox *this, struct engineMathMatrix44 *mat, double alpha);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ピッカーボタン構造体
struct partsButtonPicker{
	struct partsButton super;
	bool select;
	int picked;
	int faceIndex;
	int faceNum;
	struct e3dImage text[5];
};

void partsButtonPickerInit(struct partsButtonPicker *this, int picked);
void partsButtonPickerCreateArray(struct partsButtonPicker *this, int x, int y, int w, char **textList, int textListLength, double scale);
#define partsButtonPickerMove(this, x, y) partsButtonCalc((struct partsButton*)(this, x, y))
#define partsButtonPickerCalc(this) partsButtonCalc((struct partsButton*)(this))
#define partsButtonPickerSubCalc(this, t, clickable) partsButtonSubCalc((struct partsButton*)(this), t, clickable)
void partsButtonPickerDraw(struct partsButtonPicker *this, struct engineMathMatrix44 *mat, double alpha);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// メモリートレースボタン

#ifdef FUHAHA_MEMORY_DEBUG
void partsButtonMemoryCalcImplement();
void partsButtonMemoryDrawImplement();
#define partsButtonMemoryCalc() partsButtonMemoryCalcImplement()
#define partsButtonMemoryDraw() partsButtonMemoryDrawImplement()
#else
#define partsButtonMemoryCalc()
#define partsButtonMemoryDraw()
#endif

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// おかね情報ポップアップ
struct popupCartridge *partsPopupMoneyInfoGet(int money);

// アイテム確認ポップアップ
struct popupCartridge *partsPopupItemAskUse(int itemKind);

// アイテム情報ポップアップ
struct popupCartridge *partsPopupItemInfoGet(int itemKind);
struct popupCartridge *partsPopupItemInfoUse(int itemKind);

// アイテム選択ポップアップ
struct popupCartridge *partsPopupItemSelect(unsigned int active);

// ピッカーポップアップ
struct popupCartridge *partsPopupPicker(char *title, char **textList, int textListLength, int select);

// パペットプロフィールポップアップ
struct popupCartridge *partsPopupPuppetProfile(int puppetKind);

// チュートリアルポップアップ
struct popupCartridge *partsPopupTutorialTop01A();
struct popupCartridge *partsPopupTutorialTop01B();
struct popupCartridge *partsPopupTutorialTop02();
struct popupCartridge *partsPopupTutorialAtelier01();
struct popupCartridge *partsPopupTutorialAtelier02();
struct popupCartridge *partsPopupTutorialAtelier03();
struct popupCartridge *partsPopupTutorialAtelier04();
struct popupCartridge *partsPopupTutorialAtelier05A();
struct popupCartridge *partsPopupTutorialAtelier05B();
struct popupCartridge *partsPopupTutorialField01();
struct popupCartridge *partsPopupTutorialField02A();
struct popupCartridge *partsPopupTutorialField02B();
struct popupCartridge *partsPopupTutorialJump01();
struct popupCartridge *partsPopupTutorialJump02();
struct popupCartridge *partsPopupTutorialJump03();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#endif

