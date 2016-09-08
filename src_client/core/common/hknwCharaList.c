#include "../library.h"
#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ハコニワキャラクタリスト
static struct dataUnit{
	int isCreate;
	int isUpdate;
	struct dataList{
		struct dataList *next;
		enum hknwCharaKind charaKind;
		struct hknwChara *chara;
	} *list;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;
} *unit = NULL;

// ----------------------------------------------------------------


// 有効化設定
void hknwCharaListActive(bool active){
	if(active && unit == NULL){
		// 有効化
		unit = (struct dataUnit*)engineUtilMemoryCalloc(1, sizeof(struct dataUnit));
 	}else if(!active && unit != NULL){
		// 無効化
		struct dataList *temp = unit->list;
		while(temp != NULL){
			struct dataList *next = temp->next;
			hknwCharaDispose(temp->chara);
			engineUtilMemoryFree(temp);
			temp = next;
		}
		unit->list = NULL;

		engineGraphicObjectVBODispose(unit->e3dIdVert);
		engineGraphicObjectVBODispose(unit->e3dIdTexc);
		engineGraphicObjectIBODispose(unit->e3dIdFace);

		engineUtilMemoryFree(unit);
		unit = NULL;
	}
}

// データ獲得
struct hknwChara *hknwCharaListGet(enum hknwCharaKind charaKind){
	if(unit == NULL){return NULL;}

	// 作成済みデータ確認
	struct dataList *data = unit->list;
	while(data != NULL){
		if(data->charaKind == charaKind){
			// データが存在した場合
			return data->chara;
		}
		data = data->next;
	}

	// データが存在しなかった場合は作成する
	data = (struct dataList*)engineUtilMemoryCalloc(1, sizeof(struct dataList));
	data->charaKind = charaKind;
	data->chara = hknwCharaInit(charaKind);

	// 新しく作ったデータをリストに追加
	if(unit->list == NULL){
		unit->list = data;
	}else{
		struct dataList *temp = unit->list;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = data;
	}
	data->next = NULL;
	unit->isUpdate = 1;

	return data->chara;
}

// 描画準備
void hknwCharaListBind(){
	if(unit->isUpdate != 0){
		unit->isUpdate = 0;
		if(unit->isCreate){
			// 古いバッファ削除
			engineGraphicObjectVBODispose(unit->e3dIdVert);
			engineGraphicObjectVBODispose(unit->e3dIdTexc);
			engineGraphicObjectIBODispose(unit->e3dIdFace);
		}else{unit->isCreate = 1;}
		// バッファ作成開始
		engineGraphicBufferBegin();
		// キャラクター作成
		struct dataList *temp = unit->list;
		while(temp != NULL){hknwCharaCreateArray(temp->chara); temp = temp->next;}
		// バッファ作成完了
		engineGraphicBufferEnd(&unit->e3dIdVert, NULL, &unit->e3dIdTexc, &unit->e3dIdFace);
	}
	engineGraphicEngineBindVertVBO(unit->e3dIdVert);
	engineGraphicEngineBindTexcVBO(unit->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(unit->e3dIdFace);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

