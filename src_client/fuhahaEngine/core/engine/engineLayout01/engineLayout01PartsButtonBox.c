#include "../../library.h"
#include "../../engine/engineMath/engineMath.h"
#include "../../engine/engineCtrl/engineCtrl.h"
#include "../../engine/engineGraphic/engineGraphic.h"
#include "../../engine/engineLayout01/engineLayout01.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 描画
static void draw(struct engineLayout01Trans *that, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	struct engineLayout01ButtonBox *this = (struct engineLayout01ButtonBox*)((char*)that - offsetof(struct engineLayout01ButtonBox, trans));
	engineLayout01ButtonBoxDraw(this, mat, color);
}

// 破棄
static void dispose(struct engineLayout01Trans *that){
	struct engineLayout01ButtonBox *this = (struct engineLayout01ButtonBox*)((char*)that - offsetof(struct engineLayout01ButtonBox, trans));
	engineLayout01ButtonBoxDispose(this);
}

// ----------------------------------------------------------------

// 表示箱付きボタン構造体 初期化
void engineLayout01ButtonBoxInit(struct engineLayout01ButtonBox *this){
	engineLayout01ButtonInit(&this->super);
	engineLayout01TransInit(&this->trans);
	this->trans.draw = draw;
	this->trans.dispose = dispose;
	engineLayout01ImageFrameInit(&this->imgNormal);
	engineLayout01ImageFrameInit(&this->imgSelect);
	engineLayout01ImageFrameInit(&this->imgActibve);
	engineLayout01ImageFrameInit(&this->imgInactive);
	engineLayout01ImageTextInit(&this->imgText);
	this->imgNormal.createArrayInfo.texture.tu = TEXPOS_SYSTEM_BUTTONBASICNORMAL_X;
	this->imgNormal.createArrayInfo.texture.tv = TEXPOS_SYSTEM_BUTTONBASICNORMAL_Y;
	this->imgNormal.createArrayInfo.texture.tw = TEXPOS_SYSTEM_BUTTONBASICNORMAL_W;
	this->imgNormal.createArrayInfo.texture.th = TEXPOS_SYSTEM_BUTTONBASICNORMAL_H;
	this->imgSelect.createArrayInfo.texture.tu = TEXPOS_SYSTEM_BUTTONBASICSELECT_X;
	this->imgSelect.createArrayInfo.texture.tv = TEXPOS_SYSTEM_BUTTONBASICSELECT_Y;
	this->imgSelect.createArrayInfo.texture.tw = TEXPOS_SYSTEM_BUTTONBASICSELECT_W;
	this->imgSelect.createArrayInfo.texture.th = TEXPOS_SYSTEM_BUTTONBASICSELECT_H;
	this->imgActibve.createArrayInfo.texture.tu = TEXPOS_SYSTEM_BUTTONBASICACTIVE_X;
	this->imgActibve.createArrayInfo.texture.tv = TEXPOS_SYSTEM_BUTTONBASICACTIVE_Y;
	this->imgActibve.createArrayInfo.texture.tw = TEXPOS_SYSTEM_BUTTONBASICACTIVE_W;
	this->imgActibve.createArrayInfo.texture.th = TEXPOS_SYSTEM_BUTTONBASICACTIVE_H;
	this->imgInactive.createArrayInfo.texture.tu = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_X;
	this->imgInactive.createArrayInfo.texture.tv = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_Y;
	this->imgInactive.createArrayInfo.texture.tw = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_W;
	this->imgInactive.createArrayInfo.texture.th = TEXPOS_SYSTEM_BUTTONBASICINACTIVE_H;
	this->imgNormal.createArrayInfo.border.bt = 24;
	this->imgNormal.createArrayInfo.border.bb = 24;
	this->imgSelect.createArrayInfo.border.bt = 24;
	this->imgSelect.createArrayInfo.border.bb = 24;
	this->imgActibve.createArrayInfo.border.bt = 24;
	this->imgActibve.createArrayInfo.border.bb = 24;
	this->imgInactive.createArrayInfo.border.bt = 24;
	this->imgInactive.createArrayInfo.border.bb = 24;
	this->imgText.createArrayInfo.xalign = 0;
	this->imgText.createArrayInfo.yalign = 0;
	this->select = false;
}

