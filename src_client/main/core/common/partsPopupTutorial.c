#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

enum partsPopupTutorialType{
	PARTSPOPUPTUTORIALTYPE_TOP01A,
	PARTSPOPUPTUTORIALTYPE_TOP01B,
	PARTSPOPUPTUTORIALTYPE_TOP02,
	PARTSPOPUPTUTORIALTYPE_ATELIER01,
	PARTSPOPUPTUTORIALTYPE_ATELIER02,
	PARTSPOPUPTUTORIALTYPE_ATELIER03,
	PARTSPOPUPTUTORIALTYPE_ATELIER04,
	PARTSPOPUPTUTORIALTYPE_ATELIER05A,
	PARTSPOPUPTUTORIALTYPE_ATELIER05B,
	PARTSPOPUPTUTORIALTYPE_FIELD01,
	PARTSPOPUPTUTORIALTYPE_FIELD02A,
	PARTSPOPUPTUTORIALTYPE_FIELD02B,
	PARTSPOPUPTUTORIALTYPE_JUMP01,
	PARTSPOPUPTUTORIALTYPE_JUMP02,
	PARTSPOPUPTUTORIALTYPE_JUMP03
};

// ポップアップカートリッジ構造体
struct partsPopupCartridgeTutorial{
	struct popupCartridge super;

	struct partsButtonBox btnClose;

	struct e3dImage back;
	struct e3dImage box;
	struct e3dImage image;
	struct e3dImage title;
	struct e3dImage label;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	enum partsPopupTutorialType type;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct partsPopupCartridgeTutorial *this){
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct partsPopupCartridgeTutorial *this){
}

