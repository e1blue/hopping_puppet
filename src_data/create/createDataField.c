#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../core/data.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

char *dir = "field";
typedef struct dataField Data;

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

int indexMap;
int indexEvent;
int indexProduct;
void setCode(char *code){SETSTR(code, data.code); indexMap = indexEvent = indexProduct = 0;}
void setName(char *name){SETSTR(name, data.name);}
void setBackgroundKind(signed short backgroundKindField, signed short backgroundKindEnemy){data.backgroundKindField = backgroundKindField; data.backgroundKindEnemy = backgroundKindEnemy;}

void setSize(unsigned char xsize, unsigned char ysize){data.xsize = xsize; data.ysize = ysize;}
void setOpenCost(unsigned short openCost){data.openCost = openCost;}
void setMap(unsigned char mapchip, unsigned char height){
	if(indexMap >= DATAFIELD_MAPMAX){fprintf(stderr, "%4d: %s is too long!!\n", kind, "map"); exit(1);}
	data.map[indexMap].mapchip = mapchip;
	data.map[indexMap].height = height;
	indexMap++;
}

void setEventIcon(signed char icon, unsigned char fieldx, unsigned char fieldy){
	if(indexEvent >= DATAFIELD_EVENTMAX){fprintf(stderr, "%4d: %s is too long!!\n", kind, "event"); exit(1);}
	data.event[indexEvent].type = DATAFIELDEVENTTYPE_ICON;
	data.event[indexEvent].icon = icon;
	data.event[indexEvent].fieldx = fieldx;
	data.event[indexEvent].fieldy = fieldy;
	indexEvent++;
}
void setEventPrev(signed char icon, unsigned char fieldx, unsigned char fieldy, signed short nextFieldKind, unsigned char nextFieldx, unsigned char nextFieldy, unsigned char nextFieldr){
	if(indexEvent >= DATAFIELD_EVENTMAX){fprintf(stderr, "%4d: %s is too long!!\n", kind, "event"); exit(1);}
	data.event[indexEvent].type = DATAFIELDEVENTTYPE_PREV;
	data.event[indexEvent].icon = icon;
	data.event[indexEvent].fieldx = fieldx;
	data.event[indexEvent].fieldy = fieldy;
	data.event[indexEvent].nextFieldKind = nextFieldKind;
	data.event[indexEvent].nextFieldx = nextFieldx;
	data.event[indexEvent].nextFieldy = nextFieldy;
	data.event[indexEvent].nextFieldr = nextFieldr;
	indexEvent++;
}
void setEventNext(signed char icon, unsigned char fieldx, unsigned char fieldy, signed short nextFieldKind, unsigned char nextFieldx, unsigned char nextFieldy, unsigned char nextFieldr){
	if(indexEvent >= DATAFIELD_EVENTMAX){fprintf(stderr, "%4d: %s is too long!!\n", kind, "event"); exit(1);}
	data.event[indexEvent].type = DATAFIELDEVENTTYPE_NEXT;
	data.event[indexEvent].icon = icon;
	data.event[indexEvent].fieldx = fieldx;
	data.event[indexEvent].fieldy = fieldy;
	data.event[indexEvent].nextFieldKind = nextFieldKind;
	data.event[indexEvent].nextFieldx = nextFieldx;
	data.event[indexEvent].nextFieldy = nextFieldy;
	data.event[indexEvent].nextFieldr = nextFieldr;
	indexEvent++;
}
void setEventTrial(signed char icon, unsigned char fieldx, unsigned char fieldy){
	if(indexEvent >= DATAFIELD_EVENTMAX){fprintf(stderr, "%4d: %s is too long!!\n", kind, "event"); exit(1);}
	data.event[indexEvent].type = DATAFIELDEVENTTYPE_TRIAL;
	data.event[indexEvent].icon = icon;
	data.event[indexEvent].fieldx = fieldx;
	data.event[indexEvent].fieldy = fieldy;
	indexEvent++;
}
void setEventMoney(signed char icon, unsigned char fieldx, unsigned char fieldy, unsigned short money){
	if(indexEvent >= DATAFIELD_EVENTMAX){fprintf(stderr, "%4d: %s is too long!!\n", kind, "event"); exit(1);}
	data.event[indexEvent].type = DATAFIELDEVENTTYPE_MONEY;
	data.event[indexEvent].icon = icon;
	data.event[indexEvent].fieldx = fieldx;
	data.event[indexEvent].fieldy = fieldy;
	data.event[indexEvent].money = money;
	indexEvent++;
}
void setEventItem(signed char icon, unsigned char fieldx, unsigned char fieldy, enum dataItemKind itemKind){
	if(indexEvent >= DATAFIELD_EVENTMAX){fprintf(stderr, "%4d: %s is too long!!\n", kind, "event"); exit(1);}
	data.event[indexEvent].type = DATAFIELDEVENTTYPE_ITEM;
	data.event[indexEvent].icon = icon;
	data.event[indexEvent].fieldx = fieldx;
	data.event[indexEvent].fieldy = fieldy;
	data.event[indexEvent].itemKind = itemKind;
	indexEvent++;
}

void setCharge(unsigned short charge){data.charge = charge;}
void setProduct(signed short enemyKind, unsigned short frequency, unsigned char level, enum dataItemKind itemKind, unsigned char itemPercent){
	if(indexProduct >= DATAFIELD_PRODUCTMAX){fprintf(stderr, "%4d: %s is too long!!\n", kind, "product"); exit(1);}
	data.product[indexProduct].enemyKind = enemyKind;
	data.product[indexProduct].frequency = frequency;
	data.product[indexProduct].level = level;
	data.product[indexProduct].itemKind = itemKind;
	data.product[indexProduct].itemPercent = itemPercent;
	indexProduct++;
}