// 表示箱付きボタン構造体 静的位置設定
void engineLayout01ButtonBoxSetPosition(struct engineLayout01ButtonBox *this, double x, double y, double w, double h){
	engineLayout01ButtonSetPosition(&this->super, x, y, w, h);
	this->imgNormal.x = x;
	this->imgNormal.y = y;
	this->imgNormal.w = w;
	this->imgNormal.h = h;
	this->imgSelect.x = x;
	this->imgSelect.y = y;
	this->imgSelect.w = w;
	this->imgSelect.h = h;
	this->imgActibve.x = x;
	this->imgActibve.y = y;
	this->imgActibve.w = w;
	this->imgActibve.h = h;
	this->imgInactive.x = x;
	this->imgInactive.y = y;
	this->imgInactive.w = w;
	this->imgInactive.h = h;
}

// 表示箱付きボタン構造体 計算
void engineLayout01ButtonBoxCalc(struct engineLayout01ButtonBox *this){
	engineLayout01ButtonCalc(&this->super);
}

// 表示箱付きボタン構造体 タッチ情報を使い回す計算
void engineLayout01ButtonBoxSubCalc(struct engineLayout01ButtonBox *this, struct engineCtrlTouch *t, bool clickable){
	engineLayout01ButtonSubCalc(&this->super, t, clickable);
}

// 配列に表示箱付きボタンの描画情報を追加
void engineLayout01ButtonBoxCreateArray(struct engineLayout01ButtonBox *this, char *text){
	engineLayout01ImageFrameCreateArray(&this->imgNormal, this->super.x, this->super.y, this->super.w, this->super.h);
	engineLayout01ImageFrameCreateArray(&this->imgSelect, this->super.x, this->super.y, this->super.w, this->super.h);
	engineLayout01ImageFrameCreateArray(&this->imgActibve, this->super.x, this->super.y, this->super.w, this->super.h);
	engineLayout01ImageFrameCreateArray(&this->imgInactive, this->super.x, this->super.y, this->super.w, this->super.h);
	engineLayout01ImageTextCreateArray(&this->imgText, 0, 0, text);
	this->imgNormal.trans.parent = &this->trans;
	this->imgSelect.trans.parent = &this->trans;
	this->imgActibve.trans.parent = &this->trans;
	this->imgInactive.trans.parent = &this->trans;
	this->imgText.trans.parent = &this->trans;
}

// ----------------------------------------------------------------

// 表示箱付きボタン構造体 描画
void engineLayout01ButtonBoxDraw(struct engineLayout01ButtonBox *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	bool isInactive = (this->super.inactive && !this->super.active && !this->select);
	// 領域内確認行列読み込み
	struct engineMathMatrix44 tempMat1;
	engineMathMat4Copy(&tempMat1, mat);
	engineLayout01TransMultMatrix(&this->trans, &tempMat1);
	engineLayout01ButtonSetMatrix(&this->super, &tempMat1);
	// ボタン枠描画
	if(isInactive){engineLayout01ImageFrameDraw(&this->imgInactive, mat, color);}
	else if(this->super.active){engineLayout01ImageFrameDraw(&this->imgActibve, mat, color);}
	else if(this->select){engineLayout01ImageFrameDraw(&this->imgSelect, mat, color);}
	else{engineLayout01ImageFrameDraw(&this->imgNormal, mat, color);}
	// 文字列描画
	double x = this->super.x + this->super.w * 0.5;
	double y = this->super.y + this->super.h * 0.5 + (this->super.active ? 1 : -1);
	double bright = isInactive ? 0.5 : 0.0;
	engineLayout01TransSetTranslate(&this->imgText.trans, x, y, 0.0);
	engineLayout01TransSetColorRgba(&this->imgText.trans, bright, bright, bright, 1.0);
	engineLayout01ImageTextDraw(&this->imgText, mat, color);
}

// 表示箱付きボタン構造体 破棄
void engineLayout01ButtonBoxDispose(struct engineLayout01ButtonBox *this){
	engineLayout01ImageFrameDispose(&this->imgNormal);
	engineLayout01ImageFrameDispose(&this->imgSelect);
	engineLayout01ImageFrameDispose(&this->imgActibve);
	engineLayout01ImageFrameDispose(&this->imgInactive);
	engineLayout01ImageTextDispose(&this->imgText);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

