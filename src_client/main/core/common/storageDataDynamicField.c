#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 動的データポインタ
static struct storageField0001 *storageData = NULL;

// 更新フラグ
static bool update = false;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 動的データ処理

// バージョンチェックとアップデート
void storageFieldVersion(unsigned char major, unsigned char miner){
	switch((major << 8) | miner){
		case 0x0000:
			// データ初期化
			storageData = (struct storageField0001*)engineUtilMemoryCalloc(1, sizeof(struct storageField0001));

			// 初期フィールド
			for(int i = 0; i < STORAGEFIELD0001_KIND; i++){
				for(int j = 0; j < STORAGEFIELD0001_MAPMAX; j++){}
				for(int j = 0; j < STORAGEFIELD0001_ENEMYMAX; j++){
					storageData->list[i].enemy[j].enemyKind = -1;
				}
			}

			// 保存
			storageFieldUpdate();
			storageFieldWrite();
			break;
		case 0x0001:
		case 0x0101:
			break;
	}
}

// 有効化設定
void storageFieldActive(bool active){
	if(active && storageData == NULL){
		// 有効化
		storageData = (struct storageField0001*)engineUtilMemoryCalloc(1, sizeof(struct storageField0001));
		storageGet("field", storageData, sizeof(struct storageField0001));
	}else if(!active && storageData != NULL){
		// 無効化
		storageFieldWrite();
		engineUtilMemoryFree(storageData);
		storageData = NULL;
	}
}

// データ獲得
struct storageField0001 *storageFieldGet(){
	if(storageData == NULL){exit(1);}
	return storageData;
}

// 更新フラグ
void storageFieldUpdate(){
	if(storageData == NULL){exit(1);}
	update = true;
}

// 保存
void storageFieldWrite(){
	if(storageData == NULL || !update){return;}
	update = false;
	storageSet("field", storageData, sizeof(struct storageField0001));
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

