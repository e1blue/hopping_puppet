#ifndef __util_h_
#define __util_h_

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ユーティリティ初期化
void utilGameInit();

// ユーティリティ終了
void utilGameFinal();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 音処理

void soundGameInit();

void soundBgmStop();
void soundBgmPlayAtelier();
void soundBgmPlayField();
void soundBgmPlayJump();
void soundBgmToneDown();

void soundSePlayOK();
void soundSePlayNG();
void soundSePlayGet();
void soundSePlayUse();
void soundSePlayJump();
void soundSePlayBomb();
void soundSePlayGacha();
void soundSePlayHarvest0();
void soundSePlayHarvest1();
void soundSePlayHarvest2();
void soundSePlayOpenCircle();
void soundSePlayTransfer();
void soundSePlayEnemyEnter();
void soundSePlayPenalty();
void soundSePlayTension();
void soundSePlayEnemyDown();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// パラメータ

#define UTILPARAMETER_LEVELMAX 99
#define UTILPARAMETER_EXPERIENCEMAX 99999999
#define UTILPARAMETER_MONEYMAX 99999999
#define UTILPARAMETER_ITEMNUMMAX 99

void parameterInit();
int parameterPlayerLevel();
int parameterPlayerExperience();
int parameterPlayerHealthMax();
int parameterPlayerTensionMax();
int parameterPlayerPuppetNum();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#define ITEMNAME_HEALTH "かいふくジュース"
#define ITEMNAME_TENSION "テンションプリン"
#define ITEMNAME_MONSTERMOVE "モンスタうごくん"
#define ITEMNAME_CANVASFULL "かそくそうち"
#define ITEMNAME_CANVASOPEN "アトリエかくちょうA"
#define ITEMNAME_WORKSPACEOPEN "アトリエかくちょうB"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#endif

