#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../core/data.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

char *dir = "enemy";
typedef struct dataEnemy Data;

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

int indexWeapon;
int indexWeak;
void setCode(char *code){SETSTR(code, data.code); indexWeapon = indexWeak = 0;}
void setName(char *name){SETSTR(name, data.name);}
void setFullname(char *fullname){SETSTR(fullname, data.fullname);}

void setHealthRatio(unsigned char healthRatio100){data.healthRatio100 = healthRatio100;}
void setMotionType(enum dataEnemyMotionType motionType){data.motionType = motionType;}
void setCharge(unsigned short charge){data.charge = charge;}
void setWeapon(unsigned char imageIndex, unsigned char radius, unsigned char healthRatio10, unsigned char attackRatio100, enum dataEnemyWeaponMotionType motionType, enum dataAttributionKind attributionKind, unsigned char white, unsigned char red, unsigned char green, unsigned char blue, unsigned char yellow){
	if(indexWeapon >= DATAENEMY_WEAPONNUM){fprintf(stderr, "%4d: %s is too long!!\n", kind, "weapon"); exit(1);}
	data.weapon[indexWeapon].imageIndex = imageIndex;
	data.weapon[indexWeapon].radius = radius;
	data.weapon[indexWeapon].healthRatio10 = healthRatio10;
	data.weapon[indexWeapon].attackRatio100 = attackRatio100;
	data.weapon[indexWeapon].motionType = motionType;
	data.weapon[indexWeapon].attributionKind = attributionKind;
	data.weapon[indexWeapon].attributionRatio10[DATAATTRIBUTIONKIND_WHITE] = white;
	data.weapon[indexWeapon].attributionRatio10[DATAATTRIBUTIONKIND_RED] = red;
	data.weapon[indexWeapon].attributionRatio10[DATAATTRIBUTIONKIND_GREEN] = green;
	data.weapon[indexWeapon].attributionRatio10[DATAATTRIBUTIONKIND_BLUE] = blue;
	data.weapon[indexWeapon].attributionRatio10[DATAATTRIBUTIONKIND_YELLOW] = yellow;
	indexWeapon++;
}
void setWeak(signed char x, signed char y, unsigned char radius, unsigned char white, unsigned char red, unsigned char green, unsigned char blue, unsigned char yellow){
	if(indexWeak >= DATAENEMY_WEAKNUM){fprintf(stderr, "%4d: %s is too long!!\n", kind, "weak"); exit(1);}
	data.weak[indexWeak].x = x;
	data.weak[indexWeak].y = y;
	data.weak[indexWeak].radius = radius;
	data.weak[indexWeak].attributionRatio10[DATAATTRIBUTIONKIND_WHITE] = white;
	data.weak[indexWeak].attributionRatio10[DATAATTRIBUTIONKIND_RED] = red;
	data.weak[indexWeak].attributionRatio10[DATAATTRIBUTIONKIND_GREEN] = green;
	data.weak[indexWeak].attributionRatio10[DATAATTRIBUTIONKIND_BLUE] = blue;
	data.weak[indexWeak].attributionRatio10[DATAATTRIBUTIONKIND_YELLOW] = yellow;
	indexWeak++;
}

