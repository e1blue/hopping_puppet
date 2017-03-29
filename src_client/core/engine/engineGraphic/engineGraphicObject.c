#include "../../library.h"
#include "../../plugin/pluginTexture.h"
#include "../engineMath/engineMath.h"
#include "../engineUtil/engineUtil.h"
#include "engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 3DオブジェクトVBO構造体
struct engineGraphicObjectVBOCage{
	struct engineGraphicObjectVBOCage *next;
	engineGraphicObjectVBOId egoId;
	// VBOデータ
	GLuint glId;
	int length;
	GLfloat *vertices;
};

// 3DオブジェクトIBO構造体
struct engineGraphicObjectIBOCage{
	struct engineGraphicObjectIBOCage *next;
	engineGraphicObjectIBOId egoId;
	// IBOデータ
	GLuint glId;
	int length;
	GLushort *indexes;
};

// 3DオブジェクトTex構造体
struct engineGraphicObjectTexCage{
	struct engineGraphicObjectTexCage *next;
	engineGraphicObjectTexId egoId;
	// テクスチャデータ
	struct engineGraphicObjectTexData *data;
	enum engineGraphicObjectTexType type;
};

// テクスチャ引数構造体
struct engineGraphicObjectTexArg{
	enum engineGraphicObjectTexArgType{
		ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL,
		ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT,
		ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEST,
	} type;
	union{
		struct{char *src;} pluginTextureLocal;
		struct{int fontSetId; char *letterList; int letterLenght;} pluginTextureFont;
		struct{} pluginTest;
	};
};

// テクスチャ情報構造体
struct engineGraphicObjectTexData{
	struct engineGraphicObjectTexData *next;
	struct engineGraphicObjectTexArg arg;
	union{
		struct{GLuint glId; int w; int h;} pluginTextureLocal;
		struct{GLuint glId; int w; int h; struct pluginTextureFontCode *codeList;} pluginTextureFont;
		struct{} pluginTest;
	};
	enum engineGraphicObjectTexDataStatus{
		ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADING,
		ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED,
		ENGINEGRAPHICOBJECTTEXDATASTATUS_CANCEL,
	} status;
};

static struct{
	// デフォルトテクスチャ
	struct{
		GLuint glId;
		int w;
		int h;
	} defaultTexture;
	// 3Dオブジェクトリスト
	int egoIdCount;
	struct engineGraphicObjectVBOCage *egoVBOList;
	struct engineGraphicObjectIBOCage *egoIBOList;
	struct engineGraphicObjectTexCage *egoTexList;
	struct engineGraphicObjectTexData *texDataList;
} localGlobal = {0};

// ----------------------------------------------------------------

// 3DオブジェクトVBO解放
static void egoVBOFree(struct engineGraphicObjectVBOCage *this){
	glDeleteBuffers(1, &this->glId);
	engineUtilMemoryInfoFree("engineGraphicObject vbo2", this->vertices);
	this->vertices = NULL;
	engineUtilMemoryInfoFree("engineGraphicObject vbo1", this);
}

// 3DオブジェクトIBO解放
static void egoIBOFree(struct engineGraphicObjectIBOCage *this){
	glDeleteBuffers(1, &this->glId);
	engineUtilMemoryInfoFree("engineGraphicObject ibo2", this->indexes);
	this->indexes = NULL;
	engineUtilMemoryInfoFree("engineGraphicObject ibo1", this);
}

