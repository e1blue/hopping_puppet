#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static bool isStorageWrite = false;

// base64形式で構造体データ保存
void storageSet(char *key, void *data, int length){
	char *buff = base64encode((unsigned char*)data, length);

	isStorageWrite = true;
	platformPluginPreferenceSet(key, buff);
}

// base64形式で構造体データ読込
int storageGet(char *key, void *data, int length){
	char *b64Buff = platformPluginPreferenceGet(key);
	if(b64Buff != NULL){
		unsigned char *rawBuff = base64decode((unsigned char*)b64Buff, length);
		memcpy(data, rawBuff, length);
		return 0;
	}else{
		memset(data, 0, length);
		return 1;
	}
}

// 構造体データ保存確定
void storageCommit(){if(isStorageWrite){isStorageWrite = false; platformPluginPreferenceCommit();}}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ストレージメインループ計算
void storageGameCalc(){
	static int count = 0;
	if(((count++) % (60 * 10)) == 0){
		storageStatusWrite();
		storagePuppetWrite();
		storageMeisterWrite();
		storageAtelierWrite();
		storageFieldWrite();
		storageCommit();
	}
}

// 一時停止時のストレージ保存処理
void storageGamePause(){
	storageStatusWrite();
	storagePuppetWrite();
	storageMeisterWrite();
	storageAtelierWrite();
	storageFieldWrite();
	storageCommit();
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

