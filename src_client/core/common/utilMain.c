#include "../library.h"
#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ユーティリティ初期化
void utilGameInit(){
	engineMathRandomInit(platformPluginUtilTimeGet());
	parameterInit();
}

// ユーティリティ終了
void utilGameFinal(){
	dataPuppetActive(false);
	dataMeisterActive(false);
	dataGachaActive(false);
	dataFieldActive(false);
	dataEnemyActive(false);
	dataBackgroundActive(false);
	hknwCharaListActive(false);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

