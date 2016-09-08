#ifndef __pageField_h_
#define __pageField_h_

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページ状態構造体
struct pageFieldStatus{
};

void pageFieldStatusDispose(struct pageFieldStatus *this);

// ----------------------------------------------------------------
// ページコンストラクタ

struct pageCartridge *pageFieldMain(struct pageFieldStatus *stat);

// ----------------------------------------------------------------
// ポップアップコンストラクタ

struct popupCartridge *popupFieldOpen(struct pageFieldStatus *stat, int cost);
struct popupCartridge *popupFieldInfoHealth(struct pageFieldStatus *stat);
struct popupCartridge *popupFieldInfoPuppet(struct pageFieldStatus *stat);
struct popupCartridge *popupFieldInfoTrial(struct pageFieldStatus *stat);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#endif