// テクスチャ情報解放
static void texDataFree(struct engineGraphicObjectTexData *this){
	if(this->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL){
		if(this->status == ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED){
			// 解放
			if(this->pluginTextureLocal.glId != localGlobal.defaultTexture.glId){glDeleteTextures(1, &this->pluginTextureLocal.glId);}
			engineUtilMemoryInfoFree("engineGraphicObject tex3", this->arg.pluginTextureLocal.src);
			this->arg.pluginTextureLocal.src = NULL;
			engineUtilMemoryInfoFree("engineGraphicObject tex2", this);
		}else{
			// ロードが完了していないのでコールバックで破棄
			this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_CANCEL;
		}
	}else if(this->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT){
		if(this->status == ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED){
			// 解放
			if(this->pluginTextureFont.glId != localGlobal.defaultTexture.glId){glDeleteTextures(1, &this->pluginTextureFont.glId);}
			engineUtilMemoryInfoFree("pluginTextureFontCodeList", this->pluginTextureFont.codeList);
			engineUtilMemoryInfoFree("engineGraphicObject tex3", this->arg.pluginTextureFont.letterList);
			this->pluginTextureFont.codeList = NULL;
			this->arg.pluginTextureFont.letterList = NULL;
			engineUtilMemoryInfoFree("engineGraphicObject tex2", this);
		}else{
			// ロードが完了していないのでコールバックで破棄
			this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_CANCEL;
		}
	}else if(this->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEST){
		// 解放
		engineUtilMemoryInfoFree("engineGraphicObject tex2", this);
	}
}

// 3DオブジェクトTex解放
static void egoTexFree(struct engineGraphicObjectTexCage *this){
	engineUtilMemoryInfoFree("engineGraphicObject tex1", this);
}

// ----------------------------------------------------------------

// ロード完了時コールバック
static void texDataLocalCallback(void *param, int glId, int w, int h){
	struct engineGraphicObjectTexData *this = (struct engineGraphicObjectTexData*)param;
	enum engineGraphicObjectTexDataStatus beforeStatus = this->status;
	this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED;
	if(beforeStatus == ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADING){
		// ロード完了
		this->pluginTextureLocal.glId = glId;
		this->pluginTextureLocal.w = w;
		this->pluginTextureLocal.h = h;
	}else if(beforeStatus == ENGINEGRAPHICOBJECTTEXDATASTATUS_CANCEL){
		// ロード中止
		texDataFree(this);
	}
}

// ロード完了時コールバック
static void texDataFontCallback(void *param, int glId, int w, int h, struct pluginTextureFontCode *codeList){
	struct engineGraphicObjectTexData *this = (struct engineGraphicObjectTexData*)param;
	enum engineGraphicObjectTexDataStatus beforeStatus = this->status;
	this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED;
	if(beforeStatus == ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADING){
		// ロード完了
		this->pluginTextureFont.glId = glId;
		this->pluginTextureFont.w = w;
		this->pluginTextureFont.h = h;
		this->pluginTextureFont.codeList = codeList;
	}else if(beforeStatus == ENGINEGRAPHICOBJECTTEXDATASTATUS_CANCEL){
		// ロード中止
		texDataFree(this);
	}
}

// ----------------------------------------------------------------

// VBO作成
static void egoVBOLoad(struct engineGraphicObjectVBOCage *this){
	glGenBuffers(1, &this->glId);
	glBindBuffer(GL_ARRAY_BUFFER, this->glId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->length, this->vertices, GL_STATIC_DRAW);
	engineGraphicEngineMemoryResetVBO();
}

// IBO作成
static void egoIBOLoad(struct engineGraphicObjectIBOCage *this){
	glGenBuffers(1, &this->glId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->glId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * this->length, this->indexes, GL_STATIC_DRAW);
	engineGraphicEngineMemoryResetIBO();
}

// テクスチャ作成
static void texDataLoad(struct engineGraphicObjectTexData *this){
	if(this->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL){
		// 読み込み中のデフォルトテクスチャ設定
		this->pluginTextureLocal.glId = localGlobal.defaultTexture.glId;
		this->pluginTextureLocal.w = localGlobal.defaultTexture.w;
		this->pluginTextureLocal.h = localGlobal.defaultTexture.h;
		// テクスチャロード
		this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADING;
		platformPluginTextureLocal(this, this->arg.pluginTextureLocal.src, texDataLocalCallback);
	}else if(this->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT){
		// 読み込み中のデフォルトテクスチャ設定
		this->pluginTextureFont.glId = localGlobal.defaultTexture.glId;
		this->pluginTextureFont.w = localGlobal.defaultTexture.w;
		this->pluginTextureFont.h = localGlobal.defaultTexture.h;
		// テクスチャロード
		this->status = ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADING;
		platformPluginTextureFont(this, this->arg.pluginTextureFont.fontSetId, this->arg.pluginTextureFont.letterList, this->arg.pluginTextureFont.letterLenght, texDataFontCallback);
	}else if(this->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEST){
	}
}

