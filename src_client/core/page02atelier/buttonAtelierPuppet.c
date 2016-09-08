#include "../library.h"
#include "../includeAll.h"
#include "pageAtelier.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 初期化
struct buttonAtelierPuppet *buttonAtelierPuppetInit(int puppetKind){
	struct buttonAtelierPuppet *this = (struct buttonAtelierPuppet*)engineUtilMemoryCalloc(1, sizeof(struct buttonAtelierPuppet));
	partsButtonInit((struct partsButton*)this);
	this->puppetKind = puppetKind;
	return this;
}

// 位置設定
void buttonAtelierPuppetPosition(struct buttonAtelierPuppet *this, int x, int y){
	partsButtonPosition((struct partsButton*)this, x, y, 120, 65);
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct buttonAtelierPuppet *this){
	struct dataPuppet *dataPuppet = NULL;
	// 状態確認
	int status = 1 | ((this->puppetKind & 0x7fff) << 1);
	if(storagePuppetGet()->list[this->puppetKind].level > 0){
		dataPuppet = dataPuppetGet(this->puppetKind, NULL);
		if(dataPuppet == NULL){
			// ロード中
			status = -1;
		}
	}else{
		// 所持せず
		status = -2;
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
			// 数字列
			e3dNumberDefaultCreateArray(&this->numberNum, 90, 40, 1.0, -1, 1);
			e3dNumberDefaultCreateArray(&this->numberLevel, 110, 25, 1.0, -1, 1);
			// データラベル
			char data[32];
			sprintf(data, "%s\nLv.  \nx  たい", dataPuppet->name);
			e3dImageTextCreateArray(&this->labelData, 60, 10, data, 1.0, 1, 1);
		}else if(this->bufferStatus == -2){
			// エラーラベル 所持せず
			e3dImageTextCreateArray(&this->labelData, this->super.w * 0.5, this->super.h * 0.5, "???", 1.0, 0, 0);
		}

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// データ描画
void buttonAtelierPuppetDrawData(struct buttonAtelierPuppet *this, struct engineMathMatrix44 *mat, double alpha){
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

	if(this->bufferStatus > 0 || this->bufferStatus == -2){
		// ボタン押下による位置ずれ
		engineMathMat4Translate(&tempmat, 0, this->super.active ? 1 : -1, 0);
		engineGraphicEngineSetMatrix(&tempmat);
		// ラベル描画
		double labelColor = isInactive ? 0.5 : 0.0;
		engineGraphicEngineSetColorRgba(labelColor, labelColor, labelColor, alpha);
		e3dImageDraw(&this->labelData);
		if(this->bufferStatus > 0){
			// 数値列描画
			engineGraphicEngineSetColorRgba(labelColor, labelColor, isInactive ? 0.5 : 1.0, alpha);
			e3dNumberDraw(&this->numberNum, &tempmat, storagePuppetGet()->list[this->puppetKind].num);
			e3dNumberDraw(&this->numberLevel, &tempmat, storagePuppetGet()->list[this->puppetKind].level);
		}
	}
}

// キャラクタ描画
void buttonAtelierPuppetDrawChara(struct buttonAtelierPuppet *this, struct engineMathMatrix44 *mat){
	// 画面外の場合は描画しない
	int px = this->super.x + this->super.movex;
	int py = this->super.y + this->super.movey;
	if(px + this->super.w < 0 || global.screen.w < px || py + this->super.h < 0 || global.screen.h < py){return;}

	if(this->bufferStatus > 0){
		// 状態確認
		int e3dIdPuppetImage;
		struct dataPuppet *dataPuppet = dataPuppetGet(this->puppetKind, &e3dIdPuppetImage);
		if(dataPuppet == NULL){return;}

		// 行列作成
		double rotv = 3.1415 / 180 *  0;
		double roth = 3.1415 / 180 * 30;
		struct engineMathMatrix44 tempmat;
		engineMathMat4Copy(&tempmat, mat);
		engineMathMat4Translate(&tempmat, px + 30, py + 55 + (this->super.active ? 1 : -1), 0);
		engineMathMat4Scale(&tempmat, 1.0, -1.0, 0.01);
		engineMathMat4RotateX(&tempmat, roth);
		engineMathMat4RotateY(&tempmat, rotv);
		// ハコニワキャラクタ描画
		engineGraphicEngineBindTexture(e3dIdPuppetImage);
		struct hknwChara *chara = hknwCharaListGet(dataPuppet->charaKind);
		hknwCharaDrawChara(chara, &tempmat, rotv, roth, 0, 0, 0, 3.1415 / 180 * 90, 50.0, HKNWCHARAPOSESID_STAND, 0);
	}
}

// ----------------------------------------------------------------

// 破棄
void buttonAtelierPuppetDispose(struct buttonAtelierPuppet *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

