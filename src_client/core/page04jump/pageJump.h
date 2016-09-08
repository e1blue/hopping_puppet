#ifndef __pageJump_h_
#define __pageJump_h_

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページ状態構造体
struct pageJumpStatus{
	// 勝利ボーナスデータ
	struct {
		int enemyKind;
		int experience;
		int money;
		int itemKind;
		int prevLevel;
		int prevExperience;
		int prevMoney;
		int nextLevel;
		int nextExperience;
		int nextMoney;
	} resultData;
};

void pageJumpStatusDispose(struct pageJumpStatus *this);

// ----------------------------------------------------------------
// ページコンストラクタ

struct pageCartridge *pageJumpMain(struct pageJumpStatus *stat);

// ----------------------------------------------------------------
// ポップアップコンストラクタ

struct popupCartridge *popupJumpEscape(struct pageJumpStatus *stat);
struct popupCartridge *popupJumpResultWin(struct pageJumpStatus *stat);
struct popupCartridge *popupJumpResultLose(struct pageJumpStatus *stat);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#endif

