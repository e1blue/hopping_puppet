#include "../includeAll.h"
#include "pageAtelier.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 初期化
struct buttonAtelierMeister *buttonAtelierMeisterInit(int meisterKind){
	struct buttonAtelierMeister *this = (struct buttonAtelierMeister*)engineUtilMemoryCalloc(1, sizeof(struct buttonAtelierMeister));
	partsButtonInit((struct partsButton*)this);
	this->meisterKind = meisterKind;
	if(meisterKind >= 0){this->meisterLevel = storageMeisterGet()->list[meisterKind].level;}
	return this;
}

// 位置設定
void buttonAtelierMeisterPosition(struct buttonAtelierMeister *this, int x, int y){
	partsButtonPosition((struct partsButton*)this, x, y, 70, 100);
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct buttonAtelierMeister *this){
	struct dataMeister *dataMeister = NULL;
	// 状態確認
	int status = 1 | ((this->meisterKind & 0x7fff) << 1) | ((this->meisterLevel & 0xff) << 16);
	if(this->meisterKind < 0){
		// 選択無し
		status = -2;
	}else if(this->meisterLevel > 0){
		dataMeister = dataMeisterGet(this->meisterKind, NULL);
		if(dataMeister == NULL){
			// ロード中
			status = -1;
		}
	}else{
		// 所持せず
		status = -3;
	}
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ボックス
		e3dImageBoxCreateArray(this->box + 0, 0, 0, this->super.w, this->super.h, TEXPOS_SYSTEM_BUTTONBASICNORMAL_X, TEXPOS_SYSTEM_BUTTONBASICNORMAL_Y, TEXPOS_SYSTEM_BUTTONBASICNORMAL_W, TEXPOS_SYSTEM_BUTTONBASICNORMAL_H);
		e3dImageBoxCreateArray(this->box + 1, 0, 0, this->super.w, this->super.h, TEXPOS_SYSTEM_BUTTONBASICACTIVE_X, TEXPOS_SYSTEM_BUTTONBASICACTIVE_Y, TEXPOS_SYSTEM_BUTTONBASICACTIVE_W, TEXPOS_SYSTEM_BUTTONBASICACTIVE_H);
		e3dImageBoxCreateArray(this->box + 2, 0, 0, this->super.w, this->super.h, TEXPOS_SYSTEM_BUTTONBASICSELECT_X, TEXPOS_SYSTEM_BUTTONBASICSELECT_Y, TEXPOS_SYSTEM_BUTTONBASICSELECT_W, TEXPOS_SYSTEM_BUTTONBASICSELECT_H);
		e3dImageBoxCreateArray(this->box + 3, 0, 0, this->super.w, this->super.h, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_X, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_Y, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_W, TEXPOS_SYSTEM_BUTTONBASICINACTIVE_H);

		if(this->bufferStatus > 0){
			char level[16];
			sprintf(level, "Lv.%d", this->meisterLevel);
			// ラベル
			e3dImageTextCreateArray(&this->labelName, this->super.w * 0.5, 8 + 20 * 0.5, dataMeister->name, 1.0, 0, 0);
			e3dImageTextCreateArray(&this->labelLevel, this->super.w * 0.5, this->super.h - 8 - 5 - 20 * 0.5, level, 1.0, 0, 0);
			// イメージ
			e3dImageCreateArray(&this->image, TEXSIZ_MEISTER_W, TEXSIZ_MEISTER_H, 3, 4, 64, 92, TEXPOS_MEISTER_WORKSPACE_X, TEXPOS_MEISTER_WORKSPACE_Y, TEXPOS_MEISTER_WORKSPACE_W, TEXPOS_MEISTER_WORKSPACE_H);
			// ゲージ
			e3dImageWhiteCreateArray(&this->gauge, 8, this->super.h - 8 - 5, 54, 5);
		}else if(this->bufferStatus == -2){
			// エラーラベル 選択無し
			e3dImageTextCreateArray(&this->labelName, this->super.w * 0.5, this->super.h * 0.5, "あいてる", 1.0, 0, 0);
		}else if(this->bufferStatus == -3){
			// エラーラベル 所持せず
			e3dImageTextCreateArray(&this->labelName, this->super.w * 0.5, this->super.h * 0.5, "???", 1.0, 0, 0);
		}

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
void buttonAtelierMeisterDraw(struct buttonAtelierMeister *this, struct engineMathMatrix44 *mat, int charge, double alpha){
	// 画面外の場合は描画しない
	int px = this->super.x + this->super.movex;
	int py = this->super.y + this->super.movey;
	if(px + this->super.w < 0 || global.screen.w < px || py + this->super.h < 0 || global.screen.h < py){return;}

	// 描画準備	
	createBuffer(this);

	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);
	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);

	struct engineMathMatrix44 tempmat;
	engineMathMat4Copy(&tempmat, mat);
	engineMathMat4Translate(&tempmat, px, py, 0);
	engineGraphicEngineSetMatrix(&tempmat);
	// ボタン枠描画
	int isInactive = (this->super.inactive && !this->super.active && !this->select);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	e3dImageDraw(&this->box[isInactive ? 3 : this->super.active ? 1 : this->select ? 2 : 0]);

	// ボタン押下による位置ずれ
	engineMathMat4Translate(&tempmat, 0, this->super.active ? 1 : -1, 0);
	engineGraphicEngineSetMatrix(&tempmat);

	if(this->bufferStatus > 0){
		int e3dIdMeisterImage;
		struct dataMeister *dataMeister = dataMeisterGet(this->meisterKind, &e3dIdMeisterImage);
		// マイスター描画
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha * 0.4);
		engineGraphicEngineBindTexture(e3dIdMeisterImage);
		e3dImageDraw(&this->image);

		// ラベル色
		double labelColor = isInactive ? 0.5 : 0.0;
		engineGraphicEngineSetColorRgba(labelColor, labelColor, labelColor, alpha);
		engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
		// 名前ラベル描画
		e3dImageDraw(&this->labelName);
		if(charge >= 0){
			double gauge = (double)charge / dataMeister->charge;
			if(gauge > 1.0){gauge = 1.0;}
			// レベルラベル描画
			e3dImageDraw(&this->labelLevel);
			// ゲージ裏描画
			engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
			e3dImageDraw(&this->gauge);
			// ゲージ表描画
			engineMathMat4Translate(&tempmat, 8, 0, 0);
			engineMathMat4Scale(&tempmat, gauge, 1, 1);
			engineMathMat4Translate(&tempmat, -8, 0, 0);
			engineGraphicEngineSetMatrix(&tempmat);
			double gaugeColor = this->super.active ? 0.8 : 1.0;
			engineGraphicEngineSetColorRgba(gaugeColor, gaugeColor, gaugeColor, alpha);
			e3dImageDraw(&this->gauge);
		}else{
			// ゲージを描画しないときはゲージの分レベルラベルの位置をずらす
			engineMathMat4Translate(&tempmat, 0, 5, 0);
			engineGraphicEngineSetMatrix(&tempmat);
			e3dImageDraw(&this->labelLevel);
		}
	}else if(this->bufferStatus == -2 || this->bufferStatus == -3){
		// エラーラベル描画
		double labelColor = isInactive ? 0.5 : 0.0;
		engineGraphicEngineSetColorRgba(labelColor, labelColor, labelColor, alpha);
		e3dImageDraw(&this->labelName);
	}
}

// ----------------------------------------------------------------

// 破棄
void buttonAtelierMeisterDispose(struct buttonAtelierMeister *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