#ifdef __TRIAL__
#define setEventNextOrTrial(icon, fieldx, fieldy, nextFieldKind, nextFieldx, nextFieldy, nextFieldr) setEventTrial(icon, fieldx, fieldy)
#else
#define setEventNextOrTrial setEventNext
#endif

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// main関数 処理はここから始まる
int main(int argc, char *argv[]){

	// ふわふわへいげん
	// モンスター 0竜 3魚 6犬 8鳥

	initData(0, true); setCode("00grass");
	setName("いえのまわり"); setBackgroundKind(0, 0);
	setSize(4, 4); setCharge(15); setOpenCost(100);
	setMap(0,  0); setMap(0,  1); setMap(0,  1); setMap(0,  0);
	setMap(0,  1); setMap(1,  1); setMap(0,  1); setMap(0,  1);
	setMap(0,  1); setMap(1,  1); setMap(1,  1); setMap(0,  1);
	setMap(0,  1); setMap(0,  1); setMap(0,  1); setMap(0,  0);
	setEventIcon(3, 1, 1);
	setEventIcon(4, 0, 2);
	setEventIcon(4, 2, 0);
	setEventIcon(4, 2, 3);
	setEventNext(0, 2, 2, 1, 1, 1, 0);
	setEventMoney(2, 0, 3, 100);
	setEventItem(2, 3, 1, DATAITEMKIND_CANVASOPEN); // キャンバス拡張 1/12
	setEventItem(-1, 2, 3, DATAITEMKIND_HEALTH);
	setProduct( 0, 60, 1, DATAITEMKIND_CANVASFULL,  0);
	setProduct( 3, 40, 1, DATAITEMKIND_CANVASFULL,  0);
	createData();

	initData(1, true); setCode("00grass");
	setName("ふわふわへいげん1"); setBackgroundKind(0, 0);
	setSize(6, 5); setCharge(20); setOpenCost(110);
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1);
	setMap(0, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 1);
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(1, 1); setMap(0, 1); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(1, 1); setMap(0, 1); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0);
	setEventIcon(4, 1, 2);
	setEventIcon(4, 4, 0);
	setEventIcon(4, 2, 4);
	setEventPrev(1, 1, 1, 0, 2, 2, 0);
	setEventNext(0, 4, 1, 2, 1, 3, 0);
	setEventNext(0, 3, 3, 3, 1, 2, 0);
	setEventMoney(2, 3, 1, 200);
	setEventItem(2, 4, 3, DATAITEMKIND_CANVASOPEN); // キャンバス拡張 2/12
	setEventItem(-1, 5, 0, DATAITEMKIND_TENSION);
	setProduct( 3, 60, 4, DATAITEMKIND_MONSTERMOVE,  0);
	setProduct( 6, 40, 4, DATAITEMKIND_MONSTERMOVE,  15);
	createData();

	initData(2, true); setCode("00grass");
	setName("ふわふわへいげん2"); setBackgroundKind(0, 0);
	setSize(5, 5); setCharge(30); setOpenCost(120);
	setMap(0,  0); setMap(0,  1); setMap(0,  1); setMap(0,  1); setMap(0,  1);
	setMap(0,  1); setMap(1,  1); setMap(0,  2); setMap(1,  1); setMap(0,  1);
	setMap(0,  1); setMap(1,  1); setMap(1,  1); setMap(0,  1); setMap(0,  0);
	setMap(0,  1); setMap(1,  1); setMap(0,  1); setMap(0,  0); setMap(0,  0);
	setMap(0,  1); setMap(0,  1); setMap(0,  0); setMap(0,  0); setMap(0,  0);
	setEventIcon(4, 2, 0);
	setEventIcon(4, 0, 2);
	setEventIcon(4, 1, 4);
	setEventPrev(1, 1, 3, 1, 4, 1, 0);
	setEventMoney(2, 3, 2, 300);
	setEventItem(2, 2, 1, DATAITEMKIND_WORKSPACEOPEN); // ワークスペース拡張 1/3
	setEventItem(2, 1, 0, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 0, 3, DATAITEMKIND_CANVASFULL);
	setProduct( 6, 60, 7, DATAITEMKIND_TENSION,  0);
	setProduct( 8, 40, 7, DATAITEMKIND_TENSION,  5);
	createData();

	initData(3, true); setCode("00grass");
	setName("ふわふわへいげん3"); setBackgroundKind(0, 0);
	setSize(7, 5); setCharge(40); setOpenCost(130);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 0); 
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 2); setMap(0, 1); setMap(0, 0); 
	setMap(0, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 1); setMap(0, 1); 
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(1, 1); setMap(1, 1); setMap(0, 1); 
	setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); 
	setEventIcon(4, 1, 3);
	setEventIcon(4, 5, 0);
	setEventIcon(4, 4, 4);
	setEventPrev(1, 1, 2, 1, 3, 3, 2);
	setEventNextOrTrial(0, 5, 3, 4, 1, 8, 10);
	setEventMoney(2, 6, 2, 200);
	setEventMoney(-1, 4, 0, 200);
	setEventItem(2, 4, 1 ,DATAITEMKIND_CANVASOPEN); // キャンバス拡張 3/12
	setEventItem(2, 3, 4, DATAITEMKIND_MONSTERMOVE);
	setEventItem(2, 2, 3, DATAITEMKIND_HEALTH); 
	setProduct( 0, 60, 10, DATAITEMKIND_HEALTH,  0);
	setProduct( 8, 40, 10, DATAITEMKIND_HEALTH,  5);
	createData();

	// モフモフのもり
	// モンスター 4魚 9鳥 12蛙 14虫 15蟹

	initData(4, false); setCode("02forest");
	setName("モフモフのもり1"); setBackgroundKind(2, 2);
	setSize(8, 10); setCharge(60); setOpenCost(300);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); 
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); 
	setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); 
	setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); 
	setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); 
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setEventIcon(4, 1, 7);
	setEventIcon(4, 1, 9);
	setEventIcon(4, 3, 7);
	setEventIcon(4, 4, 4);
	setEventIcon(4, 2, 5);
	setEventIcon(4, 2, 2);
	setEventIcon(4, 5, 1);
	setEventIcon(3, 0, 9);
	setEventIcon(3, 2, 6);
	setEventIcon(3, 2, 3);
	setEventIcon(3, 4, 0);
	setEventIcon(3, 5, 4);
	setEventIcon(3, 3, 3);
	setEventIcon(3, 4, 5);
	setEventIcon(5, 4, 2);
	setEventIcon(5, 3, 1);
	setEventPrev(1, 1, 8, 3, 5, 3, 0);
	setEventNext(0, 6, 5, 5, 8, 5, 10);
	setEventNext(0, 4, 1, 6, 1, 1, 0);
	setEventMoney(2, 3, 5, 400);
	setEventMoney(-1, 4, 6, 400);
	setEventItem(2, 6, 6, DATAITEMKIND_HEALTH);
	setEventItem(2, 4, 3, DATAITEMKIND_TENSION);
	setEventItem(2, 5, 0, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 2, 4, DATAITEMKIND_HEALTH);
	setProduct( 9, 60, 15, DATAITEMKIND_CANVASFULL,  0);
	setProduct(12, 40, 15, DATAITEMKIND_CANVASFULL,  2);
	createData();

	initData(5, false); setCode("01beach");
	setName("ふかふかかいがん"); setBackgroundKind(1, 1);
	setSize(14, 7); setCharge(62); setOpenCost(350);
	setMap(0, 0); setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); 
	setMap(0, 0); setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0); 
	setMap(0, 0); setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 0); setMap(0, 0); 
	setMap(0, 0); setMap(2, 1); setMap(2, 1); setMap(2, 1); setMap(2, 1); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(2, 1); setMap(2, 2); setMap(2, 1); setMap(2, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(2, 1); setMap(2, 1); setMap(2, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setEventIcon(3, 7, 3);
	setEventIcon(3, 10, 4);
	setEventIcon(3, 8, 6);
	setEventIcon(4, 1, 5);
	setEventIcon(4, 2, 3);
	setEventIcon(4, 4, 3);
	setEventPrev(1, 8, 5, 4, 6, 5, 0);
	setEventMoney(2, 2, 5, 500);
	setEventMoney(-1, 8, 1, 400);
	setEventItem(2, 1, 4 ,DATAITEMKIND_CANVASOPEN); // キャンバス拡張 4/12
	setEventItem(2, 10, 3, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 4, 0, DATAITEMKIND_HEALTH);
	setEventItem(-1, 13, 0, DATAITEMKIND_TENSION);
	setProduct( 4, 60, 16, DATAITEMKIND_MONSTERMOVE, 90);
	setProduct(15, 40, 16, DATAITEMKIND_MONSTERMOVE, 65);
	createData();

	initData(6, false); setCode("02forest");
	setName("モフモフのもり2"); setBackgroundKind(2, 2);
	setSize(9, 8); setCharge(64); setOpenCost(400);
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); 
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); 
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); 
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 1); setMap(0, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setEventIcon(5, 0, 2);
	setEventIcon(5, 4, 0);
	setEventIcon(5, 7, 1);
	setEventIcon(5, 5, 3);
	setEventIcon(5, 1, 4);
	setEventIcon(4, 2, 0);
	setEventIcon(4, 3, 1);
	setEventIcon(4, 0, 4);
	setEventIcon(4, 0, 1);
	setEventIcon(4, 4, 3);
	setEventIcon(4, 3, 3);
	setEventIcon(4, 0, 7);
	setEventPrev(1, 1, 1, 4, 4, 1, 10);
	setEventNext(0, 7, 2, 9, 1, 8, 0);
	setEventNext(0, 4, 6, 7, 1, 8, 0);
	setEventMoney(2, 3, 0, 400);
	setEventMoney(2, 3, 7, 400);
	setEventMoney(2, 5, 1, 400);
	setEventItem(2, 1, 3, DATAITEMKIND_HEALTH);
	setEventItem(2, 8, 1, DATAITEMKIND_TENSION);
	setEventItem(2, 1, 5, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 3, 4, DATAITEMKIND_HEALTH);
	setProduct(12, 60, 17, DATAITEMKIND_MONSTERMOVE,  0);
	setProduct(14, 40, 17, DATAITEMKIND_MONSTERMOVE, 30);
	createData();

	initData(7, false); setCode("02forest");
	setName("モフモフのもり3"); setBackgroundKind(2, 2);
	setSize(10, 10); setCharge(66); setOpenCost(450);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 1); setMap(1, 1);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(1, 1);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(2, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(1, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(2, 1); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(1, 1); setMap(2, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0);
	setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(2, 2); setMap(1, 1); setMap(0, 0); setMap(0, 0);
	setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setEventIcon(4, 0, 9);
	setEventIcon(4, 3, 7);
	setEventIcon(4, 3, 9);
	setEventIcon(4, 5, 9);
	setEventIcon(4, 7, 5);
	setEventIcon(4, 5, 5);
	setEventIcon(4, 7, 3);
	setEventIcon(4, 7, 1);
	setEventIcon(4, 9, 0);
	setEventIcon(4, 9, 1);
	setEventIcon(4, 4, 7);
	setEventIcon(3, 7, 8);
	setEventIcon(3, 1, 9);
	setEventIcon(3, 6, 6);
	setEventIcon(3, 5, 4);
	setEventIcon(3, 8, 2);
	setEventIcon(3, 5, 7);
	setEventPrev(1, 1, 8, 6, 4, 6, 0);
	setEventNext(0, 8, 1, 8, 2, 5, 0);
	setEventMoney(2, 5, 6, 400);
	setEventMoney(2, 2, 9, 400);
	setEventMoney(-1, 5, 3, 400);
	setEventItem(2, 6, 8 ,DATAITEMKIND_CANVASOPEN); // キャンバス拡張 5/12
	setEventItem(2, 8, 0, DATAITEMKIND_HEALTH);
	setEventItem(2, 7, 4, DATAITEMKIND_TENSION);
	setEventItem(-1, 7, 6, DATAITEMKIND_TENSION);
	setEventItem(-1, 4, 9, DATAITEMKIND_CANVASFULL);
	setProduct(14, 60, 18, DATAITEMKIND_HEALTH,  0);
	setProduct(15, 40, 18, DATAITEMKIND_HEALTH, 10);
	createData();

	initData(8, false); setCode("02forest");
	setName("モフモフのもり4"); setBackgroundKind(2, 2);
	setSize(7, 7); setCharge(68); setOpenCost(500);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(0, 0); 
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 1); setMap(2, 3); setMap(1, 1); setMap(0, 0); 
	setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(2, 2); setMap(2, 2); setMap(1, 1); setMap(1, 1); 
	setMap(1, 1); setMap(2, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(2, 1); setMap(1, 1); 
	setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0); 
	setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setEventIcon(4, 0, 4);
	setEventIcon(4, 2, 2);
	setEventIcon(4, 3, 4);
	setEventIcon(4, 2, 6);
	setEventIcon(4, 6, 3);
	setEventIcon(4, 5, 1);
	setEventIcon(4, 4, 0);
	setEventIcon(3, 1, 2);
	setEventIcon(3, 2, 3);
	setEventIcon(3, 5, 4);
	setEventIcon(3, 6, 2);
	setEventIcon(3, 1, 5);
	setEventPrev(1, 2, 5, 7, 8, 1, 10);
	setEventMoney(2, 0, 3, 400);
	setEventMoney(2, 4, 4, 500);
	setEventItem(2, 4, 1, DATAITEMKIND_WORKSPACEOPEN); // ワークスペース拡張 2/3
	setEventItem(2, 5, 0, DATAITEMKIND_HEALTH);
	setEventItem(-1, 2, 4, DATAITEMKIND_TENSION);
	setProduct( 9, 60, 19, DATAITEMKIND_TENSION, 80);
	setProduct(15, 40, 19, DATAITEMKIND_TENSION, 30);
	createData();

	// むくむくやま
	// モンスター 1竜 7犬 13蛙 17翼

	initData(9, false); setCode("03craggy");
	setName("むくむくやま1"); setBackgroundKind(3, 3);
	setSize(11, 11); setCharge(80); setOpenCost(1000);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 4); setMap(0, 4);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 4); setMap(0, 4); setMap(0, 4);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 4); setMap(0, 4); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 4); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 4); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 2); setMap(0, 0); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 0);
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 2); setMap(0, 2); setMap(0, 3); setMap(0, 3); setMap(1, 3); setMap(0, 3); setMap(0, 0);
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 2); setMap(0, 2); setMap(0, 3); setMap(0, 4); setMap(0, 4); setMap(0, 3); setMap(0, 0);
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 2); setMap(0, 2); setMap(0, 4); setMap(0, 4); setMap(1, 4); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 4); setMap(0, 4); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setEventIcon(3, 3, 6);
	setEventIcon(3, 5, 9);
	setEventIcon(3, 8, 2);
	setEventIcon(3, 7, 10);
	setEventPrev(1, 1, 8, 6, 7, 2, 0);
	setEventNext(0, 7, 9, 10, 5, 8, 9);
	setEventNext(0, 9, 1, 11, 1, 2, 2);
	setEventMoney(2, 2, 9, 600);
	setEventMoney(2, 10, 0, 600);
	setEventMoney(2, 9, 5, 800);
	setEventMoney(-1, 4, 8, 600);
	setEventItem(2, 5, 5 ,DATAITEMKIND_CANVASOPEN); // キャンバス拡張 6/12
	setEventItem(2, 6, 8, DATAITEMKIND_HEALTH);
	setEventItem(2, 8, 7, DATAITEMKIND_TENSION);
	setEventItem(-1, 7, 6, DATAITEMKIND_HEALTH);
	setEventItem(-1, 9, 3, DATAITEMKIND_TENSION);
	setEventItem(-1, 9, 8, DATAITEMKIND_MONSTERMOVE);
	setProduct( 1, 60, 23, DATAITEMKIND_CANVASFULL,  0);
	setProduct( 7, 40, 23, DATAITEMKIND_CANVASFULL,  4);
	createData();

	initData(10, false); setCode("03craggy");
	setName("むくむくやま2"); setBackgroundKind(3, 3);
	setSize(10, 10); setCharge(82); setOpenCost(1250);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 3); setMap(1, 3); 
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 3); setMap(1, 3); setMap(1, 3); 
	setMap(0, 0); setMap(0, 0); setMap(1, 2); setMap(0, 2); setMap(1, 3); setMap(0, 3); setMap(1, 3); setMap(1, 3); setMap(1, 3); setMap(1, 3); 
	setMap(0, 0); setMap(0, 2); setMap(0, 2); setMap(1, 3); setMap(0, 3); setMap(1, 3); setMap(0, 3); setMap(0, 3); setMap(1, 3); setMap(1, 3); 
	setMap(0, 2); setMap(1, 2); setMap(0, 2); setMap(0, 3); setMap(1, 3); setMap(0, 3); setMap(1, 3); setMap(1, 3); setMap(0, 3); setMap(0, 0); 
	setMap(0, 2); setMap(0, 2); setMap(0, 1); setMap(1, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0); 
	setMap(0, 2); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); 
	setMap(0, 2); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(1, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setEventIcon(3, 4, 9);
	setEventIcon(3, 6, 6);
	setEventIcon(3, 5, 4);
	setEventIcon(3, 2, 3);
	setEventIcon(3, 0, 6);
	setEventPrev(1, 5, 8, 9, 7, 9, 2);
	setEventNext(0, 8, 1, 13, 1, 6, 2);
	setEventMoney(2, 8, 4, 600);
	setEventMoney(2, 0, 4, 600);
	setEventMoney(2, 5, 2, 800);
	setEventMoney(-1, 4, 4, 600);
	setEventItem(2, 2, 7, DATAITEMKIND_HEALTH);
	setEventItem(2, 7, 6, DATAITEMKIND_HEALTH);
	setEventItem(2, 9, 0, DATAITEMKIND_TENSION);
	setEventItem(-1, 3, 2, DATAITEMKIND_HEALTH);
	setEventItem(-1, 5, 5, DATAITEMKIND_TENSION);
	setEventItem(-1, 8, 2, DATAITEMKIND_TENSION);
	setEventItem(-1, 0, 7, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 6, 3, DATAITEMKIND_CANVASFULL);
	setProduct( 7, 60, 24, DATAITEMKIND_MONSTERMOVE,  0);
	setProduct(13, 40, 24, DATAITEMKIND_MONSTERMOVE, 45);
	createData();

	initData(11, false); setCode("03craggy");
	setName("むくむくやま3"); setBackgroundKind(3, 3);
	setSize(13, 7); setCharge(84); setOpenCost(1500);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 3); setMap(0, 3); setMap(0, 0); setMap(0, 3); setMap(0, 3); setMap(0, 4); setMap(0, 4); setMap(0, 0); 
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 4); setMap(0, 5); setMap(0, 5); 
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 5); setMap(0, 5); setMap(0, 5); 
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 0); setMap(0, 5); setMap(0, 6); setMap(0, 6); 
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 3); setMap(0, 3); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 6); setMap(0, 6); setMap(0, 6); 
	setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 2); setMap(0, 3); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 6); setMap(0, 6); setMap(0, 0); 
	setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 3); setMap(0, 3); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setEventIcon(3, 5, 1);
	setEventIcon(3, 3, 3);
	setEventIcon(3, 4, 6);
	setEventIcon(3, 8, 2);
	setEventIcon(3, 12, 2);
	setEventPrev(1, 1, 2, 9, 9, 1, 8);
	setEventNext(0, 11, 4, 12, 1, 6, 9);
	setEventMoney(2, 4, 1, 600);
	setEventMoney(2, 9, 0, 600);
	setEventMoney(2, 6, 3, 600);
	setEventMoney(2, 12, 4, 800);
	setEventMoney(-1, 10, 2, 600);
	setEventItem(2, 5, 6, DATAITEMKIND_HEALTH);
	setEventItem(2, 0, 4, DATAITEMKIND_TENSION);
	setEventItem(2, 12, 1, DATAITEMKIND_TENSION);
	setEventItem(2, 2, 6, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 3, 4, DATAITEMKIND_HEALTH);
	setEventItem(-1, 4, 2, DATAITEMKIND_HEALTH);
	setEventItem(-1, 7, 2, DATAITEMKIND_HEALTH);
	setEventItem(-1, 11, 0, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 8, 3, DATAITEMKIND_MONSTERMOVE);
	setProduct(13, 60, 25, DATAITEMKIND_TENSION,  0);
	setProduct(17, 40, 25, DATAITEMKIND_TENSION, 15);
	createData();

	initData(12, false); setCode("03craggy");
	setName("むくむくやま4"); setBackgroundKind(3, 3);
	setSize(6, 8); setCharge(86); setOpenCost(1750);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 2); setMap(0, 2); setMap(0, 2); 
	setMap(0, 0); setMap(0, 0); setMap(0, 2); setMap(0, 3); setMap(0, 2); setMap(0, 2); 
	setMap(0, 0); setMap(0, 2); setMap(0, 3); setMap(0, 1); setMap(0, 3); setMap(0, 2); 
	setMap(0, 0); setMap(0, 2); setMap(0, 3); setMap(0, 3); setMap(0, 2); setMap(0, 0); 
	setMap(0, 0); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 0); setMap(0, 0); 
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setEventIcon(3, 3, 0);
	setEventIcon(3, 3, 4);
	setEventIcon(3, 0, 6);
	setEventPrev(1, 1, 6, 11, 11, 4, 2);
	setEventMoney(2, 4, 3, 800);
	setEventMoney(-1, 4, 0, 800);
	setEventItem(2, 3, 2 ,DATAITEMKIND_CANVASOPEN); // キャンバス拡張 7/12
	setEventItem(-1, 2, 3, DATAITEMKIND_CANVASFULL);
	setProduct( 1, 60, 26, DATAITEMKIND_HEALTH, 40);
	setProduct(17, 40, 26, DATAITEMKIND_HEALTH, 90);
	createData();

	// モサモサかざん
	// モンスター 5魚 10象 16蟹 18翼

	initData(13, false); setCode("04volcano");
	setName("モサモサかざん1"); setBackgroundKind(4, 4);
	setSize(12, 9); setCharge(100); setOpenCost(3000);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 2); setMap(0, 2); setMap(1, 2); 
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 2); setMap(0, 2); setMap(0, 0); setMap(1, 2); setMap(1, 2); setMap(1, 2); setMap(1, 3); 
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 2); setMap(1, 2); setMap(1, 2); setMap(1, 2); setMap(1, 2); setMap(1, 2); setMap(1, 3); setMap(1, 3); 
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 2); setMap(1, 2); setMap(1, 2); setMap(1, 2); setMap(1, 2); setMap(0, 3); setMap(1, 3); setMap(0, 3); 
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 2); setMap(1, 2); setMap(0, 2); setMap(0, 0); setMap(1, 3); setMap(1, 3); setMap(1, 0); 
	setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(0, 0); setMap(1, 1); setMap(1, 2); setMap(1, 2); setMap(0, 0); setMap(1, 3); setMap(1, 3); setMap(1, 3); setMap(1, 3); 
	setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 2); setMap(0, 0); setMap(0, 0); setMap(1, 3); setMap(0, 3); setMap(1, 3); setMap(0, 3); 
	setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 3); setMap(1, 3); setMap(0, 0); 
	setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setEventPrev(1, 1, 6, 10, 8, 1, 9);
	setEventNext(0, 10, 6, 14, 1, 6, 2);
	setEventMoney(2, 1, 8, 1500);
	setEventMoney(2, 11, 2, 2000);
	setEventMoney(-1, 8, 5, 1500);
	setEventItem(2, 5, 2 ,DATAITEMKIND_CANVASOPEN); // キャンバス拡張 8/12
	setEventItem(2, 6, 5, DATAITEMKIND_HEALTH);
	setEventItem(2, 10, 7, DATAITEMKIND_TENSION);
	setEventItem(2, 9, 0, DATAITEMKIND_CANVASFULL);
	setEventItem(-1, 4, 7, DATAITEMKIND_HEALTH);
	setEventItem(-1, 9, 2, DATAITEMKIND_HEALTH);
	setEventItem(-1, 4, 5, DATAITEMKIND_TENSION);
	setEventItem(-1, 6, 3, DATAITEMKIND_TENSION);
	setProduct( 5, 60, 31, DATAITEMKIND_MONSTERMOVE,  0);
	setProduct(10, 40, 31, DATAITEMKIND_MONSTERMOVE, 60);
	createData();

	initData(14, false); setCode("04volcano");
	setName("モサモサかざん2"); setBackgroundKind(4, 4);
	setSize(12, 10); setCharge(102); setOpenCost(3500);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 6); setMap(0, 6); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 6); setMap(0, 6); setMap(1, 6); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 6); setMap(0, 6); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 4); setMap(0, 4); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 3); setMap(1, 3); setMap(0, 0); setMap(0, 0); setMap(0, 4); setMap(1, 4); setMap(0, 4); setMap(0, 0); setMap(1, 0); setMap(1, 1); setMap(1, 1);
	setMap(1, 3); setMap(1, 3); setMap(1, 3); setMap(1, 0); setMap(0, 3); setMap(1, 3); setMap(1, 3); setMap(0, 2); setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(1, 1);
	setMap(0, 3); setMap(1, 3); setMap(0, 3); setMap(1, 3); setMap(1, 3); setMap(0, 3); setMap(1, 2); setMap(1, 2); setMap(1, 1); setMap(1, 1); setMap(1, 1); setMap(0, 0);
	setMap(1, 3); setMap(1, 3); setMap(1, 3); setMap(0, 3); setMap(1, 3); setMap(1, 2); setMap(0, 2); setMap(1, 2); setMap(1, 1); setMap(1, 1); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(1, 3); setMap(1, 3); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 2); setMap(1, 2); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setEventPrev(1, 1, 6, 13, 10, 6, 1);
	setEventNext(0, 10, 6, 17, 12, 1, 6);
	setEventNext(0, 8, 1, 15, 1, 1, 4);
	setEventMoney(2, 6, 3, 1500);
	setEventMoney(2, 5, 8, 2500);
	setEventMoney(-1, 7, 9, 1500);
	setEventItem(2, 9, 6, DATAITEMKIND_HEALTH);
	setEventItem(2, 11, 6, DATAITEMKIND_TENSION);
	setEventItem(2, 0, 8, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 9, 1, DATAITEMKIND_HEALTH);
	setEventItem(-1, 4, 6, DATAITEMKIND_TENSION);
	setEventItem(-1, 2, 9, DATAITEMKIND_TENSION);
	setEventItem(-1, 7, 5, DATAITEMKIND_CANVASFULL);
	setProduct(10, 60, 32, DATAITEMKIND_HEALTH,  0);
	setProduct(16, 40, 32, DATAITEMKIND_HEALTH, 20);
	createData();

	initData(15, false); setCode("04volcano");
	setName("モサモサかざん3"); setBackgroundKind(4, 4);
	setSize(11, 11); setCharge(104); setOpenCost(4000);
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(1, 1); setMap(0, 1); setMap(0, 12); setMap(0, 12); setMap(0, 11); setMap(0, 11); setMap(1, 10); setMap(0, 10);
	setMap(1, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 12); setMap(0, 12); setMap(0, 11); setMap(1, 11); setMap(0, 10); setMap(0, 10); setMap(0, 10);
	setMap(0, 1); setMap(0, 1); setMap(1, 1); setMap(1, 13); setMap(0, 12); setMap(0, 19); setMap(0, 19); setMap(0, 18); setMap(0, 18); setMap(0, 9); setMap(1, 9);
	setMap(0, 1); setMap(0, 1); setMap(0, 13); setMap(0, 13); setMap(1, 19); setMap(0, 19); setMap(0, 18); setMap(1, 18); setMap(0, 18); setMap(0, 9); setMap(0, 9);
	setMap(0, 1); setMap(1, 2); setMap(0, 13); setMap(0, 14); setMap(0, 19); setMap(0, 20); setMap(0, 20); setMap(0, 17); setMap(0, 17); setMap(0, 8); setMap(0, 8);
	setMap(0, 2); setMap(0, 2); setMap(0, 14); setMap(0, 14); setMap(1, 20); setMap(0, 20); setMap(0, 20); setMap(0, 17); setMap(1, 17); setMap(0, 8); setMap(0, 8);
	setMap(0, 2); setMap(0, 2); setMap(1, 14); setMap(0, 14); setMap(0, 20); setMap(0, 20); setMap(0, 16); setMap(0, 17); setMap(0, 7); setMap(1, 8); setMap(0, 0);
	setMap(1, 3); setMap(0, 3); setMap(0, 15); setMap(0, 15); setMap(1, 15); setMap(0, 16); setMap(0, 16); setMap(0, 7); setMap(0, 7); setMap(0, 0); setMap(0, 0);
	setMap(0, 3); setMap(0, 3); setMap(0, 15); setMap(0, 15); setMap(0, 16); setMap(0, 16); setMap(1, 6); setMap(0, 7); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 4); setMap(1, 4); setMap(0, 4); setMap(0, 5); setMap(0, 5); setMap(0, 6); setMap(0, 6); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 4); setMap(0, 4); setMap(0, 5); setMap(1, 5); setMap(0, 6); setMap(0, 6); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setEventPrev(1, 1, 1, 14, 8, 1, 9);
	setEventNext(0, 5, 5, 16, 2, 2, 1);
	setEventMoney(2, 1, 6, 1500);
	setEventMoney(2, 9, 2, 1500);
	setEventMoney(2, 2, 5, 1800);
	setEventMoney(-1, 7, 1, 1500);
	setEventMoney(-1, 9, 0, 1500);
	setEventMoney(-1, 6, 5, 1500);
	setEventMoney(-1, 3, 10, 1800);
	setEventMoney(-1, 0, 4, 1800);
	setEventItem(-1, 7, 7 ,DATAITEMKIND_CANVASOPEN); // キャンバス拡張 9/12
	setEventItem(2, 8, 3, DATAITEMKIND_HEALTH);
	setEventItem(2, 4, 0, DATAITEMKIND_TENSION);
	setEventItem(-1, 4, 2, DATAITEMKIND_HEALTH);
	setEventItem(-1, 4, 5, DATAITEMKIND_HEALTH);
	setEventItem(-1, 6, 2, DATAITEMKIND_HEALTH);
	setEventItem(-1, 4, 8, DATAITEMKIND_HEALTH);
	setEventItem(-1, 0, 9, DATAITEMKIND_HEALTH);
	setEventItem(-1, 9, 6, DATAITEMKIND_TENSION);
	setEventItem(-1, 6, 0, DATAITEMKIND_TENSION);
	setEventItem(-1, 10, 3, DATAITEMKIND_TENSION);
	setEventItem(-1, 5, 9, DATAITEMKIND_TENSION);
	setEventItem(-1, 2, 9, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 3, 7, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 7, 6, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 2, 2, DATAITEMKIND_MONSTERMOVE);
	setProduct(16, 60, 33, DATAITEMKIND_TENSION,  0);
	setProduct(18, 40, 33, DATAITEMKIND_TENSION, 20);
	createData();

	initData(16, false); setCode("04volcano");
	setName("モサモサかざん4"); setBackgroundKind(4, 4);
	setSize(11, 11); setCharge(106); setOpenCost(4500);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(1, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); 
	setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(1, 1); setMap(0, 1); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 1); 
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 1); 
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 2); setMap(0, 2); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 2); setMap(0, 2); setMap(0, 1); 
	setMap(0, 0); setMap(0, 1); setMap(0, 2); setMap(0, 2); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 2); setMap(0, 2); setMap(0, 1); 
	setMap(1, 1); setMap(0, 2); setMap(0, 2); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 2); setMap(0, 2); setMap(0, 1); 
	setMap(0, 1); setMap(0, 2); setMap(0, 2); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 2); setMap(0, 2); setMap(0, 1); setMap(0, 0); 
	setMap(0, 1); setMap(0, 2); setMap(0, 2); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 2); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); 
	setMap(0, 1); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 1); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); 
	setEventPrev(1, 2, 2, 15, 5, 5, 2);
	setEventMoney(2, 4, 8, 1500);
	setEventMoney(2, 5, 10, 1800);
	setEventMoney(-1, 8, 5, 1500);
	setEventMoney(-1, 8, 2, 1800);
	setEventItem(2, 7, 7 ,DATAITEMKIND_CANVASOPEN); // キャンバス拡張 10/12
	setEventItem(2, 10, 0, DATAITEMKIND_HEALTH);
	setEventItem(2, 10, 2, DATAITEMKIND_TENSION);
	setEventItem(2, 1, 5, DATAITEMKIND_CANVASFULL);
	setEventItem(-1, 2, 7, DATAITEMKIND_HEALTH);
	setEventItem(-1, 10, 1, DATAITEMKIND_HEALTH);
	setEventItem(-1, 10, 5, DATAITEMKIND_HEALTH);
	setEventItem(-1, 5, 2, DATAITEMKIND_TENSION);
	setEventItem(-1, 1, 10, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 5, 0, DATAITEMKIND_MONSTERMOVE);
	setProduct( 5, 60, 34, DATAITEMKIND_CANVASFULL, 50);
	setProduct(18, 40, 34, DATAITEMKIND_CANVASFULL, 25);
	createData();

	// ふさふさせつげん
	// モンスター 2竜 11象 19宙

	initData(17, false); setCode("05snow");
	setName("ふさふさせつげん1"); setBackgroundKind(5, 5);
	setSize(14, 9); setCharge(120); setOpenCost(10000);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 5);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 5);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 4); setMap(0, 4); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 5); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 4); setMap(0, 4); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 4); setMap(0, 4); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 4); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 2); setMap(0, 2); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 2); setMap(0, 2); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 2); setMap(0, 2); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 2); setMap(0, 2); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setEventIcon(5, 10, 2);
	setEventIcon(5, 8, 0);
	setEventIcon(5, 7, 3);
	setEventIcon(5, 7, 5);
	setEventIcon(5, 3, 5);
	setEventIcon(5, 3, 8);
	setEventIcon(4, 11, 0);
	setEventIcon(4, 4, 3);
	setEventIcon(4, 6, 8);
	setEventIcon(3, 6, 4);
	setEventPrev(1, 12, 1, 14, 10, 6, 10);
	setEventNext(0, 1, 7, 18, 2, 12, 0);
	setEventMoney(2, 7, 2, 5000);
	setEventMoney(2, 5, 8, 8000);
	setEventMoney(-1, 4, 4, 5000);
	setEventMoney(-1, 5, 5, 5000);
	setEventMoney(-1, 8, 6, 8000);
	setEventItem(2, 7, 4 ,DATAITEMKIND_CANVASOPEN); // キャンバス拡張 11/12
	setEventItem(2, 11, 3, DATAITEMKIND_HEALTH);
	setEventItem(2, 1, 8, DATAITEMKIND_TENSION);
	setEventItem(-1, 10, 1, DATAITEMKIND_HEALTH);
	setEventItem(-1, 9, 3, DATAITEMKIND_HEALTH);
	setEventItem(-1, 7, 0, DATAITEMKIND_TENSION);
	setEventItem(-1, 4, 6, DATAITEMKIND_CANVASFULL);
	setProduct( 2, 60, 40, DATAITEMKIND_CANVASFULL,  0);
	setProduct(11, 40, 40, DATAITEMKIND_CANVASFULL, 10);
	createData();

	initData(18, false); setCode("05snow");
	setName("ふさふさせつげん2"); setBackgroundKind(5, 5);
	setSize(16, 16); setCharge(122); setOpenCost(11000);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setEventIcon(5, 9, 1);
	setEventIcon(5, 11, 4);
	setEventIcon(5, 8, 4);
	setEventIcon(5, 10, 7);
	setEventIcon(5, 6, 7);
	setEventIcon(5, 7, 10);
	setEventIcon(5, 4, 12);
	setEventIcon(5, 9, 12);
	setEventIcon(5, 14, 1);
	setEventIcon(4, 6, 3);
	setEventIcon(4, 5, 6);
	setEventIcon(4, 2, 13);
	setEventIcon(4, 9, 9);
	setEventIcon(4, 6, 13);
	setEventIcon(4, 12, 8);
	setEventIcon(4, 12, 1);
	setEventIcon(3, 8, 7);
	setEventPrev(1, 2, 12, 17, 1, 7, 6);
	setEventNext(0, 7, 2, 19, 1, 1, 1);
	setEventMoney(2, 8, 2, 5000);
	setEventMoney(2, 8, 13, 8000);
	setEventMoney(-1, 6, 5, 5000);
	setEventMoney(-1, 8, 10, 5000);
	setEventMoney(-1, 13, 1, 8000);
	setEventItem(2, 10, 3, DATAITEMKIND_HEALTH);
	setEventItem(2, 7, 8, DATAITEMKIND_TENSION);
	setEventItem(2, 3, 13, DATAITEMKIND_MONSTERMOVE);
	setEventItem(2, 11, 7, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 6, 12, DATAITEMKIND_HEALTH);
	setEventItem(-1, 9, 5, DATAITEMKIND_TENSION);
	setEventItem(-1, 12, 5, DATAITEMKIND_MONSTERMOVE);
	setEventItem(-1, 5, 9, DATAITEMKIND_CANVASFULL);
	setProduct(11, 60, 41, DATAITEMKIND_TENSION,  0);
	setProduct(19, 40, 41, DATAITEMKIND_TENSION, 25);
	createData();

	initData(19, false); setCode("06cake");
	setName("スイートヘブン"); setBackgroundKind(6, 6);
	setSize(7, 7); setCharge(124); setOpenCost(12000);
	setMap(0, 0); setMap(2, 1); setMap(2, 1); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0);
	setMap(2, 1); setMap(2, 1); setMap(2, 1); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(0, 0);
	setMap(2, 1); setMap(2, 1); setMap(0, 2); setMap(0, 3); setMap(0, 3); setMap(0, 2); setMap(0, 0);
	setMap(0, 0); setMap(0, 2); setMap(0, 3); setMap(0, 3); setMap(0, 3); setMap(0, 2); setMap(0, 0);
	setMap(0, 0); setMap(0, 2); setMap(0, 3); setMap(0, 3); setMap(0, 2); setMap(1, 1); setMap(1, 1);
	setMap(0, 0); setMap(0, 2); setMap(0, 2); setMap(0, 2); setMap(1, 1); setMap(1, 1); setMap(1, 1);
	setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(0, 0); setMap(1, 1); setMap(1, 1); setMap(0, 0);
	setEventIcon(3, 2, 1);
	setEventIcon(3, 1, 0);
	setEventIcon(3, 0, 2);
	setEventIcon(3, 5, 1);
	setEventIcon(3, 3, 1);
	setEventIcon(3, 1, 3);
	setEventIcon(3, 1, 5);
	setEventIcon(3, 5, 3);
	setEventIcon(3, 3, 5);
	setEventIcon(3, 4, 5);
	setEventIcon(3, 6, 4);
	setEventIcon(3, 5, 6);
	setEventPrev(1, 1, 1, 18, 7, 2, 6);
	setEventMoney(2, 4, 1, 8000);
	setEventMoney(-1, 1, 4, 10000);
	setEventItem(2, 3, 3 ,DATAITEMKIND_WORKSPACEOPEN); // ワークスペース拡張 3/3
	setEventItem(2, 5, 5 ,DATAITEMKIND_CANVASOPEN); // キャンバス拡張 12/12
	setEventItem(2, 2, 5, DATAITEMKIND_HEALTH);
	setEventItem(-1, 5, 2, DATAITEMKIND_CANVASFULL);
	setProduct( 2, 60, 42, DATAITEMKIND_HEALTH,  0);
	setProduct(19, 40, 42, DATAITEMKIND_HEALTH, 25);
	createData();

	return 0;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

