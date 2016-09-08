#include "../library.h"
#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 動的データポインタ
static struct storageStatus0101 *storageData = NULL;

// 更新フラグ
static bool update = false;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 動的データ処理

// バージョンチェックとアップデート
void storageStatusVersion(unsigned char major, unsigned char miner){
	switch((major << 8) | miner){
		case 0x0000:
			// データ初期化
			storageData = (struct storageStatus0101*)engineUtilMemoryCalloc(1, sizeof(struct storageStatus0101));

			// 初期ステータス
			storageData->experience = 0;
			storageData->health = parameterPlayerHealthMax();
			storageData->money = 0;
			storageData->fieldKind = 0;
			storageData->fieldx = 1;
			storageData->fieldy = 1;
			storageData->enemyIndex = -1;
			storageData->time = platformPluginUtilTimeGet();

			// 保存
			storageStatusUpdate();
			storageStatusWrite();
			break;
		case 0x0001:{
			// 旧データ読み込み
			struct storageStatus0001 oldData;
			storageGet("status", &oldData, sizeof(struct storageStatus0001));
			// 新データ初期化
			storageData = (struct storageStatus0101*)engineUtilMemoryCalloc(1, sizeof(struct storageStatus0101));

			storageData->health = oldData.health;
			storageData->experience = oldData.experience;
			storageData->money = oldData.money;
			for(int i = 0; i < STORAGESTATUS0001_ITEMKIND; i++){storageData->itemNum[i] = oldData.itemNum[i];}
			storageData->isTension = oldData.isTension;
			storageData->isWeak = oldData.isWeak;
			storageData->fieldKind = oldData.fieldKind;
			storageData->fieldx = oldData.fieldx;
			storageData->fieldy = oldData.fieldy;
			storageData->fieldr = oldData.fieldr;
			storageData->enemyIndex = oldData.enemyIndex;
			storageData->time = oldData.time;
			storageData->tutorial.top01 = oldData.tutorial.top01;
			storageData->tutorial.top02 = oldData.tutorial.top02;
			storageData->tutorial.atelier01 = oldData.tutorial.atelier01;
			storageData->tutorial.atelier02 = oldData.tutorial.atelier02;
			storageData->tutorial.atelier03 = oldData.tutorial.atelier03;
			storageData->tutorial.atelier04 = oldData.tutorial.atelier04;
			storageData->tutorial.atelier05 = oldData.tutorial.atelier05;
			storageData->tutorial.field01 = oldData.tutorial.field01;
			storageData->tutorial.field02 = oldData.tutorial.field02;
			storageData->tutorial.jump01 = oldData.tutorial.jump01;
			storageData->tutorial.jump02 = oldData.tutorial.jump02;
			storageData->tutorial.jump03 = oldData.tutorial.jump03;

			// 保存
			storageStatusUpdate();
			storageStatusWrite();
			}break;
		case 0x0101:
			break;
	}
}

// 有効化設定
void storageStatusActive(bool active){
	if(active && storageData == NULL){
		// 有効化
		storageData = (struct storageStatus0101*)engineUtilMemoryCalloc(1, sizeof(struct storageStatus0101));
		storageGet("status", storageData, sizeof(struct storageStatus0101));
	}else if(!active && storageData != NULL){
		// 無効化
		storageStatusWrite();
		engineUtilMemoryFree(storageData);
		storageData = NULL;
	}
}

// データ獲得
struct storageStatus0101 *storageStatusGet(){
	if(storageData == NULL){exit(1);}
	return storageData;
}

// 更新フラグ
void storageStatusUpdate(){
	if(storageData == NULL){exit(1);}
	update = true;
}

// 保存
void storageStatusWrite(){
	if(storageData == NULL || !update){return;}
	update = false;
	storageSet("status", storageData, sizeof(struct storageStatus0101));
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

