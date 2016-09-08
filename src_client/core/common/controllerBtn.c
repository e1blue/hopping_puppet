#include "../library.h"
#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// コントローラボタン入力
bool ctrlBtnKup = false;
bool ctrlBtnKdn = false;
bool ctrlBtnKrt = false;
bool ctrlBtnKlt = false;

// 配列にコントローラボタン十字キーの描画情報を追加
void ctrlBtnArrowCreateArray(int *faceIndex){
	int vertIndex = engineGraphicBufferVretIndexGet();
	if(faceIndex != NULL){*faceIndex = engineGraphicBufferFaceIndexGet();}
	int tetraNum = 8;

	for(int i = 0; i < 2; i++){
		// 頂点座標データを生成
		engineGraphicBufferPushTetraVert(  0 - TEXPOS_SYSTEM_BUTTONCTRLUPNORMAL_W * 0.25, -35 - TEXPOS_SYSTEM_BUTTONCTRLUPNORMAL_H * 0.25, TEXPOS_SYSTEM_BUTTONCTRLUPNORMAL_W * 0.5, TEXPOS_SYSTEM_BUTTONCTRLUPNORMAL_H * 0.5);
		engineGraphicBufferPushTetraVert(  0 - TEXPOS_SYSTEM_BUTTONCTRLDNNORMAL_W * 0.25,  35 - TEXPOS_SYSTEM_BUTTONCTRLDNNORMAL_H * 0.25, TEXPOS_SYSTEM_BUTTONCTRLDNNORMAL_W * 0.5, TEXPOS_SYSTEM_BUTTONCTRLDNNORMAL_H * 0.5);
		engineGraphicBufferPushTetraVert( 35 - TEXPOS_SYSTEM_BUTTONCTRLRTNORMAL_W * 0.25,   0 - TEXPOS_SYSTEM_BUTTONCTRLRTNORMAL_H * 0.25, TEXPOS_SYSTEM_BUTTONCTRLRTNORMAL_W * 0.5, TEXPOS_SYSTEM_BUTTONCTRLRTNORMAL_H * 0.5);
		engineGraphicBufferPushTetraVert(-35 - TEXPOS_SYSTEM_BUTTONCTRLLTNORMAL_W * 0.25,   0 - TEXPOS_SYSTEM_BUTTONCTRLLTNORMAL_H * 0.25, TEXPOS_SYSTEM_BUTTONCTRLLTNORMAL_W * 0.5, TEXPOS_SYSTEM_BUTTONCTRLLTNORMAL_H * 0.5);
	}
	// テクスチャ座標データを生成
	engineGraphicBufferPushTetraTexc(TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, TEXPOS_SYSTEM_BUTTONCTRLUPNORMAL_X, TEXPOS_SYSTEM_BUTTONCTRLUPNORMAL_Y, TEXPOS_SYSTEM_BUTTONCTRLUPNORMAL_W, TEXPOS_SYSTEM_BUTTONCTRLUPNORMAL_H);
	engineGraphicBufferPushTetraTexc(TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, TEXPOS_SYSTEM_BUTTONCTRLDNNORMAL_X, TEXPOS_SYSTEM_BUTTONCTRLDNNORMAL_Y, TEXPOS_SYSTEM_BUTTONCTRLDNNORMAL_W, TEXPOS_SYSTEM_BUTTONCTRLDNNORMAL_H);
	engineGraphicBufferPushTetraTexc(TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, TEXPOS_SYSTEM_BUTTONCTRLRTNORMAL_X, TEXPOS_SYSTEM_BUTTONCTRLRTNORMAL_Y, TEXPOS_SYSTEM_BUTTONCTRLRTNORMAL_W, TEXPOS_SYSTEM_BUTTONCTRLRTNORMAL_H);
	engineGraphicBufferPushTetraTexc(TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, TEXPOS_SYSTEM_BUTTONCTRLLTNORMAL_X, TEXPOS_SYSTEM_BUTTONCTRLLTNORMAL_Y, TEXPOS_SYSTEM_BUTTONCTRLLTNORMAL_W, TEXPOS_SYSTEM_BUTTONCTRLLTNORMAL_H);
	engineGraphicBufferPushTetraTexc(TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, TEXPOS_SYSTEM_BUTTONCTRLUPACTIVE_X, TEXPOS_SYSTEM_BUTTONCTRLUPACTIVE_Y, TEXPOS_SYSTEM_BUTTONCTRLUPACTIVE_W, TEXPOS_SYSTEM_BUTTONCTRLUPACTIVE_H);
	engineGraphicBufferPushTetraTexc(TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, TEXPOS_SYSTEM_BUTTONCTRLDNACTIVE_X, TEXPOS_SYSTEM_BUTTONCTRLDNACTIVE_Y, TEXPOS_SYSTEM_BUTTONCTRLDNACTIVE_W, TEXPOS_SYSTEM_BUTTONCTRLDNACTIVE_H);
	engineGraphicBufferPushTetraTexc(TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, TEXPOS_SYSTEM_BUTTONCTRLRTACTIVE_X, TEXPOS_SYSTEM_BUTTONCTRLRTACTIVE_Y, TEXPOS_SYSTEM_BUTTONCTRLRTACTIVE_W, TEXPOS_SYSTEM_BUTTONCTRLRTACTIVE_H);
	engineGraphicBufferPushTetraTexc(TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, TEXPOS_SYSTEM_BUTTONCTRLLTACTIVE_X, TEXPOS_SYSTEM_BUTTONCTRLLTACTIVE_Y, TEXPOS_SYSTEM_BUTTONCTRLLTACTIVE_W, TEXPOS_SYSTEM_BUTTONCTRLLTACTIVE_H);


	// インデックスデータを作成
	for(int i = 0; i < tetraNum; i++){engineGraphicBufferPushTetraFace(vertIndex + i * 4);}
}

