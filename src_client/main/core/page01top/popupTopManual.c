#include "../includeAll.h"
#include "pageTop.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#define POPUPTOPMANUAL_PAGENUM 16
#define POPUPTOPMANUAL_BTNNUM 5
#define POPUPTOPMANUAL_IMAGENUM 16
#define POPUPTOPMANUAL_LABELNUM 16

// ポップアップカートリッジ構造体
struct popupCartridgeTopManual{
	struct popupCartridge super;
	struct pageTopStatus *stat;

	struct partsButtonBox btnBack;
	struct partsButtonBox btnClose;
	struct partsButton btnOuter;

	struct e3dImage back;
	struct e3dImage box;
	struct e3dImage titleLabel;
	struct e3dImage headerBorder;
	struct e3dImage footerBorder;
	struct e3dImage mask;

	// マニュアルページ情報
	int pageIndex;
	struct{
		double xCurr;
		double xNext;
		int pageIndexBack;
		struct e3dImage image[POPUPTOPMANUAL_IMAGENUM];
		struct e3dImage label[POPUPTOPMANUAL_LABELNUM];
		struct popupCartridgeTopManualPageNext{
			int pageIndexNext;
			struct partsButtonBox btnNext;
		} next[POPUPTOPMANUAL_BTNNUM];
	} page[POPUPTOPMANUAL_PAGENUM];

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;
	int e3dIdManualImage;
};

// ----------------------------------------------------------------

// 初期化
static void init(struct popupCartridgeTopManual *this){
	// 戻るボタン
	partsButtonBoxInit(&this->btnBack);
	this->btnBack.super.inactive = true;
	// 閉じるボタン
	partsButtonBoxInit(&this->btnClose);
	// ウインドウ外ボタン
	partsButtonInit(&this->btnOuter);
	this->btnOuter.inner = 0;
	// マニュアルページ初期化
	for(int i = 0; i < POPUPTOPMANUAL_PAGENUM; i++){
		this->page[i].xCurr = this->page[i].xNext = 320;
		// マニュアルページ遷移ボタン
		for(int j = 0; j < POPUPTOPMANUAL_BTNNUM; j++){partsButtonBoxInit(&this->page[i].next[j].btnNext);}
	}
	this->page[0].xCurr = this->page[0].xNext = 0;

	// テクスチャロード
	this->e3dIdManualImage = engineGraphicTextureCreateLocal("img/manual.png", ENGINEGRAPHICTEXTURETYPE_LINEAR);
}

// ----------------------------------------------------------------

// 前方計算
static void popupCalcBefore(struct popupCartridgeTopManual *this){
}

