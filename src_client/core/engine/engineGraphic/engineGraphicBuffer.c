#include "../../library.h"
#include "platform.h"
#include "../engineMath/engineMath.h"
#include "engineGraphic.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	double *buffVert;
	double *buffClor;
	double *buffTexc;
	int *buffFace;
	int buffVertIndex;
	int buffClorIndex;
	int buffTexcIndex;
	int buffFaceIndex;
	int buffVertLength;
	int buffClorLength;
	int buffTexcLength;
	int buffFaceLength;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// バッファ作成開始
void engineGraphicBufferBegin(void){
	localGlobal.buffVertIndex = 0;
	localGlobal.buffClorIndex = 0;
	localGlobal.buffTexcIndex = 0;
	localGlobal.buffFaceIndex = 0;
}

// バッファ作成完了
void engineGraphicBufferEnd(engineGraphicObjectVBOId *egoIdVert, engineGraphicObjectVBOId *egoIdClor, engineGraphicObjectVBOId *egoIdTexc, engineGraphicObjectIBOId *egoIdFace){
	if(egoIdVert != NULL){engineGraphicObjectVBODispose(*egoIdVert); *egoIdVert = engineGraphicObjectVBOCreate(localGlobal.buffVertIndex, localGlobal.buffVert);}
	if(egoIdClor != NULL){engineGraphicObjectVBODispose(*egoIdClor); *egoIdClor = engineGraphicObjectVBOCreate(localGlobal.buffClorIndex, localGlobal.buffClor);}
	if(egoIdTexc != NULL){engineGraphicObjectVBODispose(*egoIdTexc); *egoIdTexc = engineGraphicObjectVBOCreate(localGlobal.buffTexcIndex, localGlobal.buffTexc);}
	if(egoIdFace != NULL){engineGraphicObjectIBODispose(*egoIdFace); *egoIdFace = engineGraphicObjectIBOCreate(localGlobal.buffFaceIndex, localGlobal.buffFace);}
}

// ----------------------------------------------------------------

// 頂点座標配列に要素追加
void engineGraphicBufferPushVert(double x, double y, double z){
	if(localGlobal.buffVertLength < localGlobal.buffVertIndex + 3){
		int length = localGlobal.buffVertLength + 300;
		double *array = (double*)malloc(length * sizeof(double));
		if(localGlobal.buffVertLength > 0){
			memcpy(array, localGlobal.buffVert, localGlobal.buffVertLength * sizeof(double));
			free(localGlobal.buffVert);
		}
		localGlobal.buffVertLength = length;
		localGlobal.buffVert = array;
	}
	*(localGlobal.buffVert + localGlobal.buffVertIndex++) = x;
	*(localGlobal.buffVert + localGlobal.buffVertIndex++) = y;
	*(localGlobal.buffVert + localGlobal.buffVertIndex++) = z;
}

// 色彩配列に要素追加
void engineGraphicBufferPushClor(double r, double g, double b){
	if(localGlobal.buffClorLength < localGlobal.buffClorIndex + 3){
		int length = localGlobal.buffClorLength + 300;
		double *array = (double*)malloc(length * sizeof(double));
		if(localGlobal.buffClorLength > 0){
			memcpy(array, localGlobal.buffClor, localGlobal.buffClorLength * sizeof(double));
			free(localGlobal.buffClor);
		}
		localGlobal.buffClorLength = length;
		localGlobal.buffClor = array;
	}
	*(localGlobal.buffClor + localGlobal.buffClorIndex++) = r;
	*(localGlobal.buffClor + localGlobal.buffClorIndex++) = g;
	*(localGlobal.buffClor + localGlobal.buffClorIndex++) = b;
}

// テクスチャ座標配列に要素追加
void engineGraphicBufferPushTexc(double u, double v){
	if(localGlobal.buffTexcLength < localGlobal.buffTexcIndex + 2){
		int length = localGlobal.buffTexcLength + 200;
		double *array = (double*)malloc(length * sizeof(double));
		if(localGlobal.buffTexcLength > 0){
			memcpy(array, localGlobal.buffTexc, localGlobal.buffTexcLength * sizeof(double));
			free(localGlobal.buffTexc);
		}
		localGlobal.buffTexcLength = length;
		localGlobal.buffTexc = array;
	}
	*(localGlobal.buffTexc + localGlobal.buffTexcIndex++) = u;
	*(localGlobal.buffTexc + localGlobal.buffTexcIndex++) = v;
}

// 頂点番号配配列に要素追加
void engineGraphicBufferPushFace(int index, int t0, int t1, int t2){
	if(localGlobal.buffFaceLength < localGlobal.buffFaceIndex + 3){
		int length = localGlobal.buffFaceLength + 150;
		int *array = (int*)malloc(length * sizeof(int));
		if(localGlobal.buffFaceLength > 0){
			memcpy(array, localGlobal.buffFace, localGlobal.buffFaceLength * sizeof(int));
			free(localGlobal.buffFace);
		}
		localGlobal.buffFaceLength = length;
		localGlobal.buffFace = array;
	}
	*(localGlobal.buffFace + localGlobal.buffFaceIndex++) = index + t0;
	*(localGlobal.buffFace + localGlobal.buffFaceIndex++) = index + t1;
	*(localGlobal.buffFace + localGlobal.buffFaceIndex++) = index + t2;
}

// ----------------------------------------------------------------

// 頂点座標配列に四角形の要素追加
void engineGraphicBufferPushTetraVert(double x, double y, double w, double h){
	engineGraphicBufferPushVert(x + 0, y + 0, 0.0);
	engineGraphicBufferPushVert(x + 0, y + h, 0.0);
	engineGraphicBufferPushVert(x + w, y + h, 0.0);
	engineGraphicBufferPushVert(x + w, y + 0, 0.0);
}

// テクスチャ座標配列に要素追加
void engineGraphicBufferPushTetraTexc(int imgw, int imgh, double u, double v, double w, double h){
	engineGraphicBufferPushTexc((u + 0) / imgw, (v + 0) / imgh);
	engineGraphicBufferPushTexc((u + 0) / imgw, (v + h) / imgh);
	engineGraphicBufferPushTexc((u + w) / imgw, (v + h) / imgh);
	engineGraphicBufferPushTexc((u + w) / imgw, (v + 0) / imgh);
}

// 頂点番号配配列に要素追加
void engineGraphicBufferPushTetraFace(int index){
	engineGraphicBufferPushFace(index, 0, 1, 2);
	engineGraphicBufferPushFace(index, 2, 3, 0);
}

// ----------------------------------------------------------------

// VBOバッファ配列内の位置獲得
int engineGraphicBufferVretIndexGet(void){
	return localGlobal.buffVertIndex / 3;
}

// IBOバッファ配列内の位置獲得
int engineGraphicBufferFaceIndexGet(void){
	return localGlobal.buffFaceIndex / 3;
}

// ----------------------------------------------------------------

// バッファ片付け
void engineGraphicBufferClean(void){
	free(localGlobal.buffVert);
	free(localGlobal.buffClor);
	free(localGlobal.buffTexc);
	free(localGlobal.buffFace);
	localGlobal.buffVertLength = 0;
	localGlobal.buffClorLength = 0;
	localGlobal.buffTexcLength = 0;
	localGlobal.buffFaceLength = 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
