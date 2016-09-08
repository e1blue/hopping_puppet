#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../core/data.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

char *dir = "background";
typedef struct dataBackground Data;

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
void setSize(unsigned short w, unsigned short h){data.w = w; data.h = h;}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// main関数 処理はここから始まる
int main(int argc, char *argv[]){

	initData(0, true); setCode("00grass");
	setSize(256, 512);
	createData();

	initData(1, false); setCode("01beach");
	setSize(256, 512);
	createData();

	initData(2, false); setCode("02forest");
	setSize(256, 512);
	createData();

	initData(3, false); setCode("03craggy");
	setSize(256, 512);
	createData();

	initData(4, false); setCode("04volcano");
	setSize(256, 512);
	createData();

	initData(5, false); setCode("05snow");
	setSize(256, 512);
	createData();

	initData(6, false); setCode("06cake");
	setSize(256, 512);
	createData();

	return 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

