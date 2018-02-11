#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 動的データポインタ
static struct storagePuppet0001 *storageData = NULL;

// 更新フラグ
static bool update = false;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 動的データ処理

// バージョンチェックとアップデート
void storagePuppetVersion(unsigned char major, unsigned char miner){
	switch((major << 8) | miner){
		case 0x0000:
			// データ初期化
			storageData = (struct storagePuppet0001*)engineUtilMemoryCalloc(1, sizeof(struct storagePuppet0001));

			// 保存
			storagePuppetUpdate();
			storagePuppetWrite();
			break;
		case 0x0001:
		case 0x0101:
			break;
	}
}

// 有効化設定
void storagePuppetActive(bool active){
	if(active && storageData == NULL){
		// 有効化
		storageData = (struct storagePuppet0001*)engineUtilMemoryCalloc(1, sizeof(struct storagePuppet0001));
		storageGet("puppet", storageData, sizeof(struct storagePuppet0001));
	}else if(!active && storageData != NULL){
		// 無効化
		storagePuppetWrite();
		engineUtilMemoryFree(storageData);
		storageData = NULL;
	}
}

// データ獲得
struct storagePuppet0001 *storagePuppetGet(){
	if(storageData == NULL){exit(1);}
	return storageData;
}

// 更新フラグ
void storagePuppetUpdate(){
	if(storageData == NULL){exit(1);}
	update = true;
}

// 保存
void storagePuppetWrite(){
	if(storageData == NULL || !update){return;}
	update = false;
	storageSet("puppet", storageData, sizeof(struct storagePuppet0001));
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

