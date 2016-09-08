#include "../library.h"
#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static int experienceNeed[UTILPARAMETER_LEVELMAX];

// ----------------------------------------------------------------

// パラメータ初期化
void parameterInit(){
	// 必要経験点
	experienceNeed[0] = 0;
	for(int i = 1; i < UTILPARAMETER_LEVELMAX; i++){
		experienceNeed[i] = (int)(engineMathPow(3, i * 0.1) * 100 - 100) * 3;
	}
}

// ----------------------------------------------------------------

// プレイヤーレベル
int parameterPlayerLevel(){
	int exp = storageStatusGet()->experience;
	for(int i = 1; i < UTILPARAMETER_LEVELMAX; i++){
		if(exp < experienceNeed[i]){return i;}
	}
	return UTILPARAMETER_LEVELMAX;
}

// レベルアップに必要な経験値
int parameterPlayerExperience(){
	int exp = storageStatusGet()->experience;
	for(int i = 1; i < UTILPARAMETER_LEVELMAX; i++){
		if(exp < experienceNeed[i]){return experienceNeed[i] - exp;}
	}
	return 0;
}

// プレイヤー体力の最大値
int parameterPlayerHealthMax(){
	int level = parameterPlayerLevel();
	return level * (8 * level + 50) + 242;
}

// プレイヤーテンションの最大値
int parameterPlayerTensionMax(){
	return 30;
}

// プレイヤーの所持パペット数
int parameterPlayerPuppetNum(){
	int puppetNum = 0;
	for(int i = 0; i < STORAGEPUPPET0001_KIND; i++){
		puppetNum += storagePuppetGet()->list[i].num;
	}
	return puppetNum;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

