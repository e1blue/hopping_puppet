#include "../library.h"
#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ポップアップ展開計算
static int calc(struct popupCartridge *this){
	this->popupCalcBefore(this);
	// 前遷移開始確認
	if(this->openStep >= this->openStepMax){this->openStep = -this->openStepMax;}
	// 前後遷移中処理
	if(this->openStep != 0){this->openStep++;}
	// 前遷移完了後の通常処理
	if(this->openStep == 0 && this->popupCalcMain(this)){this->openStep = 1;}
	// 後遷移完了確認
	return (this->openStep >= this->openStepMax);
}

// ポップアップカートリッジ初期化
void popupCartridgeInit(struct popupCartridge *this){
	this->calc = calc;
	this->openStep = this->openStepMax = 10;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