// ----------------------------------------------------------------

// 3DオブジェクトTex読み込み完了確認
bool engineGraphicObjectTexIsComplete(engineGraphicObjectTexId egoId){
	struct engineGraphicObjectTexCage *temp = localGlobal.egoTexList;
	while(temp != NULL){
		if(temp->egoId == egoId){
			if(temp->data == NULL){return false;}
			return (temp->data->status == ENGINEGRAPHICOBJECTTEXDATASTATUS_LOADED);
		}
		temp = temp->next;
	}
	return false;
}

// ----------------------------------------------------------------

// 3DオブジェクトVBO作成
engineGraphicObjectVBOId engineGraphicObjectVBOCreate(int length, GLfloat *vertices){
	// データ作成
	struct engineGraphicObjectVBOCage *obj = (struct engineGraphicObjectVBOCage*)engineUtilMemoryInfoCalloc("engineGraphicObject vbo1", 1, sizeof(struct engineGraphicObjectVBOCage));
	obj->egoId = ++localGlobal.egoIdCount;
	obj->length = length;
	size_t size = length * sizeof(GLfloat);
	obj->vertices = (GLfloat*)engineUtilMemoryInfoMalloc("engineGraphicObject vbo2", size);
	memcpy(obj->vertices, vertices, size);
	//for(int i = 0; i < length; i++){obj->vertices[i] = (GLfloat)vertices[i];}
	egoVBOLoad(obj);
	// リスト登録
	if(localGlobal.egoVBOList == NULL){
		localGlobal.egoVBOList = obj;
	}else{
		struct engineGraphicObjectVBOCage *temp = localGlobal.egoVBOList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj->egoId;
}

// 3DオブジェクトIBO作成
engineGraphicObjectIBOId engineGraphicObjectIBOCreate(int length, GLushort *indexes){
	// データ作成
	struct engineGraphicObjectIBOCage *obj = (struct engineGraphicObjectIBOCage*)engineUtilMemoryInfoCalloc("engineGraphicObject ibo1", 1, sizeof(struct engineGraphicObjectIBOCage));
	obj->egoId = ++localGlobal.egoIdCount;
	obj->length = length;
	size_t size = length * sizeof(GLushort);
	obj->indexes = (GLushort*)engineUtilMemoryInfoMalloc("engineGraphicObject ibo2", size);
	memcpy(obj->indexes, indexes, size);
	//for(int i = 0; i < length; i++){obj->indexes[i] = (GLushort)indexes[i];}
	egoIBOLoad(obj);
	// リスト登録
	if(localGlobal.egoIBOList == NULL){
		localGlobal.egoIBOList = obj;
	}else{
		struct engineGraphicObjectIBOCage *temp = localGlobal.egoIBOList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj->egoId;
}

// テクスチャ情報作成
static struct engineGraphicObjectTexData *texDataCreate(struct engineGraphicObjectTexArg *arg){
	// 重複確認
	struct engineGraphicObjectTexData *temp = localGlobal.texDataList;
	while(temp != NULL){
		if(arg->type == temp->arg.type){
			if(arg->type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL){
				if(strcmp(temp->arg.pluginTextureLocal.src, arg->pluginTextureLocal.src) == 0){return temp;}
			}else if(arg->type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT){
				bool isSame = false;
				if(!isSame && temp->arg.pluginTextureFont.fontSetId == arg->pluginTextureFont.fontSetId){isSame = true;}
				if(!isSame && strcmp(temp->arg.pluginTextureFont.letterList, arg->pluginTextureFont.letterList) == 0){isSame = true;}
				if(!isSame && temp->arg.pluginTextureFont.letterLenght == arg->pluginTextureFont.letterLenght){isSame = true;}
				if(isSame){return temp;}
			}else if(arg->type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEST){
				return temp;
			}
		}
		temp = temp->next;
	}
	// 重複がなければ新規作成
	struct engineGraphicObjectTexData *obj = (struct engineGraphicObjectTexData*)engineUtilMemoryInfoCalloc("engineGraphicObject tex2", 1, sizeof(struct engineGraphicObjectTexData));
	obj->arg.type = arg->type;
	if(arg->type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL){
		size_t length = ((int)strlen(arg->pluginTextureLocal.src) + 1) * sizeof(char);
		obj->arg.pluginTextureLocal.src = (char*)engineUtilMemoryInfoMalloc("engineGraphicObject tex3", length);
		memcpy(obj->arg.pluginTextureLocal.src, arg->pluginTextureLocal.src, length);
	}else if(arg->type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT){
		size_t length = ((int)strlen(arg->pluginTextureFont.letterList) + 1) * sizeof(char);
		obj->arg.pluginTextureFont.letterList = (char*)engineUtilMemoryInfoMalloc("engineGraphicObject tex3", length);
		memcpy(obj->arg.pluginTextureFont.letterList, arg->pluginTextureFont.letterList, length);
		obj->arg.pluginTextureFont.fontSetId = arg->pluginTextureFont.fontSetId;
		obj->arg.pluginTextureFont.letterLenght = arg->pluginTextureFont.letterLenght;
	}else if(arg->type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEST){
	}
	texDataLoad(obj);
	// リスト登録
	if(localGlobal.texDataList == NULL){
		localGlobal.texDataList = obj;
	}else{
		struct engineGraphicObjectTexData *temp = localGlobal.texDataList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj;
}

// 3DオブジェクトTex作成
static engineGraphicObjectTexId texCageCreate(struct engineGraphicObjectTexArg *arg, enum engineGraphicObjectTexType type){
	// データ作成
	struct engineGraphicObjectTexCage *obj = (struct engineGraphicObjectTexCage*)engineUtilMemoryInfoCalloc("engineGraphicObject tex1", 1, sizeof(struct engineGraphicObjectTexCage));
	obj->egoId = ++localGlobal.egoIdCount;
	obj->data = texDataCreate(arg);
	obj->type = type;
	// リスト登録
	if(localGlobal.egoTexList == NULL){
		localGlobal.egoTexList = obj;
	}else{
		struct engineGraphicObjectTexCage *temp = localGlobal.egoTexList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = obj;
	}
	obj->next = NULL;
	return obj->egoId;
}

// 3DオブジェクトTex作成
engineGraphicObjectTexId engineGraphicObjectTexCreateLocal(char *src, enum engineGraphicObjectTexType type){
	struct engineGraphicObjectTexArg arg;
	arg.type = ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL;
	arg.pluginTextureLocal.src = src;
	return texCageCreate(&arg, type);
}

// 3DオブジェクトTex作成
engineGraphicObjectTexId engineGraphicObjectTexCreateFont(int fontSetId, char *letterList, int letterLenght, enum engineGraphicObjectTexType type){
	struct engineGraphicObjectTexArg arg;
	arg.type = ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT;
	arg.pluginTextureFont.fontSetId = fontSetId;
	arg.pluginTextureFont.letterList = letterList;
	arg.pluginTextureFont.letterLenght = letterLenght;
	return texCageCreate(&arg, type);
}

// ----------------------------------------------------------------

// VBOID取得
bool engineGraphicObjectVBOGetGLId(engineGraphicObjectVBOId egoId, GLuint *glId){
	if(glId == NULL){return false;}
	struct engineGraphicObjectVBOCage *temp = localGlobal.egoVBOList;
	while(temp != NULL){
		if(temp->egoId == egoId){
			*glId = temp->glId;
			return true;
		}
		temp = temp->next;
	}
	return false;
}

// IBOID取得
bool engineGraphicObjectIBOGetGLId(engineGraphicObjectIBOId egoId, GLuint *glId){
	if(glId == NULL){return false;}
	struct engineGraphicObjectIBOCage *temp = localGlobal.egoIBOList;
	while(temp != NULL){
		if(temp->egoId == egoId){
			*glId = temp->glId;
			return true;
		}
		temp = temp->next;
	}
	return false;
}

// テクスチャID取得
bool engineGraphicObjectTexGetGLId(engineGraphicObjectTexId egoId, GLuint *glId, enum engineGraphicObjectTexType *type){
	if(glId == NULL && type == NULL){return false;}
	struct engineGraphicObjectTexCage *temp = localGlobal.egoTexList;
	while(temp != NULL){
		if(temp->egoId == egoId){
			if(temp->data == NULL){return false;}
			if(glId != NULL && temp->data->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTURELOCAL){*glId = temp->data->pluginTextureLocal.glId;}
			if(glId != NULL && temp->data->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEXTUREFONT){*glId = temp->data->pluginTextureFont.glId;}
			if(glId != NULL && temp->data->arg.type == ENGINEGRAPHICOBJECTTEXARGTYPE_PLUGINTEST){*glId = localGlobal.defaultTexture.glId;}
			if(type != NULL){*type = temp->type;}
			return true;
		}
		temp = temp->next;
	}
	return false;
}

// ----------------------------------------------------------------

// 3DオブジェクトVBO除去
void engineGraphicObjectVBODispose(engineGraphicObjectVBOId egoId){
	if(egoId == 0){return;}
	int freeCount = 0;
	struct engineGraphicObjectVBOCage *prev = NULL;
	struct engineGraphicObjectVBOCage *temp = localGlobal.egoVBOList;
	while(temp != NULL){
		if(temp->egoId == egoId){
			// リストから要素を外す
			struct engineGraphicObjectVBOCage *dispose = temp;
			temp = temp->next;
			if(prev == NULL){localGlobal.egoVBOList = temp;}
			else{prev->next = temp;}
			// 要素の除去
			egoVBOFree(dispose);
			freeCount++;
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
	if(freeCount < 1){trace("vbo free error -- no such pointer");}
	if(freeCount > 1){trace("vbo free error -- too meny pointer");}
}

// 3DオブジェクトIBO除去
void engineGraphicObjectIBODispose(engineGraphicObjectIBOId egoId){
	if(egoId == 0){return;}
	int freeCount = 0;
	struct engineGraphicObjectIBOCage *prev = NULL;
	struct engineGraphicObjectIBOCage *temp = localGlobal.egoIBOList;
	while(temp != NULL){
		if(temp->egoId == egoId){
			// リストから要素を外す
			struct engineGraphicObjectIBOCage *dispose = temp;
			temp = temp->next;
			if(prev == NULL){localGlobal.egoIBOList = temp;}
			else{prev->next = temp;}
			// 要素の除去
			egoIBOFree(dispose);
			freeCount++;
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
	if(freeCount < 1){trace("ibo free error -- no such pointer");}
	if(freeCount > 1){trace("ibo free error -- too meny pointer");}
}

// テクスチャ情報除去
static void texDataDispose(struct engineGraphicObjectTexData *this){
	// 使用中確認
	struct engineGraphicObjectTexCage *tempTex = localGlobal.egoTexList;
	while(tempTex != NULL){
		if(tempTex->data == this){return;}
		tempTex = tempTex->next;
	}
	// 誰も使っていなければ除去
	struct engineGraphicObjectTexData *prevData = NULL;
	struct engineGraphicObjectTexData *tempData = localGlobal.texDataList;
	while(tempData != NULL){
		if(tempData == this){
			// リストから要素を外す
			tempData = tempData->next;
			if(prevData == NULL){localGlobal.texDataList = tempData;}
			else{prevData->next = tempData;}
		}else{
			prevData = tempData;
			tempData = tempData->next;
		}
	}
	// 除去
	texDataFree(this);
}

// 3DオブジェクトTex除去
void engineGraphicObjectTexDispose(engineGraphicObjectTexId egoId){
	if(egoId == 0){return;}
	int freeCount = 0;
	struct engineGraphicObjectTexCage *prev = NULL;
	struct engineGraphicObjectTexCage *temp = localGlobal.egoTexList;
	while(temp != NULL){
		if(temp->egoId == egoId){
			// リストから要素を外す
			struct engineGraphicObjectTexCage *dispose = temp;
			temp = temp->next;
			if(prev == NULL){localGlobal.egoTexList = temp;}
			else{prev->next = temp;}
			// 要素の除去
			texDataDispose(dispose->data);
			egoTexFree(dispose);
			freeCount++;
		}else{
			prev = temp;
			temp = temp->next;
		}
	}
	if(freeCount < 1){trace("tex free error -- no such pointer");}
	if(freeCount > 1){trace("tex free error -- too meny pointer");}
}

// 全3Dオブジェクト除去
void engineGraphicObjectDispose(void){
	struct engineGraphicObjectVBOCage *tempVBO = localGlobal.egoVBOList;
	while(tempVBO != NULL){
		struct engineGraphicObjectVBOCage *dispose = tempVBO;
		tempVBO = tempVBO->next;
		// 要素の除去
		egoVBOFree(dispose);
	}
	localGlobal.egoVBOList = NULL;

	struct engineGraphicObjectIBOCage *tempIBO = localGlobal.egoIBOList;
	while(tempIBO != NULL){
		struct engineGraphicObjectIBOCage *dispose = tempIBO;
		tempIBO = tempIBO->next;
		// 要素の除去
		egoIBOFree(dispose);
	}
	localGlobal.egoIBOList = NULL;

	struct engineGraphicObjectTexCage *tempTex = localGlobal.egoTexList;
	while(tempTex != NULL){
		struct engineGraphicObjectTexCage *dispose = tempTex;
		tempTex = tempTex->next;
		// 要素の除去
		egoTexFree(dispose);
	}
	localGlobal.egoTexList = NULL;

	struct engineGraphicObjectTexData *tempData = localGlobal.texDataList;
	while(tempData != NULL){
		struct engineGraphicObjectTexData *dispose = tempData;
		tempData = tempData->next;
		// 要素の除去
		texDataFree(dispose);
	}
	localGlobal.texDataList = NULL;

	// デフォルトテクスチャ除去
	glDeleteTextures(1, &localGlobal.defaultTexture.glId);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 全データロード再読み込み
void engineGraphicObjectReload(void){
	struct engineGraphicObjectVBOCage *tempVBO = localGlobal.egoVBOList;
	struct engineGraphicObjectIBOCage *tempIBO = localGlobal.egoIBOList;
	struct engineGraphicObjectTexData *tempTex = localGlobal.texDataList;
	while(tempVBO != NULL){egoVBOLoad(tempVBO); tempVBO = tempVBO->next;}
	while(tempIBO != NULL){egoIBOLoad(tempIBO); tempIBO = tempIBO->next;}
	while(tempTex != NULL){texDataLoad(tempTex); tempTex = tempTex->next;}

	// 読み込み中に使うデフォルトテクスチャ作成
	byte data[4] = {0xff, 0xff, 0xff, 0xff};
	localGlobal.defaultTexture.w = 1;
	localGlobal.defaultTexture.h = 1;
	glGenTextures(1, &localGlobal.defaultTexture.glId);
	corePluginTextureIsBind(localGlobal.defaultTexture.glId);
	glBindTexture(GL_TEXTURE_2D, localGlobal.defaultTexture.glId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, localGlobal.defaultTexture.w, localGlobal.defaultTexture.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

