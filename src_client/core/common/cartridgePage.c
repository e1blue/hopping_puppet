#include "../library.h"
#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ページカートリッジリスト
static struct pageCartridge *pageCartridgeList = NULL;

// ページカートリッジ装填
void pageCartridgePush(struct pageCartridge *cartridge){
	if(pageCartridgeList == NULL){
		pageCartridgeList = cartridge;
		pageCartridgeList->init(pageCartridgeList);
	}else{
		struct pageCartridge *temp = pageCartridgeList;
		while(temp->next != NULL){temp = temp->next;}
		temp->next = cartridge;
	}
	cartridge->next = NULL;
}

// ページカートリッジ初期化
void pageCartridgeGameInit(){
	// 最初のページカートリッジ装填
	if(pageCartridgeList == NULL){pageCartridgePush(FIRSTCARTRIDGE());}
}

// ページカートリッジメインループ計算
void pageCartridgeGameCalc(){
	while(pageCartridgeList != NULL && pageCartridgeList->calc(pageCartridgeList)){
		struct pageCartridge *next = pageCartridgeList->next;

		pageCartridgeList->pause(pageCartridgeList);
		pageCartridgeList->dispose(pageCartridgeList);

		pageCartridgeList = next;
		if(pageCartridgeList != NULL){pageCartridgeList->init(pageCartridgeList);}
	};

	// ページカートリッジ描画
	if(pageCartridgeList != NULL){pageCartridgeList->draw(pageCartridgeList);}
}

// ページカートリッジ一時停止処理
void pageCartridgeGamePause(){
	if(pageCartridgeList != NULL){pageCartridgeList->pause(pageCartridgeList);}
}

// ページカートリッジ終了処理
void pageCartridgeGameFinal(){
	if(pageCartridgeList != NULL){pageCartridgeList->dispose(pageCartridgeList);}
	pageCartridgeList = NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