void setMoneyRatio(unsigned char moneyRatio100){data.moneyRatio100 = moneyRatio100;}
void setExperienceRatio(unsigned char experienceRatio100){data.experienceRatio100 = experienceRatio100;}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// main関数 処理はここから始まる
int main(int argc, char *argv[]){

	initData(0, true); setCode("00dragon");
	setName("おおとかげ"); setFullname("おおきなとかげ");
	setHealthRatio( 90);
	setMotionType(DATAENEMYMOTIONTYPE_HORIZON);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeak(  0,   0, 100, 10,  8, 10, 14,  5);
	setWeak(-50, -25,  45, 20,  8, 20, 28,  5);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(1, false); setCode("01dragon");
	setName("スパドラ"); setFullname("スーパードラゴン");
	setHealthRatio( 90);
	setMotionType(DATAENEMYMOTIONTYPE_HORIZON);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeak(  0,   0, 100, 10,  8, 10, 14,  5);
	setWeak(-50, -25,  45, 20,  8, 20, 28,  5);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(2, false); setCode("02dragon");
	setName("レレックス"); setFullname("レトロレックス");
	setHealthRatio( 90);
	setMotionType(DATAENEMYMOTIONTYPE_HORIZON);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeak(  0,   0, 100, 10,  8, 10, 14,  5);
	setWeak(-50, -25,  45, 20,  8, 20, 28,  5);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(3, true); setCode("03fish");
	setName("たらこうお"); setFullname("たらこくちびるうお");
	setHealthRatio( 90);
	setMotionType(DATAENEMYMOTIONTYPE_SLASH);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 14,  8,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 14,  8,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 14,  8,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 14,  8,  5);
	setWeak(  0,   0, 100, 10, 10, 14,  8,  5);
	setWeak(-55,  15,  55, 20, 20, 28,  8,  5);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(4, false); setCode("04fish");
	setName("しおから"); setFullname("しおからいさかな");
	setHealthRatio( 90);
	setMotionType(DATAENEMYMOTIONTYPE_SLASH);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 14,  8,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 14,  8,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 14,  8,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 14,  8,  5);
	setWeak(  0,   0, 100, 10, 10, 14,  8,  5);
	setWeak(-55,  15,  55, 20, 20, 28,  8,  5);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(5, false); setCode("05fish");
	setName("めんたいこ"); setFullname("めんたいこがスキなさかな");
	setHealthRatio( 90);
	setMotionType(DATAENEMYMOTIONTYPE_SLASH);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 14,  8,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 14,  8,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 14,  8,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 14,  8,  5);
	setWeak(  0,   0, 100, 10, 10, 14,  8,  5);
	setWeak(-55,  15,  55, 20, 20, 28,  8,  5);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(6, true); setCode("06dog");
	setName("ビーグル"); setFullname("ビーグルけん");
	setHealthRatio( 90);
	setMotionType(DATAENEMYMOTIONTYPE_INFINIT);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_GREEN, 10, 14,  8, 10,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_GREEN, 10, 14,  8, 10,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_GREEN, 10, 14,  8, 10,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_GREEN, 10, 14,  8, 10,  5);
	setWeak(  0,   0, 100, 10, 14,  8, 10,  5);
	setWeak(  0,   0,  50, 20, 28,  8, 20,  5);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(7, false); setCode("07dog");
	setName("ラッセル"); setFullname("ラッセルけん");
	setHealthRatio( 90);
	setMotionType(DATAENEMYMOTIONTYPE_INFINIT);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_GREEN, 10, 14,  8, 10,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_GREEN, 10, 14,  8, 10,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_GREEN, 10, 14,  8, 10,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_GREEN, 10, 14,  8, 10,  5);
	setWeak(  0,   0, 100, 10, 14,  8, 10,  5);
	setWeak(  0,   0,  50, 20, 28,  8, 20,  5);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(8, true); setCode("08bird");
	setName("フクロロ"); setFullname("フクロロオウル");
	setHealthRatio( 90);
	setMotionType(DATAENEMYMOTIONTYPE_VERTICAL);
	setCharge(2);
	setWeapon(0, 50,  2,  20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50,  2,  20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50,  2,  20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50,  2,  20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeak(  0,   0, 100, 10,  8, 10, 14,  5);
	setWeak(  5,  25,  45, 20,  8, 20, 28,  5);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(9, false); setCode("09bird");
	setName("ホホウホウ"); setFullname("ホウホウホホウホウ");
	setHealthRatio( 90);
	setMotionType(DATAENEMYMOTIONTYPE_VERTICAL);
	setCharge(2);
	setWeapon(0, 50,  2,  20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50,  2,  20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50,  2,  20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeapon(0, 50,  2,  20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  8, 10, 14,  5);
	setWeak(  0,   0, 100, 10,  8, 10, 14,  5);
	setWeak(  5,  25,  45, 20,  8, 20, 28,  5);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(10, false); setCode("10elephant");
	setName("パオブー"); setFullname("パオパオブータレ");
	setHealthRatio(100);
	setMotionType(DATAENEMYMOTIONTYPE_HORIZON);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_GREEN, 10, 20,  5, 10,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_GREEN, 10, 20,  5, 10,  5);
	setWeapon(1, 50, 40,   1, DATAENEMYWEAPONMOTIONTYPE_SHIELD1, DATAATTRIBUTIONKIND_GREEN, 10, 20,  5, 10, 30);
	setWeapon(1, 50, 40,   1, DATAENEMYWEAPONMOTIONTYPE_SHIELD1, DATAATTRIBUTIONKIND_GREEN, 10, 20,  5, 10, 30);
	setWeak(  0,   0, 100, 10, 20,  5, 10,  1);
	setWeak(  0,   0,  30, 30, 60, 30, 30, 10);
	setWeak(-45, -15,  15, 30, 60, 30, 30, 10);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(11, false); setCode("11elephant");
	setName("パオーガ"); setFullname("パオパオオーガ");
	setHealthRatio(100);
	setMotionType(DATAENEMYMOTIONTYPE_HORIZON);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_GREEN, 10, 20,  5, 10,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_GREEN, 10, 20,  5, 10,  5);
	setWeapon(1, 50, 40,   1, DATAENEMYWEAPONMOTIONTYPE_SHIELD1, DATAATTRIBUTIONKIND_GREEN, 10, 20,  5, 10, 30);
	setWeapon(1, 50, 40,   1, DATAENEMYWEAPONMOTIONTYPE_SHIELD1, DATAATTRIBUTIONKIND_GREEN, 10, 20,  5, 10, 30);
	setWeak(  0,   0, 100, 10, 20,  5, 10,  1);
	setWeak(  0,   0,  30, 30, 60, 30, 30, 10);
	setWeak(-45, -15,  15, 30, 60, 30, 30, 10);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(12, false); setCode("12frog");
	setName("ケローガ"); setFullname("ケローガフロッグ");
	setHealthRatio(100);
	setMotionType(DATAENEMYMOTIONTYPE_VERTICAL);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 20,  5,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 20,  5,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 20,  5,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 20,  5,  5);
	setWeak(  0,   0, 100, 20, 10, 20,  5,  1);
	setWeak( 10,  30,  45, 40, 20, 40,  5,  1);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(13, false); setCode("13frog");
	setName("フローガ"); setFullname("フローガフロッグ");
	setHealthRatio(100);
	setMotionType(DATAENEMYMOTIONTYPE_VERTICAL);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 20,  5,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 20,  5,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 20,  5,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_BLUE, 10, 10, 20,  5,  5);
	setWeak(  0,   0, 100, 20, 10, 20,  5,  1);
	setWeak( 10,  30,  45, 40, 20, 40,  5,  1);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(14, false); setCode("14bug");
	setName("ノミ"); setFullname("ノミ");
	setHealthRatio( 60);
	setMotionType(DATAENEMYMOTIONTYPE_INFINIT);
	setCharge(2);
	setWeapon(0, 40,  2,  20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_GREEN, 20, 20,  5, 10,  5);
	setWeapon(0, 40,  2,  20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_GREEN, 20, 20,  5, 10,  5);
	setWeapon(0, 40,  2,  20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_GREEN, 20, 20,  5, 10,  5);
	setWeapon(0, 40,  2,  20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_GREEN, 20, 20,  5, 10,  5);
	setWeak(  0,   0,  40, 20, 20,  5, 10,  1);
	setWeak(-13, -16,  20, 40, 40,  5, 20,  1);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(15, false); setCode("15crab");
	setName("カニカニ"); setFullname("ヤドカニカニ");
	setHealthRatio(100);
	setMotionType(DATAENEMYMOTIONTYPE_STOP);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_BLUE,  5, 10, 20,  5,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_BLUE,  5, 10, 20,  5,  5);
	setWeapon(1, 50, 40,   1, DATAENEMYWEAPONMOTIONTYPE_SHIELD2, DATAATTRIBUTIONKIND_BLUE,  5, 10, 20,  5, 30);
	setWeapon(1, 50, 40,   1, DATAENEMYWEAPONMOTIONTYPE_SHIELD2, DATAATTRIBUTIONKIND_BLUE,  5, 10, 20,  5, 30);
	setWeak(  0,   0, 100,  1,  2,  5,  1,  1);
	setWeak(  5,  40,  50, 20, 25, 60, 15, 10);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(16, false); setCode("16crab");
	setName("ハサミス"); setFullname("ハサミスヤドカリ");
	setHealthRatio(100);
	setMotionType(DATAENEMYMOTIONTYPE_STOP);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_BLUE,  5, 10, 20,  5,  5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_BLUE,  5, 10, 20,  5,  5);
	setWeapon(1, 50, 40,   1, DATAENEMYWEAPONMOTIONTYPE_SHIELD2, DATAATTRIBUTIONKIND_BLUE,  5, 10, 20,  5, 30);
	setWeapon(1, 50, 40,   1, DATAENEMYWEAPONMOTIONTYPE_SHIELD2, DATAATTRIBUTIONKIND_BLUE,  5, 10, 20,  5, 30);
	setWeak(  0,   0, 100,  1,  2,  5,  1,  1);
	setWeak(  5,  40,  50, 20, 25, 60, 15, 10);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(17, false); setCode("17pteranodon");
	setName("ゲルニカ"); setFullname("よくりゅうゲルニカ");
	setHealthRatio( 90);
	setMotionType(DATAENEMYMOTIONTYPE_INFINIT);
	setCharge(2);
	setWeapon(0, 40, 2, 20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  5, 10, 20, 20);
	setWeapon(0, 40, 2, 20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  5, 10, 20, 20);
	setWeapon(0, 40, 2, 20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  5, 10, 20, 20);
	setWeapon(0, 40, 2, 20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  5, 10, 20, 20);
	setWeak( 25,  15,  60, 10,  5, 10, 20, 20);
	setWeak(-40, -35,  50, 20,  5, 20, 40, 40);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(18, false); setCode("18pteranodon");
	setName("ディスト"); setFullname("よくりゅうディストピア");
	setHealthRatio( 90);
	setMotionType(DATAENEMYMOTIONTYPE_INFINIT);
	setCharge(2);
	setWeapon(0, 40, 2, 20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  5, 10, 20, 20);
	setWeapon(0, 40, 2, 20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  5, 10, 20, 20);
	setWeapon(0, 40, 2, 20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  5, 10, 20, 20);
	setWeapon(0, 40, 2, 20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_RED, 10,  5, 10, 20, 20);
	setWeak( 25,  15,  60, 10,  5, 10, 20, 20);
	setWeak(-40, -35,  50, 20,  5, 20, 40, 40);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	initData(19, false); setCode("19ufo");
	setName("ユーフォ"); setFullname("ミカクニンヒコウブッタイ");
	setHealthRatio(100);
	setMotionType(DATAENEMYMOTIONTYPE_WARP);
	setCharge(5);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_DEFAULT, DATAATTRIBUTIONKIND_WHITE, 10, 10, 10, 10, 20);
	setWeapon(0, 50,  2,  20, DATAENEMYWEAPONMOTIONTYPE_SPEED, DATAATTRIBUTIONKIND_WHITE, 10, 10, 10, 10, 20);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_WHITE, 10, 10, 10, 10, 20);
	setWeapon(0, 50, 10, 100, DATAENEMYWEAPONMOTIONTYPE_MEANDERING, DATAATTRIBUTIONKIND_WHITE, 10, 10, 10, 10, 20);
	setWeak(  0,   0,  80,  1,  1,  1,  1, 40);
	setWeak(-10, -80,  40,  8,  8,  8,  8, 80);
	setMoneyRatio(100); setExperienceRatio(100);
	createData();

	return 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

