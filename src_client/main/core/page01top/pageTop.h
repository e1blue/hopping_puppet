#ifndef __pageTop_h_
#define __pageTop_h_

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページ状態構造体
struct pageTopStatus{
};

void pageTopStatusDispose(struct pageTopStatus *this);

// ----------------------------------------------------------------
// ページコンストラクタ

struct pageCartridge *pageTopMain(struct pageTopStatus *stat);

// ----------------------------------------------------------------
// ポップアップコンストラクタ

struct popupCartridge *popupTopOption(struct pageTopStatus *stat);
struct popupCartridge *popupTopVolume(struct pageTopStatus *stat, int type);
struct popupCartridge *popupTopManual(struct pageTopStatus *stat);
struct popupCartridge *popupTopCredit(struct pageTopStatus *stat);

// ----------------------------------------------------------------

enum popupTopOptionResult{
	POPUPTOPOPTIONRESULT_NONE = 0,
	POPUPTOPOPTIONRESULT_MANUAL,
	POPUPTOPOPTIONRESULT_CREDIT
};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#endif

