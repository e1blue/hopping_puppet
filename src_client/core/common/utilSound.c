#include "../library.h"
#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

void soundGameInit(){
	platformPluginSoundBgmLoad(1, "snd/bgm/loop_99");
	platformPluginSoundBgmLoad(2, "snd/bgm/loop_42");
	platformPluginSoundBgmLoad(3, "snd/bgm/loop_125");
	platformPluginSoundSeLoad(1, "snd/se/button");
	platformPluginSoundSeLoad(2, "snd/se/menu_2");
	platformPluginSoundSeLoad(3, "snd/se/get_2");
	platformPluginSoundSeLoad(4, "snd/se/jump_2");
	platformPluginSoundSeLoad(5, "snd/se/sword_f");
	platformPluginSoundSeLoad(6, "snd/se/get_5");
	platformPluginSoundSeLoad(7, "snd/se/menu");
	platformPluginSoundSeLoad(8, "snd/se/pui");
	platformPluginSoundSeLoad(9, "snd/se/get");
	platformPluginSoundSeLoad(10, "snd/se/get_4");
	platformPluginSoundSeLoad(11, "snd/se/get_3");
	platformPluginSoundSeLoad(12, "snd/se/fall");
	platformPluginSoundSeLoad(13, "snd/se/power");
	platformPluginSoundSeLoad(14, "snd/se/koke");
}

void soundBgmStop(){platformPluginSoundBgmPlay(-1);}
void soundBgmPlayAtelier(){platformPluginSoundBgmPlay(1);}
void soundBgmPlayField(){platformPluginSoundBgmPlay(2);}
void soundBgmPlayJump(){platformPluginSoundBgmPlay(3);}
void soundBgmToneDown(){platformPluginSoundBgmToneDown(0.5);}

void soundSePlayOK(){platformPluginSoundSePlay(1);}
void soundSePlayNG(){platformPluginSoundSePlay(2);}
void soundSePlayGet(){platformPluginSoundSePlay(3);}
void soundSePlayUse(){platformPluginSoundSePlay(3);}
void soundSePlayJump(){platformPluginSoundSePlay(4);}
void soundSePlayBomb(){platformPluginSoundSePlay(5);}
void soundSePlayGacha(){platformPluginSoundSePlay(6);}
void soundSePlayHarvest0(){platformPluginSoundSePlay(7);}
void soundSePlayHarvest1(){platformPluginSoundSePlay(8);}
void soundSePlayHarvest2(){platformPluginSoundSePlay(9);}
void soundSePlayOpenCircle(){platformPluginSoundSePlay(10);}
void soundSePlayTransfer(){platformPluginSoundSePlay(11);}
void soundSePlayEnemyEnter(){platformPluginSoundSePlay(12);}
void soundSePlayTension(){platformPluginSoundSePlay(13);}
void soundSePlayPenalty(){platformPluginSoundSePlay(14);}
void soundSePlayEnemyDown(){platformPluginSoundSePlay(12);}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

