#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../core/data.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

char *dir = "puppet";
typedef struct dataPuppet Data;

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

Data data;
int kind;
bool isTrial;

// データ初期化
void initData(int id, bool flag){kind = id; isTrial = flag; memset(&data, 0, sizeof(Data));}

// データ書き出し
void createData(){
	char path[256];
	sprintf(path, "contents/data/%s/%02d.dat", dir, kind);

#ifdef __TRIAL__
	if(!isTrial){return;}
#endif

	char *buff = base64encode((void*)&data, sizeof(Data));

	FILE *fp;
	fp = fopen(path, "w");
	fputs(buff, fp);
	fclose(fp);
}

#define SETSTR(SRC, DST) do{if(strlen(SRC) + 1 > sizeof(DST)){fprintf(stderr, "%4d: %s is too long!!\n", kind, #SRC); exit(1);} strcpy(DST, SRC);}while(0)

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 各パラメータ設定

void setCode(char *code){SETSTR(code, data.code);}
void setName(char *name){SETSTR(name, data.name);}
void setFullname(char *fullname){SETSTR(fullname, data.fullname);}
void setProfile(char *profile){SETSTR(profile, data.profile);}
void setCharaKind(enum hknwCharaKind charaKind){data.charaKind = charaKind;}
void setAttack(unsigned short attackMain, unsigned short attackSub, enum dataAttributionKind attributionKind){data.attackMain = attackMain; data.attackSub = attackSub; data.attributionKind = attributionKind;}
void setRatio(unsigned char tensionRatio10, unsigned char levelRatio10){data.tensionRatio10 = tensionRatio10; data.levelRatio10 = levelRatio10;}
void setWeight(unsigned char weight){data.weight = weight;}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// main関数 処理はここから始まる
int main(int argc, char *argv[]){

	initData(0, true); setCode("00kuma");
	setName("クマ"); setFullname("ふわふわクマ");
	setProfile(
		"コッペンが つくった ぬいぐる\n"
		"み パペット.みどり ぞくせい\n"
		"ちちおやに つくって もらった\n"
		"たいせつな ぬいぐるみを さん\n"
		"こうに みようみまねで つくり\n"
		"あげた いっぴん.");
	setCharaKind(HKNWCHARAKIND_KUMA);
	setAttack(20, 30, DATAATTRIBUTIONKIND_GREEN);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(1, true); setCode("01papi");
	setName("パピヨン"); setFullname("レッドパピヨン");
	setProfile(
		"ランク1の マイスターが つく\n"
		"った ヒトガタ パペット.あか\n"
		"ぞくせい.あかの ちからは じ\n"
		"ょうねつの ちから.あつき こ\n"
		"ころが あいてを こがす.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(90, 60, DATAATTRIBUTIONKIND_RED);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(2, true); setCode("02pasu");
	setName("パステル"); setFullname("パステルナーク");
	setProfile(
		"ランク1の マイスターが つく\n"
		"った ヒトガタ パペット.みど\n"
		"り ぞくせい.みどりの ちから\n"
		"は まごころの ちから.まっす\n"
		"ぐな しせいが あいてを つら\n"
		"ぬく.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(90, 60, DATAATTRIBUTIONKIND_GREEN);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(3, true); setCode("03poni");
	setName("ぽにこ"); setFullname("ポニーテールのぽにこ");
	setProfile(
		"ランク1の マイスターが つく\n"
		"った ヒトガタ パペット.あお\n"
		"ぞくせい.あおの ちからは れ\n"
		"いせいの ちから.どうじない\n"
		"たいどが あいてに きょうふを\n"
		"あたえる.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(90, 60, DATAATTRIBUTIONKIND_BLUE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(4, true); setCode("04botu");
	setName("ボツ"); setFullname("しっぱいさく");
	setProfile(
		"だれもが つくるかもしれない\n"
		"シッパイ パペット.しろ ぞく\n"
		"せい.しっぱいさく だけど モ\n"
		"ンスターに すこしは ダメージ\n"
		"を あたえることが できるので\n"
		"いちおう あつめておく.エコ.");
	setCharaKind(HKNWCHARAKIND_KUMA);
	setAttack(15, 10, DATAATTRIBUTIONKIND_WHITE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(5, true); setCode("05ribb");
	setName("リボン"); setFullname("まっかなリボンちゃん");
	setProfile(
		"むぎわらが つくった ヒトガタ\n"
		"パペット.あか ぞくせい.おお\n"
		"きくて まっかな リボンが\n"
		"チャームポイント.わりと くち\n"
		"が わるい.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(115, 80, DATAATTRIBUTIONKIND_RED);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(6, true); setCode("06pebu");
	setName("ペンギー"); setFullname("ペンギンバギー");
	setProfile(
		"むぎわらが つくった モンスタ\n"
		"パペット.あお ぞくせい.ペン\n"
		"ギンを ベースに つくられたが\n"
		"タイヤも つけてみたので レー\n"
		"スとか できちゃうかも.");
	setCharaKind(HKNWCHARAKIND_CART);
	setAttack(80, 120, DATAATTRIBUTIONKIND_BLUE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(7, true); setCode("07skul");
	setName("されこうべ"); setFullname("しゃれこうべ");
	setProfile(
		"えにしが つくった モンスタ\n"
		"パペット.あお ぞくせい.あお\n"
		"き あくまの アンデットは エ\n"
		"ターナル フォース ブリザード\n"
		"を あやつり あいては しぬ.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(85, 125, DATAATTRIBUTIONKIND_BLUE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(8, true); setCode("08dete");
	setName("たんてい"); setFullname("しょうねんたんてい");
	setProfile(
		"えにしが つくった ヒトガタ\n"
		"パペット.あお ぞくせい.あく\n"
		"まを しえきし あくを うつ.\n"
		"けんぞくでも かなわない あい\n"
		"てには みずから エターナル\n"
		"フォース ブリザードを はなち\n"
		"あいては しぬ.(せっていでは\n"
		"たんていだが,あまり たんてい\n"
		"かぎょうは しないみたいだ.)");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(130, 90, DATAATTRIBUTIONKIND_BLUE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(9, true); setCode("09dogg");
	setName("いぬ"); setFullname("ふわふわにそくほこうイヌ");
	setProfile(
		"イヌが つくった ぬいぐるみ\n"
		"パペット.あか ぞくせい.しっ\n"
		"ぽが やたらと おおきく なっ\n"
		"て しまったが それも きっと\n"
		"チャームポイント.リスでは\n"
		"ない.");
	setCharaKind(HKNWCHARAKIND_KUMA);
	setAttack(90, 135, DATAATTRIBUTIONKIND_RED);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(10, true); setCode("10fang");
	setName("ファンガ"); setFullname("ファングオブガイア");
	setProfile(
		"イヌが つくった モンスタ パ\n"
		"ペット.すこし おもい.あお\n"
		"ぞくせい.だいちを きりさく\n"
		"でんせつの ドラゴンを モデル\n"
		"にした パペット.");
	setCharaKind(HKNWCHARAKIND_DRAGON);
	setAttack(95, 140, DATAATTRIBUTIONKIND_BLUE);
	setRatio(20, 100);
	setWeight(100);
	createData();

	initData(11, true); setCode("11mami");
	setName("マミ"); setFullname("ぐるぐるマミ");
	setProfile(
		"アリスが つくった ヒトガタ\n"
		"パペット.しろ ぞくせい.いろ\n"
		"いろと ふこうで つらいことも\n"
		"いっぱい あったけど なんとか\n"
		"いきています.");
	setCharaKind(HKNWCHARAKIND_HUMAN2);
	setAttack(145, 100, DATAATTRIBUTIONKIND_WHITE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(12, true); setCode("12holy");
	setName("ホーリー"); setFullname("ホーリードール");
	setProfile(
		"アリスが つくった ヒトガタ\n"
		"パペット.すこし かるい.きい\n"
		"ろ ぞくせい.きいろの ちから\n"
		"は ちつじょの ちから.ゆうこ\n"
		"うな モンスターは すくないが\n"
		"いちぶ きいろいがい うけつけ\n"
		"ない モンスターも いる.でも\n"
		"じょばんは やくにたたない.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(150, 100, DATAATTRIBUTIONKIND_YELLOW);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(13, true); setCode("13bone");
	setName("がいこつ"); setFullname("がいこつおとこ");
	setProfile(
		"おばけマンが つくった モンス\n"
		"タ パペット.しろ ぞくせい.\n"
		"がいこつふうの パペットだが\n"
		"つくりは わりと てきとう.お\n"
		"ばけマンに いがくの ちしき\n"
		"なし.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(105, 155, DATAATTRIBUTIONKIND_WHITE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(14, true); setCode("14armo");
	setName("アーナイト"); setFullname("アーマーナイト");
	setProfile(
		"おばけマンが つくった モンス\n"
		"タ パペット.すこし おもい.\n"
		"あか ぞくせい.よろいの チカ\n"
		"ラで てっぺきの ぼうぎょりょ\n"
		"くを ほこる.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(110, 160, DATAATTRIBUTIONKIND_RED);
	setRatio(20, 100);
	setWeight(100);
	createData();

	initData(15, false); setCode("15kobu");
	setName("コロブン"); setFullname("コロコロブンブン");
	setProfile(
		"ランク2の マイスターが つく\n"
		"った モンスタ パペット.すこ\n"
		"し かるい.しろ ぞくせい.\n"
		"マイスターは この まるさを\n"
		"だすために けっこう くろう\n"
		"している らしい.");
	setCharaKind(HKNWCHARAKIND_BALL);
	setAttack(160, 240, DATAATTRIBUTIONKIND_WHITE);
	setRatio(20, 100);
	setWeight(-100);
	createData();

	initData(16, false); setCode("16kobo");
	setName("コロボン"); setFullname("コロコロボンボン");
	setProfile(
		"ランク2の マイスターが つく\n"
		"った モンスタ パペット.すこ\n"
		"し かるい.みどり ぞくせい.\n"
		"ころがって いどう するが,と\n"
		"くしゅな くんれんを うけて\n"
		"いるので めが まわることは\n"
		"ないらしい.");
	setCharaKind(HKNWCHARAKIND_BALL);
	setAttack(180, 270, DATAATTRIBUTIONKIND_GREEN);
	setRatio(20, 100);
	setWeight(-100);
	createData();

	initData(17, false); setCode("17hand");
	setName("てんち"); setFullname("てんちはんてん");
	setProfile(
		"キャロルが つくった ヒトガタ\n"
		"パペット.しろ ぞくせい.つね\n"
		"に さかだちを している.おふ\n"
		"ろに はいるとき とかも さか\n"
		"だち なのかな.");
	setCharaKind(HKNWCHARAKIND_HANDSTAND);
	setAttack(330, 220, DATAATTRIBUTIONKIND_WHITE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(18, false); setCode("18rinn");
	setName("リン"); setFullname("ヒーローみならいリン");
	setProfile(
		"キャロルが つくった ヒトガタ\n"
		"パペット.きいろ ぞくせい.ゆ\n"
		"うき リンリン.まがったことは\n"
		"ゆるせない.すきな えいゆうは\n"
		"れっど ぜんぱん.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(340, 230, DATAATTRIBUTIONKIND_YELLOW);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(19, false); setCode("19debu");
	setName("マシュマロ"); setFullname("マシュマロチョコレートソース");
	setProfile(
		"パピプポペが つくった ヒトガ\n"
		"タ パペット.すこし おもい.\n"
		"あか ぞくせい.あまいものが\n"
		"だいすき.マシュマロは ぼうの\n"
		"さきに とりつけて ひで あぶ\n"
		"りたくなる.");
	setCharaKind(HKNWCHARAKIND_HUMAN2);
	setAttack(350, 235, DATAATTRIBUTIONKIND_RED);
	setRatio(20, 100);
	setWeight(100);
	createData();

	initData(20, false); setCode("20tare");
	setName("たれみみ"); setFullname("たれみみパーカー");
	setProfile(
		"パピプポペが つくった ヒトガ\n"
		"タ パペット.あお ぞくせい.\n"
		"おきにいりの たれみみ パーカ\n"
		"ーを つねに みにつけて いる\n"
		"みかんが すき.");
	setCharaKind(HKNWCHARAKIND_HUMAN3);
	setAttack(360, 240, DATAATTRIBUTIONKIND_BLUE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(21, false); setCode("21weak");
	setName("カンパ"); setFullname("じゃくてんカンパくん");
	setProfile(
		"パピプポペが つくった ヒトガ\n"
		"タ パペット.モンスターの じ\n"
		"ゃくてんぶいを みつける ちか\n"
		"らを もつ.あお ぞくせい.じ\n"
		"つは おべんきょうは にがて.");
	setCharaKind(HKNWCHARAKIND_HUMAN3);
	setAttack(30, 20, DATAATTRIBUTIONKIND_BLUE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(22, false); setCode("22bcoo");
	setName("Bクッキー"); setFullname("バタークッキー");
	setProfile(
		"ババアが つくった おいしい\n"
		"クッキー.すこし かるい.しろ\n"
		"ぞくせい.パペットではなく ク\n"
		"ッキー なのだが パペットつか\n"
		"いの コッペンは もんだいなく\n"
		"あやつれる.なぜだ.");
	setCharaKind(HKNWCHARAKIND_COOKIE);
	setAttack(2, 3, DATAATTRIBUTIONKIND_WHITE);
	setRatio(20, 100);
	setWeight(-100);
	createData();

	initData(23, false); setCode("23ccoo");
	setName("Cクッキー"); setFullname("チョコチップクッキー");
	setProfile(
		"ババアが つくった おいしい\n"
		"クッキー.すこし かるい.あか\n"
		"ぞくせい.ぞうしょく りょくが\n"
		"はんぱない どえらい クッキー\n"
		"こんだけ つくって たべるひと\n"
		"は いるのかな.");
	setCharaKind(HKNWCHARAKIND_COOKIE);
	setAttack(2, 3, DATAATTRIBUTIONKIND_RED);
	setRatio(20, 100);
	setWeight(-100);
	createData();

	initData(24, false); setCode("24tcoo");
	setName("Tクッキー"); setFullname("おちゃっぱクッキー");
	setProfile(
		"ババアが つくった OC クッ\n"
		"キー.すこし かるい.みどり\n"
		"ぞくせい.くちのなかに おちゃ\n"
		"の かおりが ひろがって OC\n"
		"です.");
	setCharaKind(HKNWCHARAKIND_COOKIE);
	setAttack(2, 3, DATAATTRIBUTIONKIND_GREEN);
	setRatio(20, 100);
	setWeight(-100);
	createData();

	initData(25, false); setCode("25guru");
	setName("ぐるり"); setFullname("たかさき ぐるり");
	setProfile(
		"レイナが つくった ヒトガタ\n"
		"パペット.しろ ぞくせい.じゅ\n"
		"うりょくの ほうこうを じざい\n"
		"に あやつる ちからを もつ.\n"
		"おべんとを つくるのが とくい\n"
		"もふーっ.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(370, 250, DATAATTRIBUTIONKIND_WHITE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(26, false); setCode("26koni");
	setName("コニワ"); setFullname("コニワ グリーンフィールド");
	setProfile(
		"レイナが つくった ヒトガタ\n"
		"パペット.みどり ぞくせい.だ\n"
		"いちの ちけいを かえる ちか\n"
		"らを もつ.キックに ていひょ\n"
		"うが あり,まるいものを みた\n"
		"ら けらずには いられない.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(380, 255, DATAATTRIBUTIONKIND_GREEN);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(27, false); setCode("27pebe");
	setName("ペングル"); setFullname("ペンギンビーグル");
	setProfile(
		"ペンギンが つくった モンスタ\n"
		"パペット.みどり ぞくせい.ペ\n"
		"ンギンを ベースに つくられた\n"
		"ハイスピード ビーグル.みつぼ\n"
		"しの ぎじゅつが おしみなく\n"
		"つかわれている.");
	setCharaKind(HKNWCHARAKIND_CART);
	setAttack(260, 390, DATAATTRIBUTIONKIND_GREEN);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(28, false); setCode("28buta");
	setName("ぶたボール"); setFullname("まんまるぶたボール");
	setProfile(
		"ペンギンが つくった ぬいぐる\n"
		"み パペット.みどり ぞくせい\n"
		"ぶたを ベースに つくられた\n"
		"ふしぎな どうぶつ.まるいので\n"
		"ひょうめんせきは ちいさく,た\n"
		"ぶん さむさに つよい.");
	setCharaKind(HKNWCHARAKIND_BUTA);
	setAttack(270, 400, DATAATTRIBUTIONKIND_GREEN);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(29, false); setCode("29koga");
	setName("コロガン"); setFullname("コロコロガンガン");
	setProfile(
		"ランク3の マイスターが つく\n"
		"った モンスタ パペット.すこ\n"
		"し かるい.みどり ぞくせい.\n"
		"ちかくに こどもが いたら ま\n"
		"ちがいなく けられるので こど\n"
		"もが キライ.");
	setCharaKind(HKNWCHARAKIND_BALL);
	setAttack(400, 600, DATAATTRIBUTIONKIND_GREEN);
	setRatio(20, 100);
	setWeight(-100);
	createData();

	initData(30, false); setCode("30bear");
	setName("たまごクマ"); setFullname("まるまるたまごクマ");
	setProfile(
		"エイミーが つくった ぬいぐる\n"
		"み パペット.あか ぞくせい.\n"
		"きあいで クマの ぬいぐるみを\n"
		"つくったら こんなんできました\n"
		"かべの うえに すわったら お\n"
		"ちて われちゃう.");
	setCharaKind(HKNWCHARAKIND_BEAR);
	setAttack(550, 825, DATAATTRIBUTIONKIND_RED);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(31, false); setCode("31titi");
	setName("チィチィ"); setFullname("ミョン チィチィ");
	setProfile(
		"エイミーが つくった ヒトガタ\n"
		"パペット.しろ ぞくせい.アル\n"
		"アル うるさい エセ ちゅうか\n"
		"むすめ.ぐんまけん しゅっしん\n"
		"ギョウザが すき.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(850, 570, DATAATTRIBUTIONKIND_WHITE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(32, false); setCode("32ruck");
	setName("リュック"); setFullname("リュックサックとツインテール");
	setProfile(
		"はにゃんが つくった ヒトガタ\n"
		"パペット.あお ぞくせい.リュ\n"
		"ックサックの なかには ゆめと\n"
		"ガラクタが つまっている.\n"
		"カッパでは ないし みずも\n"
		"あやつれない.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(875, 585, DATAATTRIBUTIONKIND_BLUE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(33, false); setCode("33guda");
	setName("ぐだぐだ"); setFullname("ぐだぐだはにゃん");
	setProfile(
		"はにゃんが つくった ヒトガタ\n"
		"パペット.みどり ぞくせい.あ\n"
		"んな すてきな おねえさんから\n"
		"こんな だらけた パペットが\n"
		"うまれるなんて しんじられない");
	setCharaKind(HKNWCHARAKIND_GUDAGUDA);
	setAttack(900, 600, DATAATTRIBUTIONKIND_GREEN);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(34, false); setCode("34fadd");
	setName("ファッド"); setFullname("ファングオブゴッド");
	setProfile(
		"フハッハが つくった モンスタ\n"
		"パペット. すこし おもい.あ\n"
		"か ぞくせい.その おたけびは\n"
		"みみせんをも つきぬけて だい\n"
		"ダメージを あたえる.");
	setCharaKind(HKNWCHARAKIND_DRAGON);
	setAttack(620, 925, DATAATTRIBUTIONKIND_RED);
	setRatio(20, 100);
	setWeight(100);
	createData();

	initData(35, false); setCode("35abso");
	setName("アブソル"); setFullname("アブソリュートアーマー");
	setProfile(
		"フハッハが つくった モンスタ\n"
		"パペット. すこし おもい.あ\n"
		"お ぞくせい.どんな こうげき\n"
		"をも むこうにする ぜったいて\n"
		"きな ぼうぎょりょくを ほこる\n"
		"よろいを みにつけている.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(635, 950, DATAATTRIBUTIONKIND_BLUE);
	setRatio(20, 100);
	setWeight(100);
	createData();

	initData(36, false); setCode("36robo");
	setName("ロボット"); setFullname("うちゅうロボット");
	setProfile(
		"こうじょうで つくられた ハイ\n"
		"テク パペット.すこし おもい\n"
		"しろ ぞくせい.うちゅうかいは\n"
		"つに つかう ばんのう たんさ\n"
		"く ロボットを モデルとして\n"
		"つくられた パペット.");
	setCharaKind(HKNWCHARAKIND_ROBO);
	setAttack(650, 975, DATAATTRIBUTIONKIND_WHITE);
	setRatio(20, 100);
	setWeight(100);
	createData();

	initData(37, false); setCode("37rock");
	setName("ロケット"); setFullname("うちゅうロケット");
	setProfile(
		"こうじょうで つくられた ハイ\n"
		"テク パペット.すこし かるい\n"
		"あか ぞくせい.ほんものの ロ\n"
		"ケットでは ないので すいしん\n"
		"そうちは つけていない.");
	setCharaKind(HKNWCHARAKIND_ROCKET);
	setAttack(670, 1000, DATAATTRIBUTIONKIND_RED);
	setRatio(20, 100);
	setWeight(-100);
	createData();

	initData(38, true); setCode("38pika");
	setName("ピカリ"); setFullname("ピカリ キラ");
	setProfile(
		"ポコペンが つくった ヒトガタ\n"
		"パペット.きいろ ぞくせい.し\n"
		"ゅぎょうの ために あたまを\n"
		"まるめた そうりょ.とても し\n"
		"んじんぶかく かみをひていする\n"
		"はつげんを きくと あわをはい\n"
		"て きぜつする.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(480, 360, DATAATTRIBUTIONKIND_YELLOW);
	setRatio(20, 100);
	setWeight(0);
	createData();

	initData(39, true); setCode("39copp");
	setName("コッペン"); setFullname("コッペン スワニルダ");
	setProfile(
		"コッペンが つくった ヒトガタ\n"
		"パペット.しろ ぞくせい.ほん\n"
		"にんを モデルとした パペット\n"
		"は せいかくも ほんにんに\n"
		"にるみたい.この パペットも\n"
		"じつは あまえんぼな ところが\n"
		"そっくり.");
	setCharaKind(HKNWCHARAKIND_HUMAN);
	setAttack(180, 120, DATAATTRIBUTIONKIND_WHITE);
	setRatio(20, 100);
	setWeight(0);
	createData();

	return 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

