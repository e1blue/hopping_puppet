#include "../includeAll.h"
#include "pageAtelier.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 初期化
struct buttonAtelierGacha *buttonAtelierGachaInit(int gachaKind){
	struct buttonAtelierGacha *this = (struct buttonAtelierGacha*)engineUtilMemoryCalloc(1, sizeof(struct buttonAtelierGacha));
	partsButtonInit((struct partsButton*)this);
	this->gachaKind = gachaKind;
	return this;
}

// 位置設定
void buttonAtelierGachaPosition(struct buttonAtelierGacha *this, int x, int y){
	partsButtonPosition((struct partsButton*)this, x, y, 220, 60);
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct buttonAtelierGacha *this){
	// 状態確認
	int status = 1 | ((this->gachaKind & 0x7fff) << 1);
	struct dataGacha *dataGacha = dataGachaGet(this->gachaKind, NULL);
	if(dataGacha == NULL){
		// ロード中
		status = -1;
	}
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// 使用可能判定
		if(dataGacha != NULL){
			bool isTrial = (dataGacha->product[0].frequency <= 0);
			bool isPoor = (storageStatusGet()->money < dataGacha->money);
			this->super.inactive = (isTrial || isPoor);
		}else{
			this->super.inactive = true;
		}

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ボックス
		e3dImageBoxCreateArray(this->box + 0, 0, 0, this->super.w, this->super.h, TEXPOS_SYSTEM_BUTTONBASICNORMAL_X, TEXPOS_SYSTEM_BUTTONBASICNORMAL_Y, TEXPOS_SYSTEM_BUTTONBASICNORMAL_W, TEXPOS_SYSTEM_BUTTONBASICNORMAL_H);
		e3dImageBoxCreateArray(this->box + 1, 0, 0, this->super.w, this->super.h, TEXPOS_SYSTEM_BUTTONBASICACTIVE_X, TEXPOS_SYSTEM_BUTTONBASICACTIVE_Y, TEXPOS_SYSTEM_BUTTONBASICACTIVE_W, TEXPOS_SYSTEM_BUTTONBASICACTIVE_H);
		e3dImageBoxCreateArray(this->box + 2, 0, 0, this->super.w, this->super.h, TEXPOS_SYSTEM_BUTTONBASICSELECT_X, TEXPOS_SYSTEM_BUTTONBASICSELECT_Y, TEXPOS_SYSTEM_BUTTONBASICSELECT_W, TEXPOS_SYSTEM_BUTTONBASICSELECT_H);
		e3dImageBoxCreateArray(this->box + 3, 0, 0, this->super.w, this->super.h, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_X, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_Y, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_W, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_H);

		if(this->bufferStatus > 0){
			// データラベル
			char data[32];
			sprintf(data, "1かい%dゲソ", dataGacha->money);
			e3dImageTextCreateArray(&this->labelName, 10, 10, dataGacha->name, 1.0, 1, 1);
			e3dImageTextCreateArray(&this->labelMoney, 160, 60 - 15 - 10, data, 1.0, -1, 1);
			// イメージ
			e3dImageCreateArray(&this->image, TEXSIZ_GACHA_W, TEXSIZ_GACHA_H, 165, 5, 50, 50, TEXPOS_GACHA_PROMOTION_X, TEXPOS_GACHA_PROMOTION_Y, TEXPOS_GACHA_PROMOTION_W, TEXPOS_GACHA_PROMOTION_H);
		}

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// データ描画
void buttonAtelierGachaDraw(struct buttonAtelierGacha *this, struct engineMathMatrix44 *mat, double alpha){
	// 画面外の場合は描画しない
	int px = this->super.x + this->super.movex;
	int py = this->super.y + this->super.movey;
	if(px + this->super.w < 0 || global.screen.w < px || py + this->super.h < 0 || global.screen.h < py){return;}

	// 描画準備
	createBuffer(this);

	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);

	struct engineMathMatrix44 tempmat;
	engineMathMat4Copy(&tempmat, mat);
	engineMathMat4Translate(&tempmat, px, py, 0);
	engineGraphicEngineSetMatrix(&tempmat);
	// ボタン枠描画
	int isInactive = (this->super.inactive && !this->super.active && !this->select);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	e3dImageDraw(&this->box[isInactive ? 3 : this->super.active ? 1 : this->select ? 2 : 0]);

	if(this->bufferStatus > 0){
		// ボタン押下による位置ずれ
		engineMathMat4Translate(&tempmat, 0, this->super.active ? 1 : -1, 0);
		engineGraphicEngineSetMatrix(&tempmat);
		// ラベル色
		double labelColor = isInactive ? 0.5 : 0.0;
		engineGraphicEngineSetColorRgba(labelColor, labelColor, labelColor, alpha);
		e3dImageDraw(&this->labelName);
		e3dImageDraw(&this->labelMoney);

		// ガチャイメージ描画
		int e3dIdGachaImage;
		struct dataGacha *dataGacha = dataGachaGet(this->gachaKind, &e3dIdGachaImage);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha * (isInactive ? 0.5 : 1.0));
		engineGraphicEngineBindTexture(e3dIdGachaImage);
		e3dImageDraw(&this->image);
	}
}

// ----------------------------------------------------------------

// 破棄
void buttonAtelierGachaDispose(struct buttonAtelierGacha *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

