#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../core/data.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

char *dir = "meister";
typedef struct dataMeister Data;

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

int indexProduct;
void setCode(char *code){SETSTR(code, data.code); indexProduct = 0;}
void setName(char *name){SETSTR(name, data.name);}
void setFullname(char *fullname){SETSTR(fullname, data.fullname);}
void setProfile(char *profile){SETSTR(profile, data.profile);}
void setCharge(unsigned short charge){data.charge = charge;}
void setProduct(signed short puppetKind, unsigned short frequency, unsigned char openLevel){
	if(indexProduct >= DATAMEISTER_PRODUCTMAX){fprintf(stderr, "%4d: %s is too long!!\n", kind, "product"); exit(1);}
	data.product[indexProduct].puppetKind = puppetKind;
	data.product[indexProduct].frequency = frequency;
	data.product[indexProduct].openLevel = openLevel;
	indexProduct++;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// main関数 処理はここから始まる
int main(int argc, char *argv[]){

	initData(0, true); setCode("00copp");
	setName("コッペン"); setFullname("コッペン スワニルダ");
	setProfile(
		"ふしぎな ちからで パペットを\n"
		"あやつる パペットつかいの\n"
		"おんなのこ.パペットつくりも\n"
		"すこし たしなむ が あまり\n"
		"うでは よろしくない.\n"
		"いつも かかえている クマの\n"
		"ぬいぐるみは ちいさい ころに\n"
		"ちちおやに つくって もらった\n"
		"たいせつな たからもの.");
	setCharge(10);
	setProduct( 0, 30,  1);
	setProduct( 1, 30,  2); // Lv.1 共通キャラ
	setProduct( 4, 10,  2); // ボツキャラ
	setProduct(39, 30, 80);
	createData();

	initData(1, true); setCode("01poko");
	setName("ポコペン"); setFullname("すっとこ ポコペン");
	setProfile(
		"たぬきの くせに パペットを\n"
		"つくれる すごいやつ.まあ,\n"
		"しょせん たぬき なので\n"
		"その じつりょくは おさっし.\n"
		"ゆうふくな かていの ペットで\n"
		"ひまを もてあまして みよう\n"
		"みまねで パペットをつくる.\n"
		"がんばったら もしかしたら\n"
		"まともな ものが つくれるよう\n"
		"に なるかもね.");
	setCharge(30);
	setProduct( 4, 50,  1); // ボツキャラ
	setProduct(38, 50, 75);
	createData();

	initData(2, true); setCode("02mugi");
	setName("むぎわら"); setFullname("わらしな つむぎ");
	setProfile(
		"こうこうの ぶかつどうで パペ\n"
		"ットを つくる おんなのこ.\n"
		"リボンや かわいい ものが\n"
		"だいすき.ひふが よわいので\n"
		"がいしゅつ するさいには かな\n"
		"らず むぎわらぼうを かぶる.\n"
		"しょうしょくで ラーメンは\n"
		"いっぽんづつ たべる タイプ.\n"
		"");
	setCharge(40);
	setProduct( 2, 30,  1); // Lv.1 共通キャラ
	setProduct( 4, 10,  1); // ボツキャラ
	setProduct( 5, 40,  1);
	setProduct( 6, 20,  1);
	createData();

	initData(3, true); setCode("03enis");
	setName("えにし"); setFullname("くろかわ えにし");
	setProfile(
		"そこそこ ちゅうにびょうな お\n"
		"とこのこ.りょうしんも ともだ\n"
		"ちも おなじく ちゅうにびょう\n"
		"なので キキかんは うすい.\n"
		"ゆうふくで そぼの いけんが\n"
		"ぜったいな かていに うまれ,\n"
		"そぼの いいつけで パペットを\n"
		"つくっている.さいしょは\n"
		"イヤイヤ だったが さいきんは\n"
		"たのしんで つくっている.");
	setCharge(45);
	setProduct( 3, 30,  1); // Lv.1 共通キャラ
	setProduct( 4, 10,  1); // ボツキャラ
	setProduct( 7, 40,  1);
	setProduct( 8, 20,  1);
	createData();

	initData(4, true); setCode("04dogg");
	setName("イヌ"); setFullname("てさきがきようなイヌ");
	setProfile(
		"とても かしこい イヌ.かしこ\n"
		"すぎて パペットの つくりかた\n"
		"まで おぼえて しまった.\n"
		"かしこくて てさきも きよう\n"
		"だが しゃべったりは しない.\n"
		"ところで みどりいろの イヌっ\n"
		"て じつざい するみたい.\n"
		"こいぬの ころ げんてい\n"
		"みたい だけど.");
	setCharge(50);
	setProduct( 1, 30,  1); // Lv.1 共通キャラ
	setProduct( 4, 10,  1); // ボツキャラ
	setProduct( 9, 40,  1);
	setProduct(10, 20,  1);
	createData();

	initData(5, true); setCode("05alic");
	setName("アリス"); setFullname("アリス B ヴァンガード");
	setProfile(
		"あかるくて げんきな おんなの\n"
		"こ.ドジな しっぱいも おおく\n"
		"かのじょの まわりは いつも\n"
		"にぎやか.パペットを つくる\n"
		"ときも なにかと うるさい.\n"
		"のりものに ひじょうに よわく\n"
		"ブランコや たけうまでも\n"
		"のりもの よいを してしまう.\n"
		"ゲロインの そしつ あり.");
	setCharge(55);
	setProduct( 2, 30,  1); // Lv.1 共通キャラ
	setProduct( 4, 10,  1); // ボツキャラ
	setProduct(11, 40,  1);
	setProduct(12, 20,  1);
	createData();

	initData(6, true); setCode("06ghos");
	setName("おばけマン"); setFullname("おばけだんしゃく");
	setProfile(
		"やみの チカラで パペットを\n"
		"つくっている らしい.ゆびの\n"
		"バンソウコウは やみのチカラの\n"
		"だいしょう なので あろうか.\n"
		"めが わるく なりそうだから\n"
		"むりに やみのチカラを つかう\n"
		"ことなんて ないのに.\n"
		"コーヒーゼリーが だいすき.\n"
		"その りゆうは くろいから.");
	setCharge(60);
	setProduct( 3, 30,  1); // Lv.1 共通キャラ
	setProduct( 4, 10,  1); // ボツキャラ
	setProduct(13, 40,  1);
	setProduct(14, 20,  1);
	createData();

	initData(7, false); setCode("07caro");
	setName("キャロル"); setFullname("キャロル ポプキンス");
	setProfile(
		"とっても よいこ.ごはんは の\n"
		"こさず たべるし おてつだいも\n"
		"かかさない.だいきらいな しい\n"
		"たけも なきながら たべるぞ.\n"
		"はやく おとなに なりたくて\n"
		"ははおやから レディの たしな\n"
		"みとして パペットの つくりか\n"
		"たを おそわって いる.");
	setCharge(120);
	setProduct( 4,  5,  1); // ボツキャラ
	setProduct(15, 15,  1); // Lv.2 共通キャラ
	setProduct(17, 50,  1);
	setProduct(18, 30,  1);
	createData();

	initData(8, false); setCode("08papi");
	setName("パピプポペ"); setFullname("パピプポペ ピペパポブ");
	setProfile(
		"いにしえより つたわる パペッ\n"
		"ト さくせいの ひでんを うけ\n"
		"つぐ おんなのこ.ひでんを う\n"
		"けついでいる とはいえ べつに\n"
		"それいがいは ふつうの おんな\n"
		"のこで あり めだまやきには\n"
		"しょうゆを かけて たべる.\n"
		"もしかしたら ふしぎな ちから\n"
		"を もつ パペットを つくれる\n"
		"かも.");
	setCharge(130);
	setProduct(16, 19,  1); // Lv.2 共通キャラ
	setProduct(19, 50,  1);
	setProduct(20, 30,  1);
	setProduct(21,  1,  1);
	createData();

	initData(9, false); setCode("09baba");
	setName("ババア"); setFullname("クッキー テスラ");
	setProfile(
		"かのじょには パペットを つく\n"
		"る のうりょくは なく,ただた\n"
		"だ ものすごい いきおいで ク\n"
		"ッキーを やくこと のみ とく\n"
		"いとする ごくふつうの おばあ\n"
		"さんだ.かのじょの やく クッ\n"
		"キーは とても おいしく まご\n"
		"は よろこび ふうふげんかも\n"
		"いっぱつで なかなおりだ.");
	setCharge(1);
	setProduct(22, 40, 1);
	setProduct(23, 30, 1);
	setProduct(24, 30, 1);
	createData();

	initData(10, false); setCode("10rena");
	setName("レイナ"); setFullname("きさらぎ レイナ");
	setProfile(
		"くちかずは すくないが しんの\n"
		"とおった おんなのこ.パペット\n"
		"つくりの うでは そこそこ.\n"
		"しゃげきが とくいで ひととお\n"
		"りの じゅうかきを あつかう\n"
		"ことが できる.\n"
		"マフラーを つけている けど\n"
		"そでなし だから すくなくとも\n"
		"ひえしょう では ないんだね.");
	setCharge(140);
	setProduct( 4,  5,  1); // ボツキャラ
	setProduct(15, 15,  1); // Lv.2 共通キャラ
	setProduct(25, 50,  1);
	setProduct(26, 30,  1);
	createData();

	initData(11, false); setCode("11peng");
	setName("ペンギン"); setFullname("ペンギンぎじゅつし");
	setProfile(
		"みつぼしの ぎじゅつを もつ\n"
		"すごい ペンギン.みつぼし な\n"
		"ので ねんりょう でんちを つ\n"
		"くったり かくゆうごう したり\n"
		"びぶん せきぶんの もんだいを\n"
		"いっしゅんで といたり たけの\n"
		"こ よりも きのこ だったり\n"
		"いろいろ すごいのです.ところ\n"
		"で みどりいろのペンギンは\n"
		"ググっても みつからなかった.");
	setCharge(150);
	setProduct( 4,  5,  1); // ボツキャラ
	setProduct(16, 15,  1); // Lv.2 共通キャラ
	setProduct(27, 50,  1);
	setProduct(28, 30,  1);
	createData();

	initData(12, false); setCode("12amie");
	setName("エイミー"); setFullname("エイミー ネイピア");
	setProfile(
		"たいいくかいけいの やかましい\n"
		"おんなのこ.こえが おおきい.\n"
		"まいあさの ハーフ マラソンは\n"
		"かかさない.そばかすは チャー\n"
		"ムポイントだと おもっている.\n"
		"パペットつくりは きあいで\n"
		"おこなっている.いがいと\n"
		"なんとか なるもん ですね.\n"
		"きあいで.");
	setCharge(300);
	setProduct( 4,  2,  1); // ボツキャラ
	setProduct(29,  8,  1); // Lv.3 共通キャラ
	setProduct(30, 60,  1);
	setProduct(31, 30,  1);
	createData();

	initData(13, false); setCode("13hnya");
	setName("はにゃん"); setFullname("はにや あんず");
	setProfile(
		"すてきな おとなの おねえさん\n"
		"パペットつくりも おしゃれに\n"
		"こなし,おりょうりも おじょう\n"
		"ず.かのじょの つくる めだま\n"
		"やきは ぜっぴんとの もっぱら\n"
		"のうわさ.よだんだが,このゲー\n"
		"ムの キャラの なかで せって\n"
		"いじょう にばんめに むねが\n"
		"おおきい.Aカップ");
	setCharge(320);
	setProduct( 4,  2,  1); // ボツキャラ
	setProduct(29,  8,  1); // Lv.3 共通キャラ
	setProduct(32, 60,  1);
	setProduct(33, 30,  1);
	createData();

	initData(14, false); setCode("14fuha");
	setName("フハッハ"); setFullname("ドクター フハッハ");
	setProfile(
		"おおいなる やぼうを もつ\n"
		"エキセントリックで マッドな\n"
		"サイエンティスト.いでんしか\n"
		"なにかを あやつり おそろしい\n"
		"マモノを つくりだす.ウソ.マ\n"
		"モノは つくれないので マモノ\n"
		"の パペットを つくっている.\n"
		"かれが きている うわぎは も\n"
		"とは はくいだった もの.どの\n"
		"くらい きていたの だろうか.");
	setCharge(340);
	setProduct( 4,  2,  1); // ボツキャラ
	setProduct(29,  8,  1); // Lv.3 共通キャラ
	setProduct(34, 60,  1);
	setProduct(35, 30,  1);
	createData();

	initData(15, false); setCode("15fact");
	setName("こうじょう"); setFullname("ロケットこうじょう");
	setProfile(
		"さいせんたんの ぎじゅつを\n"
		"ゆうする うちゅうかいはつ\n"
		"こうじょう.みためは ふつうの\n"
		"こうじょう だが ここの せい\n"
		"ひんは すでに うちゅうで\n"
		"だいかつやく している.\n"
		"その こうじょうには おひる\n"
		"やすみに パペットを ひたすら\n"
		"つくりつづける サークルが\n"
		"そんざいする.");
	setCharge(360);
	setProduct( 4,  2,  1); // ボツキャラ
	setProduct(29,  8,  1); // Lv.3 共通キャラ
	setProduct(36, 60,  1);
	setProduct(37, 30,  1);
	createData();

	return 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

