#include "../library.h"
#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 設定構造体
struct settingGlobal0001{
	unsigned char bgmVolume;
	unsigned char seVolume;
};

// ----------------------------------------------------------------

#define STORAGESETTING_MAJOR 0x01
#define STORAGESETTING_MINER 0x01

// バージョン情報構造体
static struct settingVersion{
	unsigned char major;
	unsigned char miner;
} settingVersion;

static struct settingGlobal0001 settingGlobal;

// 設定初期化
void settingGameInit(){
	// バージョン情報確認
	if(storageGet("version", &settingVersion, sizeof(struct settingVersion))){settingVersion.major = settingVersion.miner = 0x00;}
	storageGet("setting", &settingGlobal, sizeof(struct settingGlobal0001));

	// テスト
	//settingVersion.major = 0;
	//settingVersion.miner = 0;

	if(settingVersion.major != STORAGESETTING_MAJOR || settingVersion.miner != STORAGESETTING_MINER){
		// バージョンが最新でない場合は保存データのバージョンアップを行う
		storageStatusVersion(settingVersion.major, settingVersion.miner);
		storagePuppetVersion(settingVersion.major, settingVersion.miner);
		storageMeisterVersion(settingVersion.major, settingVersion.miner);
		storageAtelierVersion(settingVersion.major, settingVersion.miner);
		storageFieldVersion(settingVersion.major, settingVersion.miner);
		// 設定のバージョンアップ
		switch((settingVersion.major << 8) | settingVersion.miner){
			case 0x0000:
				// 設定初期化
				settingGlobal.bgmVolume = 2;
				settingGlobal.seVolume = 2;
				storageSet("setting", &settingGlobal, sizeof(struct settingGlobal0001));
				break;
			case 0x0001:
			case 0x0101:
				break;
		}

		// テスト
		//storageStatusGet()->experience = 4740000;
		//storageStatusGet()->money = 9999;
		//storageStatusGet()->itemNum[0] = 20;
		//storageStatusGet()->itemNum[1] = 20;
		//storageStatusGet()->itemNum[2] = 20;
		//storageStatusGet()->itemNum[3] = 20;
		//storageStatusGet()->itemNum[4] = 20;
		//storageStatusGet()->itemNum[5] = 20;
		//storageStatusGet()->fieldKind = 14;
		//storageStatusGet()->fieldx = 1;
		//storageStatusGet()->fieldy = 6;
		//storageStatusGet()->fieldr = 0;
		//for(int i = 0; i < STORAGEMEISTER0001_KIND; i++){storageMeisterGet()->list[i].level = 1;}
		//for(int i = 0; i < STORAGEPUPPET0001_KIND; i++){storagePuppetGet()->list[i].level = 1; storagePuppetGet()->list[i].num = 1;}
		//for(int i = 0; i < STORAGEATELIER0001_CANVASXMAX; i++){for(int j = 0; j < STORAGEATELIER0001_CANVASYMAX; j++){storageAtelierGet()->canvas[i][j].puppetKind = randomGet() % STORAGEPUPPET0001_KIND; storageAtelierGet()->canvas[i][j].level = 1;}}
		//storageAtelierGet()->workspaceSize = 3;
		//storageAtelierGet()->canvasSize = 12;
		//storageStatusGet()->tutorial.top01 = true;
		//storageStatusGet()->tutorial.top02 = true;
		//storageStatusGet()->tutorial.atelier01 = true;
		//storageStatusGet()->tutorial.atelier02 = true;
		//storageStatusGet()->tutorial.atelier03 = true;
		//storageStatusGet()->tutorial.atelier04 = true;
		//storageStatusGet()->tutorial.atelier05 = true;
		//storageStatusGet()->tutorial.field01 = true;
		//storageStatusGet()->tutorial.field02 = true;
		//storageStatusGet()->tutorial.jump01 = true;
		//storageStatusGet()->tutorial.jump02 = true;
		//storageStatusGet()->tutorial.jump03 = true;
		//storageStatusUpdate();
		//storagePuppetUpdate();
		//storageMeisterUpdate();
		//storageAtelierUpdate();
		//storageFieldUpdate();
		//storageStatusWrite();
		//storagePuppetWrite();
		//storageMeisterWrite();
		//storageAtelierWrite();
		//storageFieldWrite();
		//settingGlobal.bgmVolume = 0;
		//settingGlobal.seVolume = 0;
		//storageSet("setting", &settingGlobal, sizeof(struct settingGlobal0001));

		// バージョン更新
		settingVersion.major = STORAGESETTING_MAJOR;
		settingVersion.miner = STORAGESETTING_MINER;
		storageSet("version", &settingVersion, sizeof(struct settingVersion));
		storageCommit();
	}

	settingBgmVolumeSet(settingGlobal.bgmVolume);
	settingSeVolumeSet(settingGlobal.seVolume);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// BGM音量獲得
int settingBgmVolumeGet(){return settingGlobal.bgmVolume;}

// BGM音量設定
void settingBgmVolumeSet(int type){
	double volume = 0.0;
	switch(type){
		case 1: volume = 0.1; break;
		case 2: volume = 0.4; break;
		case 3: volume = 0.9; break;
	}

	platformPluginSoundBgmVolume(volume);

	if(settingGlobal.bgmVolume != type){
		settingGlobal.bgmVolume = type;
		storageSet("setting", &settingGlobal, sizeof(struct settingGlobal0001));
	}
}

// SE音量獲得
int settingSeVolumeGet(){return settingGlobal.seVolume;}

// SE音量設定
void settingSeVolumeSet(int type){
	double volume = 0.0;
	switch(type){
		case 1: volume = 0.1; break;
		case 2: volume = 0.4; break;
		case 3: volume = 0.9; break;
	}

	platformPluginSoundSeVolume(volume);

	if(settingGlobal.seVolume != type){
		settingGlobal.seVolume = type;
		storageSet("setting", &settingGlobal, sizeof(struct settingGlobal0001));
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

