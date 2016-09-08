#include "../library.h"
#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 静的データリスト
static struct dataUnit{
	int backgroundKind;
	struct dataBackground data;
	int e3dIdImage;
	int loading;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;
	int faceNum;
	int action;
} *unit = NULL;

// ----------------------------------------------------------------

// ロード完了時コールバック
static void callback(void *param, void *buff, size_t size){
	struct dataUnit *this = param;
	if(this->loading > 0){
		if(buff != NULL){
			// ロード成功
			unsigned char *rawBuff = base64decode(buff, sizeof(struct dataBackground));
			memcpy(&this->data, rawBuff, sizeof(struct dataBackground));
			char path[256];
			sprintf(path, "img/background/%s.png", this->data.code);
			this->e3dIdImage = engineGraphicObjectTexCreate(path, ENGINEGRAPHICOBJECTTEXTYPE_LINEAR);
		}
		this->loading = 0;
	}else if(this->loading < 0){
		// ロード中止
		engineUtilMemoryFree(this);
	}
	if(buff != NULL){engineUtilMemoryFree(buff);}
}

// データが存在しなかった場合は作成する
static void setBackground(int backgroundKind){
	if(unit->backgroundKind != backgroundKind){
		engineGraphicObjectTexDispose(unit->e3dIdImage);
		unit->backgroundKind = backgroundKind;
		// ロード開始
		char path[256];
		sprintf(path, "data/background/%02d.dat", backgroundKind);
		unit->loading = 1;
		platformPluginDataLocal(unit, path, callback);
	}
}

// バッファ作成
static void createBuffer(struct dataUnit *this){
	int status = (this->loading == 0) ? 1 : 0;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ステンシルマスク作成
		engineGraphicBufferPushTetraFace(engineGraphicBufferVretIndexGet());
		engineGraphicBufferPushTetraVert(0, 0, global.screen.w, global.screen.h);
		engineGraphicBufferPushTetraTexc(1, 1, 0, 0, 1, 1);

		if(this->bufferStatus){
			this->faceNum = 0;
			int w05 = this->data.w * 0.5;
			for(int x = 0; x < global.screen.w + w05; x += w05){
				engineGraphicBufferPushTetraFace(engineGraphicBufferVretIndexGet());
				engineGraphicBufferPushTetraVert(x, 0, w05, global.screen.h);
				engineGraphicBufferPushTetraTexc(this->data.w, this->data.h, 0, (this->data.h - global.screen.h * 2) * 0.5, this->data.w, global.screen.h * 2);
				this->faceNum += 2;
			}
		}

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// ----------------------------------------------------------------

// 有効化設定
void dataBackgroundActive(bool active){
	if(active && unit == NULL){
		// 有効化
		unit = (struct dataUnit*)engineUtilMemoryCalloc(1, sizeof(struct dataUnit));
		unit->backgroundKind = -1;
	}else if(!active && unit != NULL){
		// 無効化
		if(unit->loading == 0){
			engineGraphicObjectTexDispose(unit->e3dIdImage);
			engineGraphicObjectVBODispose(unit->e3dIdVert);
			engineGraphicObjectVBODispose(unit->e3dIdTexc);
			engineGraphicObjectIBODispose(unit->e3dIdFace);
			engineUtilMemoryFree(unit);
		}else{
			// ロードが完了していないのでコールバックで破棄
			unit->loading = -1;
		}
		unit = NULL;
	}
}

// 使用設定
void dataBackgroundPreset(int backgroundKind){
	if(unit == NULL){exit(1);}
	if(backgroundKind < 0){return;}
	setBackground(backgroundKind);
}

// 描画
void dataBackgroundDraw(int backgroundKind){
	if(unit == NULL){exit(1);}
	if(backgroundKind < 0){return;}
	setBackground(backgroundKind);

	// 描画準備
	createBuffer(unit);

	// ステンシルマスク描画
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
	engineGraphicEngineSetStencilMode(ENGINEGRAPHICENGINEMODESTENCIL_MASK_1);
	engineGraphicEngineBindTexture(unit->e3dIdImage);
	engineGraphicEngineBindVertVBO(unit->e3dIdVert);
	engineGraphicEngineBindTexcVBO(unit->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(unit->e3dIdFace);
	engineGraphicEngineSetMatrix(&e3dMatOrtho);
	engineGraphicEngineDrawIndex(0 * 3, 2 * 3);
	engineGraphicEngineSetStencilMode(ENGINEGRAPHICENGINEMODESTENCIL_READ_EQ1);

	if(unit->loading == 0){
		struct engineMathMatrix44 tempmat;
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, -(unit->action++ % (unit->data.w >> 1)), 0, 0);
		engineGraphicEngineSetMatrix(&tempmat);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
		engineGraphicEngineDrawIndex(2 * 3, unit->faceNum * 3);
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