// 主計算
static int popupCalcMain(struct popupCartridgeTopManual *this){
	// タッチ処理
	for(int i = 0; i < POPUPTOPMANUAL_BTNNUM; i++){
		struct popupCartridgeTopManualPageNext *next = &this->page[this->pageIndex].next[i];
		if(next->pageIndexNext > 0){partsButtonBoxCalc(&next->btnNext);}
	}
	partsButtonBoxCalc(&this->btnBack);
	partsButtonBoxCalc(&this->btnClose);
	partsButtonCalc(&this->btnOuter);

	// マニュアルページ位置更新
	for(int i = 0; i < POPUPTOPMANUAL_PAGENUM; i++){
		this->page[i].xCurr -= 0.1 * (this->page[i].xCurr - this->page[i].xNext);
		for(int j = 0; j < POPUPTOPMANUAL_BTNNUM; j++){
			struct popupCartridgeTopManualPageNext *next = &this->page[i].next[j];
			if(next->pageIndexNext > 0){partsButtonBoxMove(&next->btnNext, this->page[i].xCurr, 0);}
		}
	}

	// ボタン押下確認
	for(int i = 0; i < POPUPTOPMANUAL_BTNNUM; i++){
		struct popupCartridgeTopManualPageNext *next = &this->page[this->pageIndex].next[i];
		if(next->pageIndexNext > 0 && next->btnNext.super.trigger){
			next->btnNext.super.trigger = false;
			soundSePlayOK();
			// マニュアルページ遷移
			int pageIndexPrev = this->pageIndex;
			int pageIndexNext = next->pageIndexNext;
			this->pageIndex = pageIndexNext;
			this->page[pageIndexPrev].xNext = -320;
			this->page[pageIndexNext].xCurr = 320;
			this->page[pageIndexNext].xNext = 0;
			this->page[pageIndexNext].pageIndexBack = pageIndexPrev;
			return 0;
		}
	}

	// ボタン押下確認
	this->btnBack.super.inactive = (this->pageIndex == this->page[this->pageIndex].pageIndexBack);
	if(!this->btnBack.super.inactive && (this->btnBack.super.trigger || global.key.bk.triggerInactive)){
		this->btnBack.super.trigger = false;
		global.key.bk.triggerInactive = false;
		soundSePlayNG();
		// マニュアルページ遷移
		int pageIndexPrev = this->pageIndex;
		int pageIndexNext = this->page[this->pageIndex].pageIndexBack;
		this->pageIndex = pageIndexNext;
		this->page[pageIndexPrev].xNext = 320;
		this->page[pageIndexNext].xCurr = -320;
		this->page[pageIndexNext].xNext = 0;
		return 0;
	}

	// ボタン押下確認
	if(this->btnClose.super.trigger || this->btnOuter.trigger || global.key.bk.triggerInactive){
		this->btnClose.super.trigger = false;
		this->btnOuter.trigger = false;
		global.key.bk.triggerInactive = false;
		soundSePlayNG();
		// ポップアップ閉じる
		return 1;
	}

	return 0; 
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct popupCartridgeTopManual *this){
	int status = 1;
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		int ww = 310;
		int wh = 70 + 320;
		int wx = (global.screen.w - ww) * 0.5;
		int wy = (global.screen.h - wh) * 0.5;

		// ウインドウ外ボタン位置
		partsButtonPosition(&this->btnOuter, wx, wy, ww, wh);

		// バッファ作成開始
		engineGraphicBufferBegin();

		// ウインドウとタイトル作成
		e3dImageWhiteCreateArray(&this->back, -1.0, -1.0, 2.0, 2.0);
		e3dImageBoxDefaultCreateArray(&this->box, wx, wy, ww, wh);
		e3dImageTextCreateArray(&this->titleLabel, wx + ww * 0.5, wy + 30 * 0.5, "マニュアル", 1.0, 0, 0);
		e3dImageWhiteCreateArray(&this->headerBorder, wx + 3, wy + 30 - 2, ww - 6, 2);
		e3dImageWhiteCreateArray(&this->footerBorder, wx + 3, wy + wh - 40, ww - 6, 2);
		e3dImageWhiteCreateArray(&this->mask, wx + 3, wy + 30, ww - 6, wh - 70);

		int pageIndex;
		int imageNum;
		int labelNum;
		int btnNum;
		int labely;
		struct popupCartridgeTopManualPageNext *next;

		// 最初のマニュアルページ
		pageIndex = 0;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "ホッピンパペット について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL00_X, TEXPOS_MANUAL_MANUAL00_Y, TEXPOS_MANUAL_MANUAL00_W, TEXPOS_MANUAL_MANUAL00_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "ホッピンパペットは パペットを あつめる ゲームです.\nモンスターを たおして おかねを ためて\nマイスターを あつめ,\nいろいろな パペットを つくってください.", 1.0, 0, 1);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 1; partsButtonBoxCreateArray(&next->btnNext, wx + 1 * (ww + 80) / 4 - 80, (labely += 65), 80, 30, "パペット", 1.0);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 2; partsButtonBoxCreateArray(&next->btnNext, wx + 2 * (ww + 80) / 4 - 80, (labely +=  0), 80, 30, "マイスター", 1.0);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 3; partsButtonBoxCreateArray(&next->btnNext, wx + 3 * (ww + 80) / 4 - 80, (labely +=  0), 80, 30, "モンスター", 1.0);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 4; partsButtonBoxCreateArray(&next->btnNext, wx + 1 * (ww + 80) / 3 - 80, (labely += 35), 80, 30, "アトリエ", 1.0);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 5; partsButtonBoxCreateArray(&next->btnNext, wx + 2 * (ww + 80) / 3 - 80, (labely +=  0), 80, 30, "とうばつ", 1.0);

		// パペットのマニュアルページ
		pageIndex = 1;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "パペット について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL01_X, TEXPOS_MANUAL_MANUAL01_Y, TEXPOS_MANUAL_MANUAL01_W, TEXPOS_MANUAL_MANUAL01_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "しゅじんこう コッペンが ちからを こめることにより\nあるいたり ばくはつしたりする フシギな にんぎょうです.\nアトリエで マイスターの てによって つくられます.\nスワイプして モンスターに あてることで,\nモンスターに ダメージを あたえます.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 80), "パペットには  ぞくぜいがあり,それにより\nモンスターとの あいしょうが きまります.\nぞくせいは パペットの いろで はんだんできます.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 50), "パペットには レベルがあり,きほんてきには レベルが\nたかいほうが おおく ダメージを あたえます.", 1.0, 0, 1);

		// マイスターのマニュアルページ
		pageIndex = 2;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "マイスター について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL02_X, TEXPOS_MANUAL_MANUAL02_Y, TEXPOS_MANUAL_MANUAL02_W, TEXPOS_MANUAL_MANUAL02_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "パペットを つくる しょくにんさんです.\nアトリエの ガチャで やとうことが できます.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "マイスターには レベルがあり,\nマイスターと おなじ レベルの パペットが つくられます.\nガチャで ダブると マイスターの レベルが あがります.", 1.0, 0, 1);

		// モンスターのマニュアルページ
		pageIndex = 3;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "モンスター について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL03_X, TEXPOS_MANUAL_MANUAL03_Y, TEXPOS_MANUAL_MANUAL03_W, TEXPOS_MANUAL_MANUAL03_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "このセカイに あらわれる ナゾの おおきな いきものです.\nべつに ガイは ありませんが\nおおきくて ウザいので とうばつ されます.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 50), "たおすと ナゼか おかねを おとし,\nしゅういの マップを かいほう します.", 1.0, 0, 1);

		// アトリエのマニュアルページ
		pageIndex = 4;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "アトリエ について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL04_X, TEXPOS_MANUAL_MANUAL04_Y, TEXPOS_MANUAL_MANUAL04_W, TEXPOS_MANUAL_MANUAL04_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + 60, (labely += 105), "(1) マイスターボタン", 1.0, 1, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + 60, (labely += 15), "(2) パペットボタン", 1.0, 1, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 20), "マイスターが パペットを つくる ばしょです.\nつくられた パペットを なぞることで かくとく できます.", 1.0, 0, 1);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 6; partsButtonBoxCreateArray(&next->btnNext, wx + 1 * (ww + 120) / 3 - 120, (labely += 35), 120, 30, "マイスターボタン", 1.0);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 7; partsButtonBoxCreateArray(&next->btnNext, wx + 2 * (ww + 120) / 3 - 120, (labely +=  0), 120, 30, "パペットボタン", 1.0);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 8; partsButtonBoxCreateArray(&next->btnNext, wx + (ww - 60) * 0.5, (labely += 35), 60, 30, "ガチャ", 1.0);

		// とうばつのマニュアルページ
		pageIndex = 5;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "とうばつ について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL05_X, TEXPOS_MANUAL_MANUAL05_Y, TEXPOS_MANUAL_MANUAL05_W, TEXPOS_MANUAL_MANUAL05_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "モンスターが あらわれる マップに いくことが できます.\nじかんけいかで モンスターが あらわれるので\nとうばつ しましょう.\nとうばつに せいこうすれば しゅういの マップが\nかいほうされ いけるばしょが ふえます.", 1.0, 0, 1);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 9; partsButtonBoxCreateArray(&next->btnNext, wx + 1 * (ww + 100) / 3 - 100, (labely += 80), 100, 30, "マップいどう", 1.0);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 10; partsButtonBoxCreateArray(&next->btnNext, wx + 2 * (ww + 100) / 3 - 100, (labely += 0), 100, 30, "アイテムおかね", 1.0);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 11; partsButtonBoxCreateArray(&next->btnNext, wx + (ww - 140) * 0.5, (labely += 35), 140, 30, "モンスターとのたたかい", 1.0);

		// マイスターボタンのマニュアルページ
		pageIndex = 6;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "アトリエ マイスターボタン について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL06_X, TEXPOS_MANUAL_MANUAL06_Y, TEXPOS_MANUAL_MANUAL06_W, TEXPOS_MANUAL_MANUAL06_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "マイスターを センタク すると\nパペットせいさくを かいし します.\nゲージが いっぱいに なると\nパペットが 1たい つくられます.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 65), "ゲージは アプリを とじていても ふえます.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 20), "センタクしている マイスターを もういちど\nタップすることで センタクの かいじょが できます.", 1.0, 0, 1);

		// パペットボタンのマニュアルページ
		pageIndex = 7;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "アトリエ パペットボタン について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL07_X, TEXPOS_MANUAL_MANUAL07_Y, TEXPOS_MANUAL_MANUAL07_W, TEXPOS_MANUAL_MANUAL07_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "これまで あつめた パペットを かくにん できます.\nパペットは 1しゅるい 99たい まで\nもつことが できます.", 1.0, 0, 1);

		// ガチャのマニュアルページ
		pageIndex = 8;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "アトリエ ガチャ について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL08_X, TEXPOS_MANUAL_MANUAL08_Y, TEXPOS_MANUAL_MANUAL08_W, TEXPOS_MANUAL_MANUAL08_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "おかねを つかって マイスターを ランダムで やとえます.\nねだんが たかい ガチャほど\nじつりょくの たかい マイスターを やとえます.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 50), "おかねの つかいみちは ガチャいがい ほとんど ないので\nおかねが てにはいったら どんどん まわしましょう.", 1.0, 0, 1);

		// マップいどうのマニュアルページ
		pageIndex = 9;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "とうばつ マップいどう について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL09_X, TEXPOS_MANUAL_MANUAL09_Y, TEXPOS_MANUAL_MANUAL09_W, TEXPOS_MANUAL_MANUAL09_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "やじるしが かいてあるマスを タップして しらべることで\nマップいどうが できます.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "さきに すすむほど モンスターは つよくなり\nてにはいる おかねも ふえていきます.", 1.0, 0, 1);

		// アイテムおかねのマニュアルページ
		pageIndex = 10;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "とうばつ アイテムおかね について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL10_X, TEXPOS_MANUAL_MANUAL10_Y, TEXPOS_MANUAL_MANUAL10_W, TEXPOS_MANUAL_MANUAL10_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "ふくろの おいてあるマスを タップして しらべることで\nアイテムや おかねが てにはいります.\nときどき かくしアイテムも あります.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 50), "じょばんは モンスターが おとす おかねも すくないので\nたくさん おかねが てにはいるように\nいろいろ しらべて いきましょう.", 1.0, 0, 1);

		// モンスターとのたたかいのマニュアルページ
		pageIndex = 11;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "とうばつ モンスターとのたたかい について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL11_X, TEXPOS_MANUAL_MANUAL11_Y, TEXPOS_MANUAL_MANUAL11_W, TEXPOS_MANUAL_MANUAL11_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "モンスターの いるマスを タップして しらべることで\nたたかうことが できます.", 1.0, 0, 1);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 12; partsButtonBoxCreateArray(&next->btnNext, wx + 1 * (ww + 100) / 3 - 100, (labely += 35), 100, 30, "たたかいかた", 1.0);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 13; partsButtonBoxCreateArray(&next->btnNext, wx + 2 * (ww + 100) / 3 - 100, (labely +=  0), 100, 30, "テンション", 1.0);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 14; partsButtonBoxCreateArray(&next->btnNext, wx + 1 * (ww + 100) / 3 - 100, (labely += 35), 100, 30, "けっか", 1.0);
		next = &this->page[pageIndex].next[btnNum++]; next->pageIndexNext = 15; partsButtonBoxCreateArray(&next->btnNext, wx + 2 * (ww + 100) / 3 - 100, (labely +=  0), 100, 30, "けいけん", 1.0);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "モンスターによって うばわれた じぶんの たいりょくは\nたたかって いない ときに 1びょう ごとに\n1づつ かいふくします.\nたいりょくは アプリを とじていても かいふくします.", 1.0, 0, 1);

		// たたかいかたのマニュアルページ
		pageIndex = 12;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "とうばつ モンスターとのたたかい たたかいかた について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL12_X, TEXPOS_MANUAL_MANUAL12_Y, TEXPOS_MANUAL_MANUAL12_W, TEXPOS_MANUAL_MANUAL12_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "したから しょじしている パペットが あらわれるので\nうえに スワイプして モンスターに ぶつけましょう.\nなげた　きせきの　ちょうてんで　ばくはつします.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 50), "モンスターの こうげきも パペットを\nぶつけて おいはらいましょう.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "つかわない パペットは したに スワイプすれば\nひっこめることが できます.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "モンスターには ジャクテンぶぶんと ジャクテンぞくせいが\nあります. それをたたけば ゆうりに たたかえます.", 1.0, 0, 1);

		// テンションゲージのマニュアルページ
		pageIndex = 13;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "とうばつ モンスターとのたたかい テンション について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL13_X, TEXPOS_MANUAL_MANUAL13_Y, TEXPOS_MANUAL_MANUAL13_W, TEXPOS_MANUAL_MANUAL13_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "パペットを なげるたびに\nあおい テンションゲージが たまります.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "テンションゲージが あがるほど きほんてきに\nパペットは モンスターに おおく ダメージを あたえます.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "テンションゲージは たたかいが おわると\nリセットされます.", 1.0, 0, 1);

		// けっかのマニュアルページ
		pageIndex = 14;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "とうばつ モンスターとのたたかい けっか について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL14_X, TEXPOS_MANUAL_MANUAL14_Y, TEXPOS_MANUAL_MANUAL14_W, TEXPOS_MANUAL_MANUAL14_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "パペットを ぶつけて モンスターの たいりょくを\nからにして とうばつに せいこうしたら,\nおかねと けいけんがてにはいり\nどうじに しゅういの マップを かいほう します.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 65), "ぎゃくに じぶんの たいりょくが なくなったら\nかいふくするまで たたかえなく なります.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "にげても テンションゲージが なくなる だけで\nモンスターの たいりょくが かいふくすることも ないので\nあぶなくなったら すぐ にげましょう.", 1.0, 0, 1);

		// けいけんのマニュアルページ
		pageIndex = 15;
		labely = wy;
		imageNum = labelNum = btnNum = 0;
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "とうばつ モンスターとのたたかい けいけん について", 1.0, 0, 1);
		e3dImageCreateArray(&this->page[pageIndex].image[imageNum++], TEXSIZ_MANUAL_W, TEXSIZ_MANUAL_H, wx + (ww - 100) * 0.5, (labely += 25), 100, 100, TEXPOS_MANUAL_MANUAL15_X, TEXPOS_MANUAL_MANUAL15_Y, TEXPOS_MANUAL_MANUAL15_W, TEXPOS_MANUAL_MANUAL15_H);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 105), "モンスターを たおして えられた けいけんを,\nいっていいじょう ためると レベルが あがります.", 1.0, 0, 1);
		e3dImageTextCreateArray(&this->page[pageIndex].label[labelNum++], wx + ww * 0.5, (labely += 35), "レベルが あがると\nじぶんの たいりょくの じょうげんが あがります.", 1.0, 0, 1);

		// 戻るボタン
		partsButtonBoxCreateArray(&this->btnBack, wx + (ww - 90) * 0.5 - (ww + 90) / 6, wy + wh - 5 - 30, 90, 30, "ひとつもどる", 1.0);
		// 閉じるボタン
		partsButtonBoxCreateArray(&this->btnClose, wx + (ww - 60) * 0.5 + (ww + 60) / 6, wy + wh - 5 - 30, 60, 30, "とじる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// 描画
static void draw(struct popupCartridgeTopManual *this){
	// 描画準備
	createBuffer(this);

	struct engineMathMatrix44 tempmat1;
	struct engineMathMatrix44 tempmat2;
	double num = (double)this->super.openStep / this->super.openStepMax;
	double alpha = 1 - num * num;
	bool isOpening = (this->super.openStep < 0 || this->super.openStepMax <= this->super.openStep);
	bool isClosing = (0 < this->super.openStep && this->super.openStep < this->super.openStepMax);
	double alphaBack = (isOpening) ? 1.0 : alpha;

	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);

	// 背景半透明幕描画
	engineMathMat4Identity(&tempmat1);
	engineGraphicEngineSetMatrix(&tempmat1);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alphaBack * 0.5);
	e3dImageDraw(&this->back);

	engineMathMat4Copy(&tempmat1, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat1, global.screen.w * 0.5, global.screen.h * 0.5, 0);
	engineMathMat4Scale(&tempmat1, 1 + 0.5 * num * num, 1 + 0.5 * num * num, 1.0);
	engineMathMat4Translate(&tempmat1, -global.screen.w * 0.5, -global.screen.h * 0.5, 0);
	engineGraphicEngineSetMatrix(&tempmat1);
	// ウインドウ描画
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
	e3dImageDraw(&this->box);
	// ラベル描画
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
	e3dImageDraw(&this->titleLabel);
	e3dImageDraw(&this->headerBorder);
	e3dImageDraw(&this->footerBorder);
	// ボタン描画
	partsButtonBoxDraw(&this->btnBack, &tempmat1, alpha);
	partsButtonBoxDraw(&this->btnClose, &tempmat1, alpha);

	// マニュアルページマスク設定
	engineGraphicStencilClear();
	engineGraphicStencilSetMode(ENGINEGRAPHICSTENCILMODE_MASK_1);
	engineGraphicEngineSetMatrix(&tempmat1);
	e3dImageDraw(&this->mask);
	engineGraphicStencilSetMode(ENGINEGRAPHICSTENCILMODE_READ_EQ1);

	for(int i = 0; i < POPUPTOPMANUAL_PAGENUM; i++){
		if(engineMathAbs(this->page[i].xCurr) < 310){
			engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
			// マニュアルページボタン描画
			for(int j = 0; j < POPUPTOPMANUAL_BTNNUM; j++){
				struct popupCartridgeTopManualPageNext *next = &this->page[i].next[j];
				if(next->pageIndexNext > 0){partsButtonBoxDraw(&next->btnNext, &tempmat1, alpha);}
			}
			engineMathMat4Copy(&tempmat2, &tempmat1);
			engineMathMat4Translate(&tempmat2, this->page[i].xCurr, 0, 0);
			engineGraphicEngineSetMatrix(&tempmat2);
			// マニュアルページラベル描画
			engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, alpha);
			for(int j = 0; j < POPUPTOPMANUAL_LABELNUM; j++){e3dImageDraw(&this->page[i].label[j]);}
			// マニュアルページイメージ描画
			engineGraphicEngineBindTexture(this->e3dIdManualImage);
			engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, alpha);
			for(int j = 0; j < POPUPTOPMANUAL_IMAGENUM; j++){e3dImageDraw(&this->page[i].image[j]);}
		}
	}

	engineGraphicStencilSetMode(ENGINEGRAPHICSTENCILMODE_NONE);
}

// ----------------------------------------------------------------

// 選択結果
static int result(struct popupCartridgeTopManual *this){
	return 0;
}

// ----------------------------------------------------------------

// 破棄
static void dispose(struct popupCartridgeTopManual *this){
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineGraphicTextureDispose(this->e3dIdManualImage);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// カートリッジ作成
struct popupCartridge *popupTopManual(struct pageTopStatus *stat){
	struct popupCartridgeTopManual *this = (struct popupCartridgeTopManual*)engineUtilMemoryCalloc(1, sizeof(struct popupCartridgeTopManual));
	this->stat = stat;
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

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

