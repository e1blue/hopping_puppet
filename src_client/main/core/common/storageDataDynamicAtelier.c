#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 動的データポインタ
static struct storageAtelier0001 *storageData = NULL;

// 更新フラグ
static bool update = false;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 動的データ処理

// バージョンチェックとアップデート
void storageAtelierVersion(unsigned char major, unsigned char miner){
	switch((major << 8) | miner){
		case 0x0000:
			// データ初期化
			storageData = (struct storageAtelier0001*)engineUtilMemoryCalloc(1, sizeof(struct storageAtelier0001));

			// 初期アトリエ
			storageData->time = platformPluginUtilTimeGet();
			storageData->workspaceSize = 1;
			storageData->canvasSize = 4;
			for(int i = 0; i < STORAGEATELIER0001_WORKSPACEXMAX; i++){
				storageData->workspace[i].meisterKind = -1;
			}
			for(int i = 0; i < STORAGEATELIER0001_CANVASXMAX; i++){
				for(int j = 0; j < STORAGEATELIER0001_CANVASYMAX; j++){
					storageData->canvas[i][j].puppetKind = -1;
				}
			}

			// 保存
			storageAtelierUpdate();
			storageAtelierWrite();
			break;
		case 0x0001:
		case 0x0101:
			break;
	}
}

// 有効化設定
void storageAtelierActive(bool active){
	if(active && storageData == NULL){
		// 有効化
		storageData = (struct storageAtelier0001*)engineUtilMemoryCalloc(1, sizeof(struct storageAtelier0001));
		storageGet("atelier", storageData, sizeof(struct storageAtelier0001));
	}else if(!active && storageData != NULL){
		// 無効化
		storageAtelierWrite();
		engineUtilMemoryFree(storageData);
		storageData = NULL;
	}
}

// データ獲得
struct storageAtelier0001 *storageAtelierGet(){
	if(storageData == NULL){exit(1);}
	return storageData;
}

// 更新フラグ
void storageAtelierUpdate(){
	if(storageData == NULL){exit(1);}
	update = true;
}

// 保存
void storageAtelierWrite(){
	if(storageData == NULL || !update){return;}
	update = false;
	storageSet("atelier", storageData, sizeof(struct storageAtelier0001));
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