// 主計算
static int popupCalcMain(struct partsPopupCartridgeTutorial *this){
	// タッチ処理
	partsButtonBoxCalc(&this->btnClose);

	// ボタン押下確認
	if(this->btnClose.super.trigger){
		this->btnClose.super.trigger = false;
		soundSePlayNG();
		// ポップアップ閉じる
		return 1;
	}

	// 戻るボタン押下確認
	if(global.key.bk.triggerInactive){
		global.key.bk.triggerInactive = false;
		// 何も起きない
	}

	return 0; 
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct partsPopupCartridgeTutorial *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		int lineNum = 1;
		switch(this->type){
			case PARTSPOPUPTUTORIALTYPE_TOP01A: lineNum = 4; break;
			case PARTSPOPUPTUTORIALTYPE_TOP01B: lineNum = 3; break;
			case PARTSPOPUPTUTORIALTYPE_TOP02: lineNum = 2; break;
			case PARTSPOPUPTUTORIALTYPE_ATELIER01: lineNum = 4; break;
			case PARTSPOPUPTUTORIALTYPE_ATELIER02: lineNum = 2; break;
			case PARTSPOPUPTUTORIALTYPE_ATELIER03: lineNum = 3; break;
			case PARTSPOPUPTUTORIALTYPE_ATELIER04: lineNum = 3; break;
			case PARTSPOPUPTUTORIALTYPE_ATELIER05A: lineNum = 3; break;
			case PARTSPOPUPTUTORIALTYPE_ATELIER05B: lineNum = 4; break;
			case PARTSPOPUPTUTORIALTYPE_FIELD01: lineNum = 2; break;
			case PARTSPOPUPTUTORIALTYPE_FIELD02A: lineNum = 2; break;
			case PARTSPOPUPTUTORIALTYPE_FIELD02B: lineNum = 5; break;
			case PARTSPOPUPTUTORIALTYPE_JUMP01: lineNum = 4; break;
			case PARTSPOPUPTUTORIALTYPE_JUMP02: lineNum = 3; break;
			case PARTSPOPUPTUTORIALTYPE_JUMP03: lineNum = 4; break;
		}
		int ww = 186;
		int wh = 68 + 15 * lineNum;
		int wx = (global.screen.w - ww) * 0.5;
		int wy = (global.screen.h - wh) * 0.5;

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ウインドウとタイトル作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->box, wx, wy, ww, wh);
		e3dImageCreateArray(&this->image, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, wx + ww - 3 - 60, wy + wh - 3 - 80, 60, 80, TEXPOS_SYSTEM_IMAGETUTORIALNAVI_X, TEXPOS_SYSTEM_IMAGETUTORIALNAVI_Y, TEXPOS_SYSTEM_IMAGETUTORIALNAVI_W, TEXPOS_SYSTEM_IMAGETUTORIALNAVI_H);
		e3dImageTextCreateArray(&this->title, wx + ww * 0.5, wy + 3 + 5, "チュートリアル", 1.0, 0, 1);
		switch(this->type){
			case PARTSPOPUPTUTORIALTYPE_TOP01A: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "ようこそ!ホッピンパペットっス!\nこのゲームは とにかく\nたくさん パペットを\nあつめる ゲームっス!", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_TOP01B: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "サイショの パペットを つくるため\nまずは アトリエへ\nむかうっス! ", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_TOP02: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "パペットが たくさん あるので\nとうばつに むかうっス!", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_ATELIER01: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "ここが アトリエっス!\nがめんウエの マイスターボタンを\nおして マイスターを\nセットするっス!", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_ATELIER02: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "セットしたら しばらく まてば\nパペットが でてくるっス!", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_ATELIER03: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "でてきた パペットを なぞれば\nパペットを てにいれることが\nできるっス!", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_ATELIER04: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "パペット ゲットっス!\nこのちょうしで 5たいくらい\nあつめるっス!", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_ATELIER05A: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "じゅうぶん パペットが\nあつまったっス!\nいったん もどるっス!", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_ATELIER05B: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "パペット ゲットっス!\nじゅうぶん パペットが\nあつまったっス!\nいったん もどるっス!", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_FIELD01: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "モンスターが あらわれたっス!\nタップして しらべるっス!", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_FIELD02A: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "モンスターを たおしたので\nあらたな みちが ひらけたっス!", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_FIELD02B: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "チュートリアルは ここまでっス!\nたくさんモンスターをたおして,\nたくさんおかねをひろって,\nたくさんマイスターをてにいれ,\nたくさんパペットをあつめるっス!", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_JUMP01: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "モンスターは パペットを たくさん\nぶつければ たおせるっス!\nゆびで はじくように\nスワイプ するっス!", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_JUMP02: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "パペットが なくなったっス!\nここは いったん にげて,\nアトリエで また あつめるっス!", 1.0, 1, 1); break;
			case PARTSPOPUPTUTORIALTYPE_JUMP03: e3dImageTextCreateArray(&this->label, wx + 3 + 5, wy + 3 + 5 + 20, "やったっス!\nモンスターを たおしたっス!\nおかねとか アイテムとか\nもらえるっス!", 1.0, 1, 1); break;
		}

		// 閉じるボタン
		partsButtonBoxCreateArray(&this->btnClose, wx + (ww - 60) * 0.5, wy + wh - 5 - 30, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct partsPopupCartridgeTutorial *this){
	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempmat1;
	double num = (double)this->super.openStep / this->super.openStepMax;
	double alpha = 1 - num * num;

	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);

	// 背景半透明幕描画
	engineMathMat4Identity(&tempmat1);
	engineGraphicEngineSetMatrix(&tempmat1);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha * 0.5);
	e3dImageDraw(&this->back);

	engineMathMat4Copy(&tempmat1, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat1, global.screen.w * 0.5, global.screen.h * 0.5, 0);
	engineMathMat4Scale(&tempmat1, 1 + 0.5 * num * num, 1 + 0.5 * num * num, 1.0);
	engineMathMat4Translate(&tempmat1, -global.screen.w * 0.5, -global.screen.h * 0.5, 0);
	engineGraphicEngineSetMatrix(&tempmat1);
	// ウインドウ描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	e3dImageDraw(&this->box);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha * 0.3);
	e3dImageDraw(&this->image);
	// ラベル描画
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->title);
	e3dImageDraw(&this->label);
	// ボタン描画
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct partsPopupCartridgeTutorial *this){
	return 0;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct partsPopupCartridgeTutorial *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *partsPopupTutorial(enum partsPopupTutorialType type){
	struct partsPopupCartridgeTutorial *this = (struct partsPopupCartridgeTutorial*)engineUtilMemoryCalloc(1, sizeof(struct partsPopupCartridgeTutorial));
	this->type = type;
	init(this);

	struct popupCartridge *cartridge = (struct popupCartridge*)this;
	popupCartridgeInit(cartridge);
	cartridge->popupCalcBefore = (void(*)(struct popupCartridge*))popupCalcBefore;
	cartridge->popupCalcMain = (int(*)(struct popupCartridge*))popupCalcMain;
	cartridge->draw = (void(*)(struct popupCartridge*))draw;
	cartridge->result = (int(*)(struct popupCartridge*))result;
	cartridge->dispose = (void(*)(struct popupCartridge*))dispose;
	return cartridge;
}

struct popupCartridge *partsPopupTutorialTop01A(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_TOP01A);}
struct popupCartridge *partsPopupTutorialTop01B(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_TOP01B);}
struct popupCartridge *partsPopupTutorialTop02(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_TOP02);}
struct popupCartridge *partsPopupTutorialAtelier01(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_ATELIER01);}
struct popupCartridge *partsPopupTutorialAtelier02(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_ATELIER02);}
struct popupCartridge *partsPopupTutorialAtelier03(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_ATELIER03);}
struct popupCartridge *partsPopupTutorialAtelier04(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_ATELIER04);}
struct popupCartridge *partsPopupTutorialAtelier05A(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_ATELIER05A);}
struct popupCartridge *partsPopupTutorialAtelier05B(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_ATELIER05B);}
struct popupCartridge *partsPopupTutorialField01(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_FIELD01);}
struct popupCartridge *partsPopupTutorialField02A(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_FIELD02A);}
struct popupCartridge *partsPopupTutorialField02B(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_FIELD02B);}
struct popupCartridge *partsPopupTutorialJump01(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_JUMP01);}
struct popupCartridge *partsPopupTutorialJump02(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_JUMP02);}
struct popupCartridge *partsPopupTutorialJump03(){return partsPopupTutorial(PARTSPOPUPTUTORIALTYPE_JUMP03);}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

