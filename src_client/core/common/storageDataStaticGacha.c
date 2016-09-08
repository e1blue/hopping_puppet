#include "../library.h"
#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 静的データリスト
static struct dataList{
	struct dataList *next;
	int gachaKind;
	struct dataGacha data;
	int e3dIdImage;
	int loading;
} *list = NULL;

static struct dataList *previous = NULL;

// ----------------------------------------------------------------

// ロード完了時コールバック
static void callback(void *param, void *buff, size_t size){
	struct dataList *this = param;
	if(this->loading > 0){
		if(buff != NULL){
			// ロード成功
			unsigned char *rawBuff = base64decode(buff, sizeof(struct dataGacha));
			memcpy(&this->data, rawBuff, sizeof(struct dataGacha));
			char path[256];
			sprintf(path, "img/gacha/%s.png", this->data.code);
			this->e3dIdImage = engineGraphicObjectTexCreate(path, ENGINEGRAPHICOBJECTTEXTYPE_LINEAR);
		}
		this->loading = 0;
	}else if(this->loading < 0){
		// ロード中止
		engineUtilMemoryFree(this);
	}
	if(buff != NULL){engineUtilMemoryFree(buff);}
}

// ロード済データ除去
static void clean(){
	struct dataList *temp = list->next;
	while(temp != NULL){
		struct dataList *next = temp->next;
		if(temp->loading == 0){
			engineGraphicObjectTexDispose(temp->e3dIdImage);
			engineUtilMemoryFree(temp);
		}else{
			// ロードが完了していないのでコールバックで破棄
			temp->loading = -1;
		}
		temp = next;
	}
	list->next = NULL;
}

// ----------------------------------------------------------------

// 有効化設定
void dataGachaActive(bool active){
	if(active && list == NULL){
		// 有効化
		list = (struct dataList*)engineUtilMemoryCalloc(1, sizeof(struct dataList));
		list->next = NULL;
	}else if(!active && list != NULL){
		// 無効化
		clean();
		engineUtilMemoryFree(list);
		list = NULL;
		previous = NULL;
	}
}

// データ獲得
struct dataGacha *dataGachaGet(int gachaKind, int *e3dIdImage){
	if(list == NULL){exit(1);}
	if(gachaKind < 0){return NULL;}

	// 直前のデータ確認
	if(previous != NULL && previous->gachaKind == gachaKind){
		if(e3dIdImage != NULL){*e3dIdImage = previous->e3dIdImage;}
		return &previous->data;
	}
	// ロード済みのデータ確認
	struct dataList *data = list->next;
	while(data != NULL){
		if(data->gachaKind == gachaKind){
			if(data->loading != 0){
				// データロード中
				return NULL;
			}else{
				// データが存在した場合
				previous = data;
				if(e3dIdImage != NULL){*e3dIdImage = data->e3dIdImage;}
				return &data->data;
			}
		}
		data = data->next;
	}

	// データが存在しなかった場合は作成する
	data = (struct dataList*)engineUtilMemoryCalloc(1, sizeof(struct dataList));
	data->gachaKind = gachaKind;
	// リストにデータ追加
	struct dataList *temp = list;
	while(temp->next != NULL){temp = temp->next;}
	temp->next = data;
	data->next = NULL;
	// ロード開始
	char path[256];
	sprintf(path, "data/gacha/%02d.dat", gachaKind);
	data->loading = 1;
	platformPluginDataLocal(data, path, callback);

	return dataGachaGet(gachaKind, e3dIdImage);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

