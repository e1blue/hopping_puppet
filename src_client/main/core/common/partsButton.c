#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ボタン初期化
void partsButtonInit(struct partsButton *this){
	this->movex = 0;
	this->movey = 0;
	this->x = 0;
	this->y = 0;
	this->w = 0;
	this->h = 0;
	this->active = false;
	this->inactive = false;
	this->trigger = false;
	this->child = NULL;
	this->zKey = false;
	this->xKey = false;
	this->cKey = false;
	this->vKey = false;
	this->inner = true;
}

// ボタン静的位置設定
void partsButtonPosition(struct partsButton *this, int x, int y, int w, int h){
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

// ボタン動的位置設定
void partsButtonMove(struct partsButton *this, int x, int y){
	this->movex = x;
	this->movey = y;
}

// ボタン計算
void partsButtonCalc(struct partsButton *this){
	struct engineCtrlTouch *t = engineCtrlTouchGet(ENGINECTRLTOUCHID_BUTTON);
	partsButtonSubCalc(this, t, true);
	if(t != NULL){
		if(this->active && !t->active){
			engineCtrlTouchOwn();
		}else if(!t->dn){
			engineCtrlTouchFree();
		}
	}
}

// タッチ情報を使い回しての計算
void partsButtonSubCalc(struct partsButton *this, struct engineCtrlTouch *t, bool clickable){
	if(this->inactive || !clickable){
		// ボタン無効状態
		this->active = false;
	}else if((this->zKey && global.key.zb.isActive) || (this->xKey && global.key.xb.isActive) || (this->cKey && global.key.cb.isActive) || (this->vKey && global.key.vb.isActive)){
		// 対応キー押下中
		this->active = true;
	}else if(t == NULL){
		// タッチ無し
		this->active = false;
	}else if(t->dn){
		// ボタン押下中
		double x0 = this->movex + this->x;
		double y0 = this->movey + this->y;
		double x1 = x0 + this->w;
		double y1 = y0 + this->h;
		int inner = (x0 < t->screen.x && t->screen.x < x1 && y0 < t->screen.y && t->screen.y < y1);
		this->active = (inner == !!this->inner);
		// 子要素の範囲内では押下状態にならない
		if(this->child != NULL){
			double x0 = this->child->movex + this->child->x - this->child->w * 0.5;
			double y0 = this->child->movey + this->child->y - this->child->h * 0.5;
			double x1 = x0 + this->child->w;
			double y1 = y0 + this->child->h;
			int inner = (x0 < t->screen.x && t->screen.x < x1 && y0 < t->screen.y && t->screen.y < y1);
			this->active = this->active && !(inner == !!this->child->inner);
		}
	}else if(this->active){
		// ボタンを放した瞬間
		this->active = false;
		this->trigger = true;
	}
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ボタン初期化
void partsButtonBoxInit(struct partsButtonBox *this){
	partsButtonInit((struct partsButton*)this);
	this->select = false;
}

// 配列にボタンの描画情報を追加
void partsButtonBoxCreateArray(struct partsButtonBox *this, int x, int y, int w, int h, char *text, double scale){
	partsButtonPosition((struct partsButton*)(this), x, y, w, h);

	this->faceIndex = engineGraphicBufferFaceIndexGet();

	e3dImageBoxCreateArray(NULL, 0, 0, this->super.w, this->super.h, TEXPOS_SYSTEM_BUTTONBASICNORMAL_X, TEXPOS_SYSTEM_BUTTONBASICNORMAL_Y, TEXPOS_SYSTEM_BUTTONBASICNORMAL_W, TEXPOS_SYSTEM_BUTTONBASICNORMAL_H);
	e3dImageBoxCreateArray(NULL, 0, 0, this->super.w, this->super.h, TEXPOS_SYSTEM_BUTTONBASICACTIVE_X, TEXPOS_SYSTEM_BUTTONBASICACTIVE_Y, TEXPOS_SYSTEM_BUTTONBASICACTIVE_W, TEXPOS_SYSTEM_BUTTONBASICACTIVE_H);
	e3dImageBoxCreateArray(NULL, 0, 0, this->super.w, this->super.h, TEXPOS_SYSTEM_BUTTONBASICSELECT_X, TEXPOS_SYSTEM_BUTTONBASICSELECT_Y, TEXPOS_SYSTEM_BUTTONBASICSELECT_W, TEXPOS_SYSTEM_BUTTONBASICSELECT_H);
	e3dImageBoxCreateArray(NULL, 0, 0, this->super.w, this->super.h, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_X, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_Y, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_W, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_H);
	e3dImageTextCreateArray(NULL, this->super.w * 0.5, this->super.h * 0.5, text, scale, 0, 0);

	this->faceNum = engineGraphicBufferFaceIndexGet() - this->faceIndex;
}

// ボタン描画
void partsButtonBoxDraw(struct partsButtonBox *this, struct engineMathMatrix44 *mat, double alpha){
	struct engineMathMatrix44 tempmat;
	engineMathMat4Copy(&tempmat, mat);
	engineMathMat4Translate(&tempmat, this->super.movex + this->super.x, this->super.movey + this->super.y, 0);
	engineGraphicEngineSetMatrix(&tempmat);
	// ボタン枠描画
	int isInactive = (this->super.inactive && !this->super.active && !this->select);
	int index = this->faceIndex + 18 * (isInactive ? 3 : this->super.active ? 1 : this->select ? 2 : 0);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	engineGraphicEngineDrawIndex(index * 3, 18 * 3);

	engineMathMat4Translate(&tempmat, 0, this->super.active ? 1 : -1, 0);
	engineGraphicEngineSetMatrix(&tempmat);
	// 文字列描画
	double bright = isInactive ? 0.5 : 0.0;
	engineGraphicEngineSetColorRgba(bright, bright, bright, alpha);
	engineGraphicEngineDrawIndex((this->faceIndex + 18 * 4) * 3, (this->faceNum - 18 * 4) * 3);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ボタン初期化
void partsButtonPickerInit(struct partsButtonPicker *this, int picked){
	partsButtonInit((struct partsButton*)this);
	this->select = false;
	this->picked = picked;
}

// 配列にボタンの描画情報を追加
void partsButtonPickerCreateArray(struct partsButtonPicker *this, int x, int y, int w, char **textList, int textListLength, double scale){
	partsButtonPosition((struct partsButton*)(this), x, y, w, 30);

	this->faceIndex = engineGraphicBufferFaceIndexGet();

	e3dImageBoxCreateArray(NULL, 28 - 10, 3, this->super.w - (28 - 10), this->super.h - 6, TEXPOS_SYSTEM_BUTTONBASICNORMAL_X, TEXPOS_SYSTEM_BUTTONBASICNORMAL_Y, TEXPOS_SYSTEM_BUTTONBASICNORMAL_W, TEXPOS_SYSTEM_BUTTONBASICNORMAL_H);
	e3dImageBoxCreateArray(NULL, 28 - 10, 3, this->super.w - (28 - 10), this->super.h - 6, TEXPOS_SYSTEM_BUTTONBASICACTIVE_X, TEXPOS_SYSTEM_BUTTONBASICACTIVE_Y, TEXPOS_SYSTEM_BUTTONBASICACTIVE_W, TEXPOS_SYSTEM_BUTTONBASICACTIVE_H);
	e3dImageBoxCreateArray(NULL, 28 - 10, 3, this->super.w - (28 - 10), this->super.h - 6, TEXPOS_SYSTEM_BUTTONBASICSELECT_X, TEXPOS_SYSTEM_BUTTONBASICSELECT_Y, TEXPOS_SYSTEM_BUTTONBASICSELECT_W, TEXPOS_SYSTEM_BUTTONBASICSELECT_H);
	e3dImageBoxCreateArray(NULL, 28 - 10, 3, this->super.w - (28 - 10), this->super.h - 6, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_X, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_Y, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_W, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_H);
	e3dImageCreateArray(NULL, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERNORMAL_X, TEXPOS_SYSTEM_BUTTONPICKERNORMAL_Y, TEXPOS_SYSTEM_BUTTONPICKERNORMAL_W, TEXPOS_SYSTEM_BUTTONPICKERNORMAL_H);
	e3dImageCreateArray(NULL, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERACTIVE_X, TEXPOS_SYSTEM_BUTTONPICKERACTIVE_Y, TEXPOS_SYSTEM_BUTTONPICKERACTIVE_W, TEXPOS_SYSTEM_BUTTONPICKERACTIVE_H);
	e3dImageCreateArray(NULL, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERSELECT_X, TEXPOS_SYSTEM_BUTTONPICKERSELECT_Y, TEXPOS_SYSTEM_BUTTONPICKERSELECT_W, TEXPOS_SYSTEM_BUTTONPICKERSELECT_H);
	e3dImageCreateArray(NULL, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, 0, 0, 28, 30, TEXPOS_SYSTEM_BUTTONPICKERINACTIVE_X, TEXPOS_SYSTEM_BUTTONPICKERINACTIVE_Y, TEXPOS_SYSTEM_BUTTONPICKERINACTIVE_W, TEXPOS_SYSTEM_BUTTONPICKERINACTIVE_H);

	for(int i = 0; i < textListLength; i++){e3dImageTextCreateArray(this->text + i, 28 + 5, this->super.h * 0.5, textList[i], scale, 1, 0);}

	this->faceNum = engineGraphicBufferFaceIndexGet() - this->faceIndex;
}

// ボタン描画
void partsButtonPickerDraw(struct partsButtonPicker *this, struct engineMathMatrix44 *mat, double alpha){
	struct engineMathMatrix44 tempmat;
	engineMathMat4Copy(&tempmat, mat);
	engineMathMat4Translate(&tempmat, this->super.movex + this->super.x, this->super.movey + this->super.y, 0);
	engineGraphicEngineSetMatrix(&tempmat);
	// ボタン枠描画
	int isInactive = (this->super.inactive && !this->super.active && !this->select);
	int type = (isInactive ? 3 : this->super.active ? 1 : this->select ? 2 : 0);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	engineGraphicEngineDrawIndex((this->faceIndex + 18 * type +  2) * 3, 4 * 3);
	engineGraphicEngineDrawIndex((this->faceIndex + 18 * type +  8) * 3, 4 * 3);
	engineGraphicEngineDrawIndex((this->faceIndex + 18 * type + 14) * 3, 4 * 3);
	engineGraphicEngineDrawIndex((this->faceIndex + 18 * 4 + 2 * type) * 3, 2 * 3);

	engineMathMat4Translate(&tempmat, 0, this->super.active ? 1 : -1, 0);
	engineGraphicEngineSetMatrix(&tempmat);
	// 文字列描画
	double bright = isInactive ? 0.5 : 0.0;
	engineGraphicEngineSetColorRgba(bright, bright, bright, alpha);
	e3dImageDraw(this->text + this->picked);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

