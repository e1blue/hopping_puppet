#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 動的データポインタ
static struct storageMeister0001 *storageData = NULL;

// 更新フラグ
static bool update = false;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 動的データ処理

// バージョンチェックとアップデート
void storageMeisterVersion(unsigned char major, unsigned char miner){
	switch((major << 8) | miner){
		case 0x0000:
			// データ初期化
			storageData = (struct storageMeister0001*)engineUtilMemoryCalloc(1, sizeof(struct storageMeister0001));

			// 初期マイスター
			storageData->list[0].level = 1;

			// 保存
			storageMeisterUpdate();
			storageMeisterWrite();
			break;
		case 0x0001:
		case 0x0101:
			break;
	}
}

// 有効化設定
void storageMeisterActive(bool active){
	if(active && storageData == NULL){
		// 有効化
		storageData = (struct storageMeister0001*)engineUtilMemoryCalloc(1, sizeof(struct storageMeister0001));
		storageGet("meister", storageData, sizeof(struct storageMeister0001));
	}else if(!active && storageData != NULL){
		// 無効化
		storageMeisterWrite();
		engineUtilMemoryFree(storageData);
		storageData = NULL;
	}
}

// データ獲得
struct storageMeister0001 *storageMeisterGet(){
	if(storageData == NULL){exit(1);}
	return storageData;
}

// 更新フラグ
void storageMeisterUpdate(){
	if(storageData == NULL){exit(1);}
	update = true;
}

// 保存
void storageMeisterWrite(){
	if(storageData == NULL || !update){return;}
	update = false;
	storageSet("meister", storageData, sizeof(struct storageMeister0001));
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

