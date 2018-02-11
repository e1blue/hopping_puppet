#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../core/data.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

char *dir = "gacha";
typedef struct dataGacha Data;

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
void setMoney(unsigned int money){data.money = money;}
void setProduct(signed short meisterKind, unsigned short frequency, unsigned char isFirst){
	if(indexProduct >= DATAGACHA_PRODUCTMAX){fprintf(stderr, "%4d: %s is too long!!\n", kind, "product"); exit(1);}
	data.product[indexProduct].meisterKind = meisterKind;
	data.product[indexProduct].frequency = frequency;
	data.product[indexProduct].isFirst = isFirst;
	indexProduct++;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// main関数 処理はここから始まる
int main(int argc, char *argv[]){

	initData(0, true); setCode("00level1");
	setName("マイスターガチャLv.1");
	setMoney(100);
	setProduct( 0, 10, 0);
	setProduct( 1, 10, 0);
	setProduct( 2, 20, 1);
	setProduct( 3, 18, 1);
	setProduct( 4, 16, 1);
	setProduct( 5, 14, 1);
	setProduct( 6, 12, 1);
	createData();

#ifdef __TRIAL__

	initData(1, true); setCode("01level2");
	setName("マイスターガチャLv.2");
	setMoney(1000);
	createData();

	initData(2, true); setCode("02level3");
	setName("マイスターガチャLv.3");
	setMoney(10000);
	createData();

#else

	initData(1, false); setCode("01level2");
	setName("マイスターガチャLv.2");
	setMoney(1000);
	setProduct( 1,  5, 0);
	setProduct( 5,  7, 0);
	setProduct( 6,  7, 0);
	setProduct( 7, 26, 1);
	setProduct( 8, 22, 1);
	setProduct( 9,  1, 1);
	setProduct(10, 18, 1);
	setProduct(11, 14, 1);
	createData();

	initData(2, false); setCode("02level3");
	setName("マイスターガチャLv.3");
	setMoney(10000);
	setProduct( 1,  2, 0);
	setProduct(10,  7, 0);
	setProduct(11,  7, 0);
	setProduct(12, 27, 1);
	setProduct(13, 23, 1);
	setProduct(14, 19, 1);
	setProduct(15, 15, 1);
	createData();

#endif

	return 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

