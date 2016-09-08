#include "library.h"
#include "includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// opengl初期化
void gameMainSurfaceCreated(void){
	engineGraphicGameInit();
	e3dUtilGameInit();
	utilGameInit();
	soundGameInit();
	settingGameInit();
	pageCartridgeGameInit();

}

// opengl描画
void gameMainSurfaceDrawFrame(void){
	engineCtrlTouchCalc();
	pageCartridgeGameCalc();
	storageGameCalc();
}

// opengl中断
void gameMainSurfacePause(void){
	pageCartridgeGamePause();
	storageGamePause();
}

// opengl終了
void gameMainSurfaceDestroy(void){
	pageCartridgeGameFinal();
	utilGameFinal();
	e3dUtilGameFinal();
	engineGraphicGameExit();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