// コントローラボタン計算
void ctrlBtnCalc(){
	ctrlBtnKup = global.key.up.isActive;
	ctrlBtnKdn = global.key.dn.isActive;
	ctrlBtnKrt = global.key.rt.isActive;
	ctrlBtnKlt = global.key.lt.isActive;

	// タッチ状態の確認
	struct engineCtrlTouch *t = engineCtrlTouchGet(ENGINECTRLTOUCHID_CONTROLLER);
	if(t != NULL){
		if(t->dn){
			// 十字キーあたり判定
			double tx = t->window.x - 72;
			double ty = 72 - (global.window.h - t->window.y);
			if(tx * tx + ty * ty < 72 * 72){
				if(ty < 0 && tx < ty * ty * 0.18 && tx > ty * ty * -0.18){ctrlBtnKup = true;}
				if(ty > 0 && tx < ty * ty * 0.18 && tx > ty * ty * -0.18){ctrlBtnKdn = true;}
				if(tx > 0 && ty < tx * tx * 0.18 && ty > tx * tx * -0.18){ctrlBtnKrt = true;}
				if(tx < 0 && ty < tx * tx * 0.18 && ty > tx * tx * -0.18){ctrlBtnKlt = true;}
				if(!t->active){
					// タッチ開始処理
					engineCtrlTouchOwn();
				}
			}
		}else{
			// タッチ終了時
			engineCtrlTouchFree();
		}
	}
}

// コントローラボタン描画
void ctrlBtnDraw(int faceIndex){
	struct engineMathMatrix44 tempmat;
	engineMathMat4Ortho(&tempmat, 0, global.window.w, global.window.h, 0, -1, 1);
	engineMathMat4Translate(&tempmat, 72, global.window.h - 72, 0);
	engineGraphicEngineSetMatrix(&tempmat);

	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
	engineGraphicEngineDrawIndex((faceIndex + (ctrlBtnKup ?  8 : 0)) * 3, 2 * 3);
	engineGraphicEngineDrawIndex((faceIndex + (ctrlBtnKdn ? 10 : 2)) * 3, 2 * 3);
	engineGraphicEngineDrawIndex((faceIndex + (ctrlBtnKrt ? 12 : 4)) * 3, 2 * 3);
	engineGraphicEngineDrawIndex((faceIndex + (ctrlBtnKlt ? 14 : 6)) * 3, 2 * 3);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

