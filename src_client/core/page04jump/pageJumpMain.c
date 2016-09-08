#include "../library.h"
#include "../includeAll.h"
#include "pageJump.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#define PAGEJUMPMAIN_TENSIONNUM 6
#define PAGEJUMPMAIN_PUPPETNUM 16
#define PAGEJUMPMAIN_EFFECTNUM 16
#define PAGEJUMPMAIN_BOMBNUM 32

enum pageJumpMainWeaponActionType{
	PAGRJUMPMAINWEAPONACTIONTYPE_NONE = 0,
	PAGRJUMPMAINWEAPONACTIONTYPE_ATTACK,
	PAGRJUMPMAINWEAPONACTIONTYPE_FALL
};

enum pageJumpMainPuppetActionType{
	PAGRJUMPMAINPUPPETACTIONTYPE_NONE = 0,
	PAGRJUMPMAINPUPPETACTIONTYPE_WALK,
	PAGRJUMPMAINPUPPETACTIONTYPE_GRAB,
	PAGRJUMPMAINPUPPETACTIONTYPE_KEEP,
	PAGRJUMPMAINPUPPETACTIONTYPE_JUMP,
	PAGRJUMPMAINPUPPETACTIONTYPE_FALL
};

enum pageJumpMainEffectType{
	PAGRJUMPMAINEFFECTTYPE_NONE = 0,
	PAGRJUMPMAINEFFECTTYPE_DAMAGE,
	PAGRJUMPMAINEFFECTTYPE_BOMB
};

// ページカートリッジ構造体
struct pageCartridgeJumpMain{
	struct pageCartridge super;
	struct pageJumpStatus *stat;
	struct popupCartridge *popupEscape;
	struct popupCartridge *popupResult;
	struct popupCartridge *popupTutorial;
	int stepTutorial01;
	int stepTutorial02;
	int stepTutorial11;
	int stepTutorial12;

	struct partsButtonBox btnBack;

	struct e3dImage fieldFoothold;
	struct e3dImage enemyBody[2];
	struct e3dImage enemyWeapon[4];
	struct e3dImage enemyWindow1;
	struct e3dImage enemyWindow2;
	struct e3dImage enemyIcon[2];
	struct e3dImage enemyName;
	struct e3dImage enemyGauge;
	struct e3dImage enemyHitArea;
	struct e3dImage playerWindow1;
	struct e3dImage playerWindow2;
	struct e3dImage playerWindow3;
	struct e3dImage playerWindow4;
	struct e3dImage playerIcon[2];
	struct e3dImage playerTension;
	struct e3dImage playerName;
	struct e3dImage playerNum;
	struct e3dImage playerGauge1;
	struct e3dImage playerGauge2;
	struct e3dNumber countNumPuppet1;
	struct e3dNumber countNumPuppet2;
	struct e3dNumber effectDamage;
	struct e3dImage effectBomb[PAGEJUMPMAIN_BOMBNUM];
	struct e3dImage tutorialArrow1;
	struct e3dImage tutorialArrow2;

	int bufferStatus;
	int bufferScreen;
	int e3dIdVert;
	int e3dIdTexc;
	int e3dIdFace;

	struct{
		struct e3dImage labelName;
		struct e3dImage labelData;
		int bufferStatus;
		int bufferScreen;
		int e3dIdVert;
		int e3dIdTexc;
		int e3dIdFace;
	} puppetBuffer[STORAGEPUPPET0001_KIND];

	// エネミーデータ
	struct{
		int fieldKind;
		int enemyIndex;
		double healthMainBase;
		double healthSubBase;
		double attackBase;
		double moneyBase;
		double experienceBase;
		double x;
		double y;
		double vx;
		double vy;
		double healthPrev;
		bool isGiveup;
		int step;
		int stepShake;
		double stepShakePowerx;
		double stepShakePowery;
	} enemyData;

	// エネミー攻撃データ
	struct{
		int weaponIndex;
		enum pageJumpMainWeaponActionType type;
		double x;
		double y;
		double vx;
		double vy;
		int health;
		int step;
		bool attacked;
	} weaponList[DATAENEMY_WEAPONNUM];

	// プレイヤーデータ
	struct{
		int puppetNum;
		int healthMax;
		double healthPrev;
		int tension;
		struct{int x; int y;} tensionEffect[PAGEJUMPMAIN_TENSIONNUM];
		bool isGiveup;
		int step;
		int stepHitArea;
		int stepShake;
		double stepShakePowerx;
		double stepShakePowery;
		int grabbedPuppetKind;
	} playerData;

	// パペットデータ
	struct{
		int puppetKind;
		enum pageJumpMainPuppetActionType type;
		double x;
		double y;
		double vx;
		double vy;
		double rot;
		double rotNext;
		int step;
	} puppetList[PAGEJUMPMAIN_PUPPETNUM];

	// エフェクトデータ
	struct{
		enum pageJumpMainEffectType type;
		enum dataAttributionKind attributionKind;
		double x;
		double y;
		double vx;
		double vy;
		int power;
		int step;
	} effectList[PAGEJUMPMAIN_EFFECTNUM];
};

// ----------------------------------------------------------------

// 初期化
static void init(struct pageCartridgeJumpMain *this){
	// ボタン作成
	partsButtonBoxInit(&this->btnBack);

	// エネミー初期化
	this->enemyData.fieldKind = storageStatusGet()->fieldKind;
	this->enemyData.enemyIndex = storageStatusGet()->enemyIndex;
	struct storageFieldList *storageField = &storageFieldGet()->list[this->enemyData.fieldKind];
	struct storageFieldEnemy *storageEnemy = &storageField->enemy[this->enemyData.enemyIndex];
	this->enemyData.healthMainBase = 100 * engineMathPow(4, (double)(storageEnemy->level - 1) / 8);
	this->enemyData.healthSubBase = 5 * engineMathPow(4, (double)(storageEnemy->level - 1) / 8);
	this->enemyData.attackBase = 75 * engineMathPow(2, (double)(storageEnemy->level - 1) / 8);
	this->enemyData.moneyBase = 2 * engineMathPow(5, (double)(storageEnemy->level - 1) / 8);
	this->enemyData.experienceBase = 5 * engineMathPow(2, (double)(storageEnemy->level - 1) / 8);
	this->enemyData.x = 0;
	this->enemyData.y = -320;
	this->enemyData.healthPrev = storageEnemy->health;

	// プレイヤー初期化
	this->playerData.healthMax = parameterPlayerHealthMax();
	this->playerData.healthPrev = storageStatusGet()->health;
	this->playerData.tension = 0;
	this->playerData.grabbedPuppetKind = -1;
	if(storageStatusGet()->isTension){
		// テンションアイテムによるテンション上昇
		this->playerData.tension = parameterPlayerTensionMax();
		storageStatusGet()->isTension = false;
		storageStatusUpdate();
	}
	for(int i = 0; i < PAGEJUMPMAIN_TENSIONNUM; i++){
		this->playerData.tensionEffect[i].x = engineMathRandomGet() % 44;
		this->playerData.tensionEffect[i].y = engineMathRandomGet() % 44;
	}

	// パペット初期化
	for(int i = 0; i < PAGEJUMPMAIN_PUPPETNUM; i++){
		this->puppetList[i].step = -30 * i;
	}

}

// ----------------------------------------------------------------

// エフェクトデータ作成
static int createEffect(struct pageCartridgeJumpMain *this, enum pageJumpMainEffectType type, enum dataAttributionKind attributionKind, double x, double y, double vx, double vy, int power){
	int index = -1; for(int j = 0; j < PAGEJUMPMAIN_EFFECTNUM; j++){if(this->effectList[j].type == PAGRJUMPMAINEFFECTTYPE_NONE){index = j; break;}}
	if(index >= 0){
		this->effectList[index].type = type;
		this->effectList[index].attributionKind = attributionKind;
		this->effectList[index].x = x;
		this->effectList[index].y = y;
		this->effectList[index].vx = vx;
		this->effectList[index].vy = vy;
		this->effectList[index].power = power;
		this->effectList[index].step = 0;
	}
	return index;
}

// 掌握の確認
static void calcGrab(struct pageCartridgeJumpMain *this){
	static struct{
		int index;
		int x;
		int y;
	}grabbedInfo[2];
	// タッチ状態の確認
	for(int i = 0; i < 2; i++){
		struct engineCtrlTouch *t = engineCtrlTouchGet((i == 0) ? ENGINECTRLTOUCHID_TEST1 : ENGINECTRLTOUCHID_TEST2);
		if(t != NULL){
			if(t->dn){
				if(!t->active){
					// 掌握確認
					int touchIndex = -1;
					double touchRadiusSquare = 60 * 60;
					for(int j = 0; j < PAGEJUMPMAIN_PUPPETNUM; j++){
						if(this->puppetList[j].type != PAGRJUMPMAINPUPPETACTIONTYPE_WALK){continue;}
						if(this->puppetList[j].y > -80){continue;}
						int x = (global.screen.w * 0.5 + (this->puppetList[j].x)) - t->screen.x;
						int y = (global.screen.h + (this->puppetList[j].y - 16)) - t->screen.y;
						double xxyy = x * x + y * y;
						if(xxyy < touchRadiusSquare){
							touchIndex = j;
							touchRadiusSquare = xxyy;
						}
					}
					if(touchIndex >= 0){
						// 掌握開始
						this->playerData.grabbedPuppetKind = this->puppetList[touchIndex].puppetKind;
						this->puppetList[touchIndex].type = PAGRJUMPMAINPUPPETACTIONTYPE_GRAB;
						this->puppetList[touchIndex].vx = 0.0;
						this->puppetList[touchIndex].vy = 0.0;
						this->puppetList[touchIndex].step = 0;
						grabbedInfo[i].index = touchIndex;
						grabbedInfo[i].x = t->screen.x;
						grabbedInfo[i].y = t->screen.y;
						// タッチ開始時
						engineCtrlTouchOwn();
					}
				}
			}else{
				int index = grabbedInfo[i].index;
				int puppetKind = this->puppetList[index].puppetKind;
				int x = grabbedInfo[i].x - t->screen.x;
				int y = grabbedInfo[i].y - t->screen.y;
				struct dataPuppet *dataPuppet = dataPuppetGet(puppetKind, NULL);
				if(dataPuppet != NULL && this->puppetList[index].step <= 20 && x * x + y * y > 30 * 30){
					if(y > 0){
						// ストレージからパペット減少
						int num = storagePuppetGet()->list[puppetKind].num - 1;
						if(num < 0){num = 0;}
						storagePuppetGet()->list[puppetKind].num = num;
						storagePuppetUpdate();
						// 投擲開始
						soundSePlayJump();
						double powerX = (double)x / (global.screen.w * 0.50); if(powerX > 1.0){powerX = 1.0;}else if(powerX < -1.0){powerX = -1.0;}
						double powerY = (double)y / (global.screen.h * 0.25); if(powerY > 1.0){powerY = 1.0;} powerY = (1 + powerY) / 2;
						double powerTime = 1 - this->puppetList[index].step * 0.05; powerTime = (1 + powerTime) / 2;
						double powerWeight = 1 - 0.2 * (dataPuppet->weight * 0.01);
						this->puppetList[index].type = PAGRJUMPMAINPUPPETACTIONTYPE_JUMP;
						this->puppetList[index].vx =  -5.0 * powerWeight * powerTime * powerX;
						this->puppetList[index].vy = -27.0 * powerWeight * powerTime * powerY;
						// テンション上昇
						int tension = this->playerData.tension + 1;
						int tensionMax = parameterPlayerTensionMax();
						if(tension > tensionMax){tension = tensionMax;}
						this->playerData.tension = tension;
					}else{
						// 保持開始
						this->puppetList[index].type = PAGRJUMPMAINPUPPETACTIONTYPE_KEEP;
						this->puppetList[index].vx = 0.0;
						this->puppetList[index].vy = 1.0;
					}
					this->puppetList[index].step = 0;
				}else{
					// 掌握解除
					this->puppetList[index].type = PAGRJUMPMAINPUPPETACTIONTYPE_WALK;
					this->puppetList[index].step = 0;
				}
				// タッチ終了時
				engineCtrlTouchFree();
			}
		}
	}
}

// 時間経過計算
static void calcTimeStep(struct pageCartridgeJumpMain *this, struct dataField *dataField, struct storageFieldList *storageField){
	long long int time = (long long int)platformPluginUtilTimeGet();

	// 戦っているときは時間経過による体力の回復を行わない
	int timeDiff = (int)(time - storageStatusGet()->time);
	if(timeDiff > 0){
		storageStatusGet()->time = time;
		storageStatusUpdate();
	}

	// 戦っているときは時間経過によるエネミーの出現カウントに限界を設ける
	timeDiff = (int)(time - storageField->time);
	if(timeDiff > 0){
		if(dataField != NULL){
			storageField->charge += timeDiff;
			if(storageField->charge > dataField->charge - 5){storageField->charge = dataField->charge - 5;}
		}
		storageField->time = time;
		storageFieldUpdate();
	}
}

// 計算
static int calc(struct pageCartridgeJumpMain *this){
	// メモリートレースボタン計算
	partsButtonMemoryCalc();
	
	struct dataField *dataField = dataFieldGet(this->enemyData.fieldKind, NULL);
	struct storageFieldList *storageField = &storageFieldGet()->list[this->enemyData.fieldKind];
	struct storageFieldEnemy *storageEnemy = &storageField->enemy[this->enemyData.enemyIndex];
	struct dataEnemy *dataEnemy = dataEnemyGet(storageEnemy->enemyKind, NULL);
	calcTimeStep(this, dataField, storageField);

	// チュートリアルステップカウント
	if(!storageStatusGet()->tutorial.jump03){
		this->stepTutorial01++;
		if(this->playerData.puppetNum <= 0 && !this->enemyData.isGiveup){this->stepTutorial02++;}
		if(storageStatusGet()->tutorial.jump01 && this->stepTutorial01 > 150 && this->playerData.puppetNum > 0 && !this->enemyData.isGiveup){this->stepTutorial11++;}else{this->stepTutorial11 = 0;}
		if(storageStatusGet()->tutorial.jump02 && this->stepTutorial02 > 40){this->stepTutorial12++;}else{this->stepTutorial12 = 0;}
	}

	if(this->popupEscape != NULL){
		// 逃走ポップアップ計算
		if(!this->popupEscape->calc(this->popupEscape)){return 0;}
		// ポップアップ完了時処理
		if(this->popupEscape->result(this->popupEscape)){
			// ページ遷移
			pageCartridgePush(pageFieldInit());
			pageJumpStatusDispose(this->stat);
			return 1;
		}
		this->popupEscape->dispose(this->popupEscape);
		this->popupEscape = NULL;
	}

	if(this->popupResult != NULL){
		// 結果ポップアップ計算
		if(!this->popupResult->calc(this->popupResult)){return 0;}
		// ポップアップ完了時処理
		if(this->popupResult->result(this->popupResult)){}
		this->popupResult->dispose(this->popupResult);
		this->popupResult = NULL;
		// ページ遷移
		pageCartridgePush(pageFieldInit());
		pageJumpStatusDispose(this->stat);
		return 1;
	}

	if(this->popupTutorial != NULL){
		// チュートリアルポップアップ計算
		if(!this->popupTutorial->calc(this->popupTutorial)){return 0;}
		// チュートリアルポップアップ完了時処理
		if(this->popupTutorial->result(this->popupTutorial)){}
		this->popupTutorial->dispose(this->popupTutorial);
		this->popupTutorial = NULL;
	}else if(!storageStatusGet()->tutorial.jump01){
		// チュートリアル計算
		if(this->stepTutorial01 == 180){
			storageStatusGet()->tutorial.jump01 = true;
			storageStatusUpdate();
			soundSePlayOK(); 
			this->popupTutorial = partsPopupTutorialJump01();
			return 0;
		}
	}else if(!storageStatusGet()->tutorial.jump02){
		// チュートリアル計算
		if(this->stepTutorial02 == 60){
			storageStatusGet()->tutorial.jump02 = true;
			storageStatusUpdate();
			soundSePlayOK(); 
			this->popupTutorial = partsPopupTutorialJump02();
			return 0;
		}
	}

	if(!storageStatusGet()->tutorial.jump01){
		// チュートリアル中の戦闘開始直後はタッチを受け付けない
	}else{
		// タッチ処理
		partsButtonBoxCalc(&this->btnBack);
		calcGrab(this);
	}

	// エネミー計算
	if(dataEnemy != NULL){
		// エネミー体力ゲージオーバーの確認
		unsigned int healthMax = this->enemyData.healthMainBase * dataEnemy->healthRatio100 * 0.01;
		if(storageEnemy->health > healthMax){storageEnemy->health = healthMax; storageEnemy->healthPercent = 100; storageFieldUpdate();}
		if(this->enemyData.healthPrev > healthMax){this->enemyData.healthPrev = healthMax;}
		// エネミー体力ゲージ減衰
		this->enemyData.healthPrev -= (this->enemyData.healthPrev - storageEnemy->health) * 0.1;
		if(storageEnemy->health <= 0 && !this->enemyData.isGiveup){
			this->enemyData.isGiveup = true;
			this->enemyData.step = 0;
			// 戦闘結果
			this->stat->resultData.enemyKind = storageEnemy->enemyKind;
			this->stat->resultData.experience = this->enemyData.experienceBase * dataEnemy->experienceRatio100 * 0.01;
			this->stat->resultData.money = this->enemyData.moneyBase * dataEnemy->moneyRatio100 * 0.01;
			this->stat->resultData.itemKind = -1;
			this->stat->resultData.prevLevel = parameterPlayerLevel();
			this->stat->resultData.prevExperience = parameterPlayerExperience();
			this->stat->resultData.prevMoney = storageStatusGet()->money;
			int experience = storageStatusGet()->experience + this->stat->resultData.experience;
			int money = storageStatusGet()->money + this->stat->resultData.money;
			if(experience > UTILPARAMETER_EXPERIENCEMAX){experience = UTILPARAMETER_EXPERIENCEMAX;}
			if(money > UTILPARAMETER_MONEYMAX){money = UTILPARAMETER_MONEYMAX;}
			storageStatusGet()->experience = experience;
			storageStatusGet()->money = money;
			this->stat->resultData.nextLevel = parameterPlayerLevel();
			this->stat->resultData.nextExperience = parameterPlayerExperience();
			this->stat->resultData.nextMoney = storageStatusGet()->money;
			if(storageEnemy->isItemGet){
				int itemKind = this->stat->resultData.itemKind = storageEnemy->itemKind;
				int itemNum = storageStatusGet()->itemNum[itemKind] + 1;
				if(itemNum > UTILPARAMETER_ITEMNUMMAX){itemNum = UTILPARAMETER_ITEMNUMMAX;}
				storageStatusGet()->itemNum[itemKind] = itemNum;
			}
			storageStatusUpdate();
		}
		// エネミー振動の減衰
		if(this->enemyData.stepShake < 0){this->enemyData.stepShake++;}

		if(this->enemyData.isGiveup){
			// エネミーやられたとき
			if(this->enemyData.step < 120){
				this->enemyData.vx = 0.0;
				this->enemyData.vy = 0.0;
				// 爆発エフェクト
				this->enemyData.stepShake = 5;
				if((this->enemyData.step % 10) == 0){
					double r = 100 * ((double)(engineMathRandomGet() % 101) / 100);
					double t = 3.1415 * 2 * ((double)(engineMathRandomGet() % 101) / 100);
					int x = this->enemyData.x + r * engineMathCos(t);
					int y = this->enemyData.y + r * engineMathSin(t);
					soundSePlayBomb();
					createEffect(this, PAGRJUMPMAINEFFECTTYPE_BOMB, DATAATTRIBUTIONKIND_WHITE, x, y, 0.0, 0.0, 50);
				}
			}else if(this->enemyData.step == 120){
				soundSePlayEnemyDown();
				this->enemyData.vy = -10.0; 
				this->enemyData.stepShake = 0;
			}else if(this->enemyData.step < 180){
				this->enemyData.vy += 1;
			}else{
				// BGMをちいさくする
				soundBgmToneDown();
				// ポップアップ展開
				this->popupResult = popupJumpResultWin(this->stat);
				return 0;
			}
		}else{
			// エネミー位置移動
			switch(dataEnemy->motionType){
				case DATAENEMYMOTIONTYPE_STOP:
					break;
				case DATAENEMYMOTIONTYPE_HORIZON:
					this->enemyData.vx = 1.5 * engineMathCos(3.1415 * this->enemyData.step * 0.01);
					this->enemyData.vy = 0;
					break;
				case DATAENEMYMOTIONTYPE_VERTICAL:
					this->enemyData.vx = 0;
					this->enemyData.vy = 1.5 * engineMathCos(3.1415 * this->enemyData.step * 0.01);
					break;
				case DATAENEMYMOTIONTYPE_SLASH:
					this->enemyData.vx = 1.5 * engineMathCos(3.1415 * this->enemyData.step * 0.01);
					this->enemyData.vy = 1.5 * engineMathCos(3.1415 * this->enemyData.step * 0.01);
					break;
				case DATAENEMYMOTIONTYPE_INFINIT:
					this->enemyData.vx = 1.5 * engineMathCos(3.1415 * this->enemyData.step * 0.01);
					this->enemyData.vy = 1.5 * engineMathCos(3.1415 * this->enemyData.step * 0.02);
					break;
				case DATAENEMYMOTIONTYPE_WARP:
					this->enemyData.vx = 0;
					this->enemyData.vy = 0.9 * engineMathCos(3.1415 * this->enemyData.step * 0.03);
					if(this->enemyData.step % 15 == 0 && engineMathRandomGet() % 3 == 0){
						this->enemyData.x = 0 + ((signed int)engineMathRandomGet() % 61) - 30;
						this->enemyData.y = -320 + ((signed int)engineMathRandomGet() % 61) - 30;
					}
					break;
			}
			// エネミー攻撃作成
			if(this->enemyData.step % (dataEnemy->charge * 60) == 0){
				int index = engineMathRandomGet() % DATAENEMY_WEAPONNUM;
				for(int i = 0; i < DATAENEMY_WEAPONNUM; i++){
					if(this->weaponList[index].type == PAGRJUMPMAINWEAPONACTIONTYPE_NONE){
						this->weaponList[index].weaponIndex = index;
						this->weaponList[index].type = PAGRJUMPMAINWEAPONACTIONTYPE_ATTACK;
						this->weaponList[index].x = (int)(engineMathRandomGet() % 301) - 150;
						this->weaponList[index].y = -480 - 50;
						this->weaponList[index].vx = 0.0;
						switch(dataEnemy->weapon[this->weaponList[index].weaponIndex].motionType){
							case DATAENEMYWEAPONMOTIONTYPE_DEFAULT: this->weaponList[index].vy = 0.8; break;
							case DATAENEMYWEAPONMOTIONTYPE_SPEED: this->weaponList[index].vy = 2.5; break;
							case DATAENEMYWEAPONMOTIONTYPE_MEANDERING: this->weaponList[index].vy = 0.8; break;
							case DATAENEMYWEAPONMOTIONTYPE_SHIELD1: this->weaponList[index].vy = 0.0; break;
							case DATAENEMYWEAPONMOTIONTYPE_SHIELD2: this->weaponList[index].vy = 0.0; break;
						}
						this->weaponList[index].health = this->enemyData.healthSubBase * dataEnemy->weapon[this->weaponList[index].weaponIndex].healthRatio10 * 0.1;
						this->weaponList[index].step = 0;
						this->weaponList[index].attacked = false;
						break;
					}
					index = (index + 1) % DATAENEMY_WEAPONNUM;
				}
			}
		}

		// 位置計算
		this->enemyData.x += this->enemyData.vx;
		this->enemyData.y += this->enemyData.vy;
		this->enemyData.step++;

		// エネミー攻撃計算
		for(int i = 0; i < DATAENEMY_WEAPONNUM; i++){
			if(this->weaponList[i].type == PAGRJUMPMAINWEAPONACTIONTYPE_NONE){
			}else if(this->weaponList[i].type == PAGRJUMPMAINWEAPONACTIONTYPE_ATTACK){
				// 攻撃状態
				if(this->enemyData.isGiveup){
					// エネミー本体がやられたらエネミー攻撃が消える
					this->weaponList[i].type = PAGRJUMPMAINWEAPONACTIONTYPE_FALL;
					this->weaponList[i].vx = 0.0;
					this->weaponList[i].vy = -10.0;
				}else{
					// エネミー攻撃位置移動
					switch(dataEnemy->weapon[this->weaponList[i].weaponIndex].motionType){
						case DATAENEMYWEAPONMOTIONTYPE_DEFAULT: break;
						case DATAENEMYWEAPONMOTIONTYPE_SPEED: break;
						case DATAENEMYWEAPONMOTIONTYPE_MEANDERING: this->weaponList[i].vx = 2.0 * engineMathCos(3.1415 * this->weaponList[i].step * 0.01); break;
						case DATAENEMYWEAPONMOTIONTYPE_SHIELD1:
							this->weaponList[i].x -= 0.1 * (this->weaponList[i].x - (this->enemyData.x  + 80 * engineMathCos(3.1415 * this->weaponList[i].step * 0.01)));
							this->weaponList[i].y -= 0.01 * (this->weaponList[i].y - (this->enemyData.y +  0));
							break;
						case DATAENEMYWEAPONMOTIONTYPE_SHIELD2:
							this->weaponList[i].x -= 0.1 * (this->weaponList[i].x - (this->enemyData.x  + 50 * engineMathCos(3.1415 * this->weaponList[i].step * 0.01)));
							this->weaponList[i].y -= 0.01 * (this->weaponList[i].y - (this->enemyData.y + 40));
							break;
					}
					if(!this->playerData.isGiveup && this->weaponList[i].y > -160 && !this->weaponList[i].attacked){
						this->weaponList[i].attacked = true;
						// 攻撃がプレイヤーにヒットした瞬間 数字と爆発エフェクト
						double ratio = 1 + 0.2 * ((engineMathRandomGet() % 101) * 0.02 - 1);
						int power = this->enemyData.attackBase * dataEnemy->weapon[this->weaponList[i].weaponIndex].attackRatio100 * 0.01 * ratio;
						double vx = ((double)(engineMathRandomGet() % 101) - 50) / 50;
						double vy = -8 + 2 * (double)(engineMathRandomGet() % 100) / 100;
						soundSePlayBomb();
						createEffect(this, PAGRJUMPMAINEFFECTTYPE_DAMAGE, DATAATTRIBUTIONKIND_WHITE, this->weaponList[i].x, this->weaponList[i].y, vx, vy, power);
						createEffect(this, PAGRJUMPMAINEFFECTTYPE_BOMB, dataEnemy->weapon[this->weaponList[i].weaponIndex].attributionKind, this->weaponList[i].x, this->weaponList[i].y, 0.0, 0.0, 30);
						this->playerData.stepShake = -15;
						// プレーヤーにダメージ
						int health = storageStatusGet()->health;
						health -= power; if(health < 0){health = 0;}
						storageStatusGet()->health = health;
						storageStatusUpdate();
					}
				}
			}else if(this->weaponList[i].type == PAGRJUMPMAINWEAPONACTIONTYPE_FALL){
				// 落下状態
				this->weaponList[i].vy += 1;
			}

			if(this->weaponList[i].x < -160 - 50 || 160 + 50 < this->weaponList[i].x || this->weaponList[i].y < -480 - 50 || -150 + 50 < this->weaponList[i].y){
				// 攻撃が隠れたとき
				this->weaponList[i].type = PAGRJUMPMAINWEAPONACTIONTYPE_NONE;
			}

			// 位置計算
			this->weaponList[i].x += this->weaponList[i].vx;
			this->weaponList[i].y += this->weaponList[i].vy;
			this->weaponList[i].step++;
		}
	}

	// プレイヤー体力ゲージ減衰
	int playerHealth = storageStatusGet()->health;
	this->playerData.healthPrev -= (this->playerData.healthPrev - playerHealth) * 0.1;
	if(playerHealth <= 0 && !this->playerData.isGiveup){
		this->playerData.isGiveup = true;
		this->playerData.step = 0;
		// 戦闘結果 虚弱体質状態になる
		storageStatusGet()->isWeak = true;
		storageStatusUpdate();
	}
	// プレイヤー振動の減衰
	if(this->playerData.stepShake < 0){this->playerData.stepShake++;}

	// プレイヤー計算
	this->playerData.puppetNum = parameterPlayerPuppetNum();
	// プレイヤーテンション計算
	for(int i = 0; i < PAGEJUMPMAIN_TENSIONNUM; i++){
		int y = this->playerData.tensionEffect[i].y - 1;
		if(y < 0){y = 44; this->playerData.tensionEffect[i].x = engineMathRandomGet() % 44;}
		this->playerData.tensionEffect[i].y = y;
	}
	if(this->playerData.isGiveup){
		// プレイヤーがやられたとき
		if(this->playerData.step < 240){
			// 爆発エフェクト
			this->playerData.stepShake = 5;
			if((this->playerData.step % 10) == 0){
				int x = (int)(engineMathRandomGet() % 321) - 160;
				int y = (int)(engineMathRandomGet() % 81) - 160;
				soundSePlayBomb();
				createEffect(this, PAGRJUMPMAINEFFECTTYPE_BOMB, DATAATTRIBUTIONKIND_WHITE, x, y, 0.0, 0.0, 50);
			}
		}else if(this->playerData.step == 240){
			this->playerData.stepShake = 0;
		}else{
			// BGMをちいさくする
			soundBgmToneDown();
			// ポップアップ展開
			this->popupResult = popupJumpResultLose(this->stat);
			return 0;
		}
	}
	this->playerData.step++;

	// パペット計算
	double timeScale = 0.01;
	double rmax0 = 50;
	double rmax1 = 16;
	double maxspeed = 1.0;
	double minspeed = 0.1;
	// パペット相互作用計算
	for(int i = 0; i < PAGEJUMPMAIN_PUPPETNUM; i++){
		bool isTypei = (this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_WALK);
		isTypei = isTypei || (this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_KEEP);
		if(isTypei){
			// パペット間相互作用計算
			for(int j = 0; j < PAGEJUMPMAIN_PUPPETNUM; j++){
				if(i == j){continue;}
				bool isTypej = (this->puppetList[j].type == PAGRJUMPMAINPUPPETACTIONTYPE_WALK);
				isTypej = isTypej || (this->puppetList[j].type == PAGRJUMPMAINPUPPETACTIONTYPE_GRAB);
				isTypej = isTypej || (this->puppetList[j].type == PAGRJUMPMAINPUPPETACTIONTYPE_KEEP);
				if(!isTypej){continue;}
				double dx = this->puppetList[i].x - this->puppetList[j].x;
				double dy = this->puppetList[i].y - this->puppetList[j].y;
				double rr = dx * dx + dy * dy;
				double irr = (engineMathAbs(rr) < DBL_EPSILON) ? 0 : (rmax0 * rmax0 / rr);
				double irrrrrr = irr * irr * irr;
				double lj = (engineMathAbs(rr) < DBL_EPSILON || irrrrrr < 1) ? 0 : ((irrrrrr * irrrrrr - irrrrrr) / rr);
				double dvx = timeScale * lj * dx;
				double dvy = timeScale * lj * dy;
				if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_WALK){
					this->puppetList[i].vx += dvx;
					this->puppetList[i].vy += dvy;
				}else if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_KEEP){
					this->puppetList[i].vx += dvx;
				}
				if(this->puppetList[j].type == PAGRJUMPMAINPUPPETACTIONTYPE_WALK){
					this->puppetList[j].vx -= dvx;
					this->puppetList[j].vy -= dvy;
				}else if(this->puppetList[j].type == PAGRJUMPMAINPUPPETACTIONTYPE_KEEP){
					this->puppetList[j].vx -= dvx;
				}
			}
		}
		if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_WALK){
			// 壁との相互作用計算
			double dx0 = this->puppetList[i].x - (-160);
			double dx1 = this->puppetList[i].x - ( 160);
			double dy0 = this->puppetList[i].y - (-150);
			double dy1 = this->puppetList[i].y - ( -80);
			double irrx0 = (engineMathAbs(dx0) < DBL_EPSILON) ? 0 : (rmax1 * rmax1 / (dx0 * dx0));
			double irrx1 = (engineMathAbs(dx1) < DBL_EPSILON) ? 0 : (rmax1 * rmax1 / (dx1 * dx1));
			double irry0 = (engineMathAbs(dy0) < DBL_EPSILON) ? 0 : (rmax1 * rmax1 / (dy0 * dy0));
			double irry1 = (engineMathAbs(dy1) < DBL_EPSILON) ? 0 : (rmax1 * rmax1 / (dy1 * dy1));
			double irrrrrrx0 = irrx0 * irrx0 * irrx0;
			double irrrrrrx1 = irrx1 * irrx1 * irrx1;
			double irrrrrry0 = irry0 * irry0 * irry0;
			double irrrrrry1 = irry1 * irry1 * irry1;
			double dvx0 = (dx0 <= 0) ?  maxspeed : (irrrrrrx0 < 1) ? 0 : (timeScale * ((irrrrrrx0 * irrrrrrx0 - irrrrrrx0) / (dx0 * dx0)) * dx0);
			double dvx1 = (dx1 >= 0) ? -maxspeed : (irrrrrrx1 < 1) ? 0 : (timeScale * ((irrrrrrx1 * irrrrrrx1 - irrrrrrx1) / (dx1 * dx1)) * dx1);
			double dvy0 = (dy0 <= 0) ?  maxspeed : (irrrrrry0 < 1) ? 0 : (timeScale * ((irrrrrry0 * irrrrrry0 - irrrrrry0) / (dy0 * dy0)) * dy0);
			double dvy1 = (dy1 >= 0) ? -maxspeed : (irrrrrry1 < 1) ? 0 : (timeScale * ((irrrrrry1 * irrrrrry1 - irrrrrry1) / (dy1 * dy1)) * dy1);
			this->puppetList[i].vx += dvx0;
			this->puppetList[i].vx += dvx1;
			this->puppetList[i].vy += dvy0;
			this->puppetList[i].vy += dvy1;
		}
	}
	// パペット位置計算
	for(int i = 0; i < PAGEJUMPMAIN_PUPPETNUM; i++){
		if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_NONE){
			// 生成前状態
			if(this->puppetList[i].step > 0 && this->playerData.puppetNum > 0 && !this->playerData.isGiveup && !this->enemyData.isGiveup){
				// パペット抽選
				int puppetKind = -1;
				int totalFrequency = 0;
				int frequencyTable[STORAGEPUPPET0001_KIND];
				for(int j = 0; j < STORAGEPUPPET0001_KIND; j++){frequencyTable[j] = storagePuppetGet()->list[j].num;}
				for(int j = 0; j < PAGEJUMPMAIN_PUPPETNUM; j++){if(this->puppetList[j].type != PAGRJUMPMAINPUPPETACTIONTYPE_NONE){frequencyTable[this->puppetList[j].puppetKind]--;}}
				for(int j = 0; j < STORAGEPUPPET0001_KIND; j++){frequencyTable[j] = (frequencyTable[j] > 0) ? 1 : 0; totalFrequency += frequencyTable[j];}
				if(totalFrequency > 0){
					int randomValue = engineMathRandomGet() % totalFrequency;
					for(int j = 0; j < STORAGEPUPPET0001_KIND; j++){
						if(randomValue < frequencyTable[j]){
							puppetKind = j;
							break;
						}else{randomValue -= frequencyTable[j];}
					}
					if(puppetKind >= 0){
						// 新しいパペット生成
						this->puppetList[i].puppetKind = puppetKind;
						this->puppetList[i].type = PAGRJUMPMAINPUPPETACTIONTYPE_WALK;
						this->puppetList[i].x = (int)(engineMathRandomGet() % 301) - 150;
						this->puppetList[i].y = 50;
						this->puppetList[i].vx = 0;
						this->puppetList[i].vy = 0;
						this->puppetList[i].rot = 3.1415 / 180 *  270;
						this->puppetList[i].step = 0;
					}
				}
			}
		}else if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_WALK){
			// 放置状態
			if(this->playerData.isGiveup){
				// プレイヤーがやられたときパペットをしまう
				this->puppetList[i].type = PAGRJUMPMAINPUPPETACTIONTYPE_KEEP;
				this->puppetList[i].vx = 0.0;
				this->puppetList[i].vy = 1.0;
			}else{
				if(this->puppetList[i].vx > maxspeed){this->puppetList[i].vx = maxspeed;}else if(this->puppetList[i].vx < -maxspeed){this->puppetList[i].vx = -maxspeed;}
				if(this->puppetList[i].vy > maxspeed){this->puppetList[i].vy = maxspeed;}else if(this->puppetList[i].vy < -maxspeed){this->puppetList[i].vy = -maxspeed;}
				this->puppetList[i].vx *= 0.99;
				this->puppetList[i].vy *= 0.99;
				if(this->puppetList[i].vx * this->puppetList[i].vx + this->puppetList[i].vy * this->puppetList[i].vy < minspeed * minspeed){this->puppetList[i].vx = this->puppetList[i].vy = 0;}
				else{this->puppetList[i].rotNext = engineMathAtan2(this->puppetList[i].vy, this->puppetList[i].vx);}
			}
		}else if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_KEEP){
			// 保持状態
			if(this->puppetList[i].vx > maxspeed){this->puppetList[i].vx = maxspeed;}else if(this->puppetList[i].vx < -maxspeed){this->puppetList[i].vx = -maxspeed;}
			this->puppetList[i].rotNext = engineMathAtan2(this->puppetList[i].vy, this->puppetList[i].vx);
			if(this->puppetList[i].y > 50){this->puppetList[i].type = PAGRJUMPMAINPUPPETACTIONTYPE_NONE; this->puppetList[i].step = 0;}
		}else if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_JUMP){
			// 投擲状態
			this->puppetList[i].vy += 1;
			if(this->puppetList[i].vy > 0){
				// 頂点到達
				this->puppetList[i].type = PAGRJUMPMAINPUPPETACTIONTYPE_FALL;
				// 衝突判定
				int puppetKind = this->puppetList[i].puppetKind;
				struct dataPuppet *dataPuppet = dataPuppetGet(puppetKind, NULL);
				double x = this->puppetList[i].x;
				double y = this->puppetList[i].y - 16;
				int power = 0;
				int bombSize = 20;
				double levelRatio = (storagePuppetGet()->list[puppetKind].level * 0.01) * (dataPuppet->levelRatio10 * 0.1 - 1);
				double tensionRatio = (this->playerData.tension * 0.01) * (dataPuppet->tensionRatio10 * 0.1 - 1);
				if(this->playerData.tension < parameterPlayerTensionMax()){tensionRatio *= 0.5;}
				double ratio = (1 + levelRatio) * (1 + tensionRatio) * (1 + 0.2 * ((engineMathRandomGet() % 101) * 0.02 - 1));
				if(dataPuppet != NULL && dataEnemy != NULL && !this->enemyData.isGiveup){
					// エネミー攻撃衝突判定
					for(int i = 0; i < DATAENEMY_WEAPONNUM; i++){
						bool isType = (this->weaponList[i].type == PAGRJUMPMAINWEAPONACTIONTYPE_ATTACK);
						if(isType && !this->weaponList[i].attacked){
							double x0 = x - this->weaponList[i].x;
							double y0 = y - this->weaponList[i].y;
							double r0 = dataEnemy->weapon[this->weaponList[i].weaponIndex].radius;
							if(x0 * x0 + y0 * y0 < r0 * r0){
								// エネミー攻撃にヒット
								double attributionRatio = dataEnemy->weapon[this->weaponList[i].weaponIndex].attributionRatio10[dataPuppet->attributionKind] * 0.1;
								power = dataPuppet->attackSub * ratio * attributionRatio;
								bombSize = (attributionRatio > 1.1) ? 30 : (attributionRatio < 0.9) ? 15 : 20;
								// エネミー攻撃にダメージ
								int health = this->weaponList[i].health;
								health -= power; if(health < 0){health = 0;}
								this->weaponList[i].health = health;
								if(health <= 0){
									// エネミー攻撃が打ち消された
									this->weaponList[i].type = PAGRJUMPMAINWEAPONACTIONTYPE_FALL;
									this->weaponList[i].vx = -0.5 * x0;
									this->weaponList[i].vy = -30.0;
								}
								break;
							}
						}
					}
					// エネミー衝突判定
					if(power <= 0){
						int index = -1;
						for(int i = 0; i < DATAENEMY_WEAKNUM; i++){
							double x0 = x - (this->enemyData.x + dataEnemy->weak[i].x);
							double y0 = y - (this->enemyData.y + dataEnemy->weak[i].y);
							double r0 = dataEnemy->weak[i].radius;
							if(x0 * x0 + y0 * y0 < r0 * r0){index = i;}
						}
						if(index >= 0){
							// エネミーにヒット
							double attributionRatio = dataEnemy->weak[index].attributionRatio10[dataPuppet->attributionKind] * 0.1;
							power = dataPuppet->attackMain * ratio * attributionRatio;
							bombSize = (attributionRatio > 1.1) ? 30 : (attributionRatio < 0.9) ? 15 : 20;
							this->enemyData.stepShake = -15;
							// エネミーにダメージ
							int health = storageEnemy->health;
							int healthMax = this->enemyData.healthMainBase * dataEnemy->healthRatio100 * 0.01;
							health -= power; if(health < 0){health = 0;}
							storageEnemy->health = health;
							storageEnemy->healthPercent = 100 * (double)health / healthMax;
							storageFieldUpdate();
						}
					}
				}
				if(power > 0){
					// 攻撃がエネミーにヒットした瞬間 数字と爆発エフェクト
					double vx = ((double)(engineMathRandomGet() % 101) - 50) / 50;
					double vy = -5 + 2 * (double)(engineMathRandomGet() % 100) / 100;
					soundSePlayBomb();
					createEffect(this, PAGRJUMPMAINEFFECTTYPE_DAMAGE, DATAATTRIBUTIONKIND_WHITE, x, y, vx, vy, power);
					createEffect(this, PAGRJUMPMAINEFFECTTYPE_BOMB, dataPuppet->attributionKind, x, y, 0.0, 0.0, bombSize);
				}
			}
		}else if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_FALL){
			// 落下状態
			this->puppetList[i].vy += 1;
			if(this->puppetList[i].y > -80){this->puppetList[i].type = PAGRJUMPMAINPUPPETACTIONTYPE_NONE; this->puppetList[i].step = 0;}
		}

		// 回転角度の減衰計算
		double dr = this->puppetList[i].rot - this->puppetList[i].rotNext;
		while(dr < -3.1415){dr += 3.1415 * 2;}
		while(dr >  3.1415){dr -= 3.1415 * 2;}
		if(engineMathAbs(dr) > 0.01){this->puppetList[i].rot -= dr * 0.1;}
		// 位置計算
		this->puppetList[i].x += this->puppetList[i].vx;
		this->puppetList[i].y += this->puppetList[i].vy;
		this->puppetList[i].step++;
	}
	// じゃくてんカンパくんによるあたり判定フラグ
	bool isHitArea = false;
	for(int i = 0; i < PAGEJUMPMAIN_PUPPETNUM; i++){if(this->puppetList[i].puppetKind == 21 && this->puppetList[i].type != PAGRJUMPMAINPUPPETACTIONTYPE_NONE){isHitArea = true;}}
	if(isHitArea){if(this->playerData.stepHitArea < 180){this->playerData.stepHitArea++;}}
	else if(this->playerData.stepHitArea > 0){this->playerData.stepHitArea--;}

	// エフェクト計算
	for(int i = 0; i < PAGEJUMPMAIN_EFFECTNUM; i++){
		if(this->effectList[i].type == PAGRJUMPMAINEFFECTTYPE_NONE){
		}else if(this->effectList[i].type == PAGRJUMPMAINEFFECTTYPE_DAMAGE){
			// ダメージ数値
			this->effectList[i].vy += 0.3;
			if(this->effectList[i].y > 50){this->effectList[i].type = PAGRJUMPMAINEFFECTTYPE_NONE;}
		}else if(this->effectList[i].type == PAGRJUMPMAINEFFECTTYPE_BOMB){
			// 爆発エフェクト
			if(this->effectList[i].step + 1 == PAGEJUMPMAIN_BOMBNUM){this->effectList[i].type = PAGRJUMPMAINEFFECTTYPE_NONE;}
		}

		// 位置計算
		this->effectList[i].x += this->effectList[i].vx;
		this->effectList[i].y += this->effectList[i].vy;
		this->effectList[i].step++;
	}
	// エフェクト プレイヤーダメージ振動
	if(this->playerData.stepShake != 0){
		int power = abs(this->playerData.stepShake); if(power > 5){power = 5;}
		this->playerData.stepShakePowerx = power * ((double)(engineMathRandomGet() % 101) - 50) / 50;
		this->playerData.stepShakePowery = power * ((double)(engineMathRandomGet() % 101) - 50) / 50;
	}
	// エフェクト エネミーダメージ振動
	if(this->enemyData.stepShake != 0){
		int power = abs(this->enemyData.stepShake); if(power > 5){power = 5;}
		this->enemyData.stepShakePowerx = power * ((double)(engineMathRandomGet() % 101) - 50) / 50;
		this->enemyData.stepShakePowery = power * ((double)(engineMathRandomGet() % 101) - 50) / 50;
	}

	// にげるボタン押下確認
	this->btnBack.super.inactive = (this->playerData.isGiveup || this->enemyData.isGiveup);
	if(this->btnBack.super.trigger || global.key.bk.triggerInactive){
		this->btnBack.super.trigger = false;
		global.key.bk.triggerInactive = false;
		if(!this->btnBack.super.inactive){
			soundSePlayOK();
			if(this->playerData.tension > 0 && this->playerData.puppetNum > 0){
				// ポップアップ展開
				this->popupEscape = popupJumpEscape(this->stat);
				return 0;
			}else{
				// ページ遷移
				pageCartridgePush(pageFieldInit());
				pageJumpStatusDispose(this->stat);
				return 1;
			}
		}
	}

	return 0;
}

// ----------------------------------------------------------------

// バッファ作成
static void createBuffer(struct pageCartridgeJumpMain *this){
	// 状態確認
	int enemyKind = storageFieldGet()->list[this->enemyData.fieldKind].enemy[this->enemyData.enemyIndex].enemyKind;
	int status = 1 | ((enemyKind & 0x7fff) << 1);
	struct dataEnemy *dataEnemy = dataEnemyGet(enemyKind, NULL);
	if(dataEnemy == NULL){
		// ロード中
		status = -1;
	}
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->bufferStatus != status || this->bufferScreen != screen){
		this->bufferStatus = status;
		this->bufferScreen = screen;

		// バッファ作成開始
		engineGraphicBufferBegin();

		// フィールド足場
		e3dImageCreateArray(&this->fieldFoothold, TEXSIZ_FIELD_W, TEXSIZ_FIELD_H, global.screen.w * 0.5 - 160, global.screen.h - 160, 320, 160, TEXPOS_FIELD_FOOTHOLD_X, TEXPOS_FIELD_FOOTHOLD_Y, TEXPOS_FIELD_FOOTHOLD_W, TEXPOS_FIELD_FOOTHOLD_H);
		// エネミー本体
		e3dImageCreateArray(&this->enemyBody[0], TEXSIZ_ENEMY_W, TEXSIZ_ENEMY_H, -100, -100, 200, 200, TEXPOS_ENEMY_BODY_X + (TEXPOS_ENEMY_BODY_W + TEXPOS_SPACE) * 0, TEXPOS_ENEMY_BODY_Y, TEXPOS_ENEMY_BODY_W, TEXPOS_ENEMY_BODY_H);
		e3dImageCreateArray(&this->enemyBody[1], TEXSIZ_ENEMY_W, TEXSIZ_ENEMY_H, -100, -100, 200, 200, TEXPOS_ENEMY_BODY_X + (TEXPOS_ENEMY_BODY_W + TEXPOS_SPACE) * 1, TEXPOS_ENEMY_BODY_Y, TEXPOS_ENEMY_BODY_W, TEXPOS_ENEMY_BODY_H);
		e3dImageCreateArray(&this->enemyWeapon[0], TEXSIZ_ENEMY_W, TEXSIZ_ENEMY_H, -50, -50, 100, 100, TEXPOS_ENEMY_WEAPON_X + (TEXPOS_ENEMY_WEAPON_W + TEXPOS_SPACE) * 0, TEXPOS_ENEMY_WEAPON_Y, TEXPOS_ENEMY_WEAPON_W, TEXPOS_ENEMY_WEAPON_H);
		e3dImageCreateArray(&this->enemyWeapon[1], TEXSIZ_ENEMY_W, TEXSIZ_ENEMY_H, -50, -50, 100, 100, TEXPOS_ENEMY_WEAPON_X + (TEXPOS_ENEMY_WEAPON_W + TEXPOS_SPACE) * 1, TEXPOS_ENEMY_WEAPON_Y, TEXPOS_ENEMY_WEAPON_W, TEXPOS_ENEMY_WEAPON_H);
		e3dImageCreateArray(&this->enemyWeapon[2], TEXSIZ_ENEMY_W, TEXSIZ_ENEMY_H, -50, -50, 100, 100, TEXPOS_ENEMY_WEAPON_X + (TEXPOS_ENEMY_WEAPON_W + TEXPOS_SPACE) * 2, TEXPOS_ENEMY_WEAPON_Y, TEXPOS_ENEMY_WEAPON_W, TEXPOS_ENEMY_WEAPON_H);
		e3dImageCreateArray(&this->enemyWeapon[3], TEXSIZ_ENEMY_W, TEXSIZ_ENEMY_H, -50, -50, 100, 100, TEXPOS_ENEMY_WEAPON_X + (TEXPOS_ENEMY_WEAPON_W + TEXPOS_SPACE) * 3, TEXPOS_ENEMY_WEAPON_Y, TEXPOS_ENEMY_WEAPON_W, TEXPOS_ENEMY_WEAPON_H);
		// エネミーウインドウ作成
		int wx1 = global.screen.w - 5 - 50;
		int wy1 = 5;
		int wx2 = global.screen.w - 5 - 50 - 5 - 255;
		int wy2 = 5;
		e3dImageBoxDefaultCreateArray(&this->enemyWindow1, wx1, wy1, 50, 50);
		e3dImageBoxDefaultCreateArray(&this->enemyWindow2, wx2, wy2, 255, 25);
		e3dImageCreateArray(&this->enemyIcon[0], TEXSIZ_ENEMY_W, TEXSIZ_ENEMY_H, wx1 + 3, wy1 + 3, 44, 44, TEXPOS_ENEMY_ICON_X + (TEXPOS_ENEMY_ICON_W + TEXPOS_SPACE) * 0, TEXPOS_ENEMY_ICON_Y, TEXPOS_ENEMY_ICON_W, TEXPOS_ENEMY_ICON_H);
		e3dImageCreateArray(&this->enemyIcon[1], TEXSIZ_ENEMY_W, TEXSIZ_ENEMY_H, wx1 + 3, wy1 + 3, 44, 44, TEXPOS_ENEMY_ICON_X + (TEXPOS_ENEMY_ICON_W + TEXPOS_SPACE) * 1, TEXPOS_ENEMY_ICON_Y, TEXPOS_ENEMY_ICON_W, TEXPOS_ENEMY_ICON_H);
		e3dImageWhiteCreateArray(&this->enemyGauge, wx2 + 10, wy2 + 8, 180, 9);
		e3dImageTextCreateArray(&this->enemyName, wx2 + 10 + 180 + 5, wy2 + 5, (dataEnemy == NULL) ? "" : dataEnemy->name, 1.0, 1, 1);
		// プレイヤーウインドウ作成
		wx1 = 5;
		wy1 = global.screen.h - 5 - 30 - 5 - 50;
		wx2 = 5 + 50 + 5;
		wy2 = global.screen.h - 5 - 30 - 5 - 55;
		int wx3 = 5 + 50 + 5;
		int wy3 = global.screen.h - 5 - 30 - 5 - 25;
		int wx4 = 5;
		int wy4 = global.screen.h - 10 - 25;
		e3dImageBoxDefaultCreateArray(&this->playerWindow1, wx1, wy1, 50, 50);
		e3dImageBoxDefaultCreateArray(&this->playerWindow2, wx2, wy2, 130, 25);
		e3dImageBoxDefaultCreateArray(&this->playerWindow3, wx3, wy3, 255, 25);
		e3dImageBoxDefaultCreateArray(&this->playerWindow4, wx4, wy4, 205, 25);
		e3dImageCreateArray(&this->playerIcon[0], TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, wx1 + 3, wy1 + 3, 44, 44, TEXPOS_SYSTEM_IMAGEPLAYERICON_X, TEXPOS_SYSTEM_IMAGEPLAYERICON_Y + (TEXPOS_SYSTEM_IMAGEPLAYERICON_H + TEXPOS_SPACE) * 0, TEXPOS_SYSTEM_IMAGEPLAYERICON_W, TEXPOS_SYSTEM_IMAGEPLAYERICON_H);
		e3dImageCreateArray(&this->playerIcon[1], TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, wx1 + 3, wy1 + 3, 44, 44, TEXPOS_SYSTEM_IMAGEPLAYERICON_X, TEXPOS_SYSTEM_IMAGEPLAYERICON_Y + (TEXPOS_SYSTEM_IMAGEPLAYERICON_H + TEXPOS_SPACE) * 1, TEXPOS_SYSTEM_IMAGEPLAYERICON_W, TEXPOS_SYSTEM_IMAGEPLAYERICON_H);
		e3dImageWhiteCreateArray(&this->playerTension, wx1 + 3 - 2 * 0.5, wy1 + 3 - 20 * 0.5, 2, 20);
		e3dImageWhiteCreateArray(&this->playerGauge1, wx3 + 10 + 50 + 5, wy3 + 5, 180, 6);
		e3dImageWhiteCreateArray(&this->playerGauge2, wx3 + 10 + 50 + 5, wy3 + 14, 180, 6);
		e3dImageTextCreateArray(&this->playerName, wx3 + 10 + 50, wy3 + 5, "コッペン", 1.0, -1, 1);
		e3dImageTextCreateArray(&this->playerNum, wx2 + 10, wy2 + 5, "パペット     たい", 1.0, 1, 1);
		e3dNumberDefaultCreateArray(&this->countNumPuppet1, wx2 + 10 + 90, wy2 + 5, 1.0, -1, 1);
		e3dNumberDefaultCreateArray(&this->countNumPuppet2, wx4 + 175, wy4 + 5, 1.0, -1, 1);
		// エフェクト作成
		e3dNumberOutlineCreateArray(&this->effectDamage, 0.0, 0.0, 1.0, 0, 0);
		for(int i = 0; i < PAGEJUMPMAIN_BOMBNUM; i++){
			int vertIndex = engineGraphicBufferVretIndexGet();
			int faceIndex = engineGraphicBufferFaceIndexGet();
			int tetraNum = 32;
			double num = 1 - (double)i / PAGEJUMPMAIN_BOMBNUM;
			double r1 = 1 - 0.4 * num * num;
			double r2 = 1 - 1.0 * num * num;
			for(int j = 0; j < tetraNum; j++){
				double t = 3.1415 * 2 * j / tetraNum;
				double c = engineMathCos(t);
				double s = engineMathSin(t);
				engineGraphicBufferPushVert(r1 * c, r1 * s, 0.0);
				engineGraphicBufferPushVert(r2 * c, r2 * s, 0.0);
				engineGraphicBufferPushTexc((double)16 / TEXSIZ_SYSTEM_W, (double)16 / TEXSIZ_SYSTEM_H);
				engineGraphicBufferPushTexc((double)16 / TEXSIZ_SYSTEM_W, (double)16 / TEXSIZ_SYSTEM_H);
				int t0 = (j * 2) + 0;
				int t1 = (j * 2) + 1;
				int t2 = (((j + 1) % tetraNum) * 2) + 1;
				int t3 = (((j + 1) % tetraNum) * 2) + 0;
				engineGraphicBufferPushFace(vertIndex, t0, t1, t2);
				engineGraphicBufferPushFace(vertIndex, t2, t3, t0);
			}
			this->effectBomb[i].faceIndex = faceIndex;
			this->effectBomb[i].faceNum = tetraNum * 2;
		}
		// あたり判定範囲作成
		int vertIndex = engineGraphicBufferVretIndexGet();
		int faceIndex = engineGraphicBufferFaceIndexGet();
		int faceNum = 32;
		engineGraphicBufferPushVert(0.0, 0.0, 0.0);
		engineGraphicBufferPushTexc((double)16 / TEXSIZ_SYSTEM_W, (double)16 / TEXSIZ_SYSTEM_H);
		for(int i = 0; i < faceNum; i++){
			double t = 3.1415 * 2 * i / faceNum;
			engineGraphicBufferPushVert(engineMathCos(t), engineMathSin(t), 0.0);
			engineGraphicBufferPushTexc((double)16 / TEXSIZ_SYSTEM_W, (double)16 / TEXSIZ_SYSTEM_H);
			int t0 = 0;
			int t1 = 1 + i;
			int t2 = 1 + (i + 1) % faceNum;
			engineGraphicBufferPushFace(vertIndex, t0, t1, t2);
		}
		this->enemyHitArea.faceIndex = faceIndex;
		this->enemyHitArea.faceNum = faceNum;

		// チュートリアル矢印作成
		e3dImageCreateArray(&this->tutorialArrow1, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, -20, -20, 40, 40, TEXPOS_SYSTEM_IMAGETUTORIALPOINT_X, TEXPOS_SYSTEM_IMAGETUTORIALPOINT_Y, TEXPOS_SYSTEM_IMAGETUTORIALPOINT_W, TEXPOS_SYSTEM_IMAGETUTORIALPOINT_H);
		e3dImageCreateArray(&this->tutorialArrow2, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, global.screen.w - 5 - 100 + 20 - 40, global.screen.h - 35 + (30 - 40) * 0.5, 40, 40, TEXPOS_SYSTEM_IMAGETUTORIALARROW_X, TEXPOS_SYSTEM_IMAGETUTORIALARROW_Y, TEXPOS_SYSTEM_IMAGETUTORIALARROW_W, TEXPOS_SYSTEM_IMAGETUTORIALARROW_H);

		// ボタン作成
		partsButtonBoxCreateArray(&this->btnBack, global.screen.w - 100 - 5, global.screen.h - 30 - 5, 100, 30, "にげる", 1.0);

		// バッファ作成完了
		engineGraphicBufferEnd(&this->e3dIdVert, NULL, &this->e3dIdTexc, &this->e3dIdFace);
	}
}

// バッファ作成
static void createBufferPuppet(struct pageCartridgeJumpMain *this){
	// 状態確認
	int grabbedPuppetKind = this->playerData.grabbedPuppetKind;
	unsigned char puppetLevel = storagePuppetGet()->list[grabbedPuppetKind].level;
	int status = 1 | ((grabbedPuppetKind & 0x7fff) << 1) | ((puppetLevel & 0xff) << 16);
	struct dataPuppet *dataPuppet = dataPuppetGet(grabbedPuppetKind, NULL);
	if(dataPuppet == NULL){
		// ロード中
		status = 0;
	}
	int screen = (global.screen.w & 0xffff) | ((global.screen.h & 0xffff) << 16);

	if(this->puppetBuffer[grabbedPuppetKind].bufferStatus != status || this->puppetBuffer[grabbedPuppetKind].bufferScreen != screen){
		this->puppetBuffer[grabbedPuppetKind].bufferStatus = status;
		this->puppetBuffer[grabbedPuppetKind].bufferScreen = screen;

		if(this->puppetBuffer[grabbedPuppetKind].bufferStatus > 0){
			// バッファ作成開始
			engineGraphicBufferBegin();

			int wx4 = 5;
			int wy4 = global.screen.h - 10 - 25;
			char data[32];
			sprintf(data, "Lv.%-2d   たい", puppetLevel);
			e3dImageTextCreateArray(&this->puppetBuffer[grabbedPuppetKind].labelName, wx4 + 35, wy4 + 5, dataPuppet->name, 1.0, 1, 1);
			e3dImageTextCreateArray(&this->puppetBuffer[grabbedPuppetKind].labelData, wx4 + 95, wy4 + 5, data, 1.0, 1, 1);

			// バッファ作成完了
			engineGraphicBufferEnd(&this->puppetBuffer[grabbedPuppetKind].e3dIdVert, NULL, &this->puppetBuffer[grabbedPuppetKind].e3dIdTexc, &this->puppetBuffer[grabbedPuppetKind].e3dIdFace);
		}
	}
}

// 描画
static void draw(struct pageCartridgeJumpMain *this){
	// 描画準備
	createBuffer(this);
	createBufferPuppet(this);
	struct engineMathMatrix44 tempmat;

	// 射影行列作成
	engineMathMat4Ortho(&e3dMatOrtho, -global.screen.offset.x, global.screen.w + global.screen.offset.x, global.screen.h + global.screen.offset.y, -global.screen.offset.y, -1, 1);
	if(this->playerData.stepShake != 0){
		// プレイヤーダメージ振動
		engineMathMat4Translate(&e3dMatOrtho, this->playerData.stepShakePowerx, this->playerData.stepShakePowery, 0.0);
	}

	engineGraphicEngineClearAll();

	int e3dIdFieldImage;
	struct dataField *dataField = dataFieldGet(this->enemyData.fieldKind, &e3dIdFieldImage);
	// 背景描画
	dataBackgroundDraw((dataField == NULL) ? -1 : dataField->backgroundKindEnemy);
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_NORMAL);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);

	// パペット描画
	double rotv = 3.1415 / 180 *  0;
	double roth = 3.1415 / 180 * 30;
	hknwCharaListBind();
	for(int i = 0; i < PAGEJUMPMAIN_PUPPETNUM; i++){
		if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_NONE){continue;}
		int e3dIdPuppetImage;
		struct dataPuppet *dataPuppet = dataPuppetGet(this->puppetList[i].puppetKind, &e3dIdPuppetImage);
		if(dataPuppet != NULL){
			struct hknwChara *chara = hknwCharaListGet(dataPuppet->charaKind);
			enum hknwCharaPosesId pose = HKNWCHARAPOSESID_STAND;
			if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_WALK){
				// 放置状態 移動確認
				double vx = this->puppetList[i].vx;
				double vy = this->puppetList[i].vy;
				if(vx * vx + vy * vy > 0){pose = HKNWCHARAPOSESID_WALK;}
			}else if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_GRAB){
				// 掌握状態
			}else if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_KEEP){
				// 保持状態
				pose = HKNWCHARAPOSESID_WALK;
			}else if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_JUMP){
				// 投擲状態
			}else if(this->puppetList[i].type == PAGRJUMPMAINPUPPETACTIONTYPE_FALL){
				// 落下状態
			}
			engineGraphicEngineBindTexture(e3dIdPuppetImage);
			engineMathMat4Copy(&tempmat, &e3dMatOrtho);
			engineMathMat4Translate(&tempmat, global.screen.w * 0.5, global.screen.h, 0.0);
			engineMathMat4Translate(&tempmat, this->puppetList[i].x, this->puppetList[i].y, (this->puppetList[i].type != PAGRJUMPMAINPUPPETACTIONTYPE_FALL) ? 0.4 : -0.4);
			engineMathMat4Scale(&tempmat, 1.0, -1.0, 0.01);
			engineMathMat4RotateX(&tempmat, roth);
			engineMathMat4RotateY(&tempmat, rotv);
			int step = (int)engineMathFloor(this->puppetList[i].step / 10) % hknwCharaGetPoseStepNum(chara, pose);
			hknwCharaDrawChara(chara, &tempmat, rotv, roth, 0, 0, 0, this->puppetList[i].rot, 50.0, pose, step);
		}
	}
	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);

	// エネミー描画
	int e3dIdEnemyImage;
	struct storageFieldEnemy *storageEnemy = &storageFieldGet()->list[this->enemyData.fieldKind].enemy[this->enemyData.enemyIndex];
	struct dataEnemy *dataEnemy = dataEnemyGet(storageEnemy->enemyKind, &e3dIdEnemyImage);
	if(dataEnemy != NULL){
		engineGraphicEngineBindTexture(e3dIdEnemyImage);
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, global.screen.w * 0.5, global.screen.h, 0.0);
		engineMathMat4Translate(&tempmat, this->enemyData.x, this->enemyData.y, -0.8);
		if(this->enemyData.isGiveup && this->enemyData.stepShake != 0){engineMathMat4Translate(&tempmat, -this->enemyData.stepShakePowerx, -this->enemyData.stepShakePowery, 0.0);}
		engineGraphicEngineSetMatrix(&tempmat);
		e3dImageDraw(&this->enemyBody[this->enemyData.isGiveup ? 1 : 0]);
		// エネミー攻撃描画
		for(int i = 0; i < DATAENEMY_WEAPONNUM; i++){
			if(this->weaponList[i].type != PAGRJUMPMAINWEAPONACTIONTYPE_NONE){
				engineMathMat4Copy(&tempmat, &e3dMatOrtho);
				engineMathMat4Translate(&tempmat, global.screen.w * 0.5, global.screen.h, 0.0);
				engineMathMat4Translate(&tempmat, this->weaponList[i].x, this->weaponList[i].y, -0.6);
				engineGraphicEngineSetMatrix(&tempmat);
				e3dImageDraw(&this->enemyWeapon[dataEnemy->weapon[this->weaponList[i].weaponIndex].imageIndex]);
			}
		}
	}

	// 足場描画
	if(dataField != NULL){
		engineGraphicEngineSetMatrix(&e3dMatOrtho);
		engineGraphicEngineBindTexture(e3dIdFieldImage);
		e3dImageDraw(&this->fieldFoothold);
	}

	if(this->playerData.stepHitArea > 0 && dataEnemy != NULL && !this->playerData.isGiveup && !this->enemyData.isGiveup){
		// あたり判定描画
		engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
		engineGraphicEngineBindVertVBO(this->e3dIdVert);
		engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
		engineGraphicEngineBindFaceIBO(this->e3dIdFace);
		engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
		engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, 0.3 * ((double)this->playerData.stepHitArea / 180));
		for(int i = 0; i < DATAENEMY_WEAPONNUM; i++){
			bool isType = (this->weaponList[i].type == PAGRJUMPMAINWEAPONACTIONTYPE_ATTACK);
			if(isType && !this->weaponList[i].attacked){
				engineMathMat4Copy(&tempmat, &e3dMatOrtho);
				engineMathMat4Translate(&tempmat, global.screen.w * 0.5, global.screen.h, 0.0);
				engineMathMat4Translate(&tempmat, this->weaponList[i].x, this->weaponList[i].y, 0.0);
				engineMathMat4Scale(&tempmat, dataEnemy->weapon[this->weaponList[i].weaponIndex].radius, dataEnemy->weapon[this->weaponList[i].weaponIndex].radius, 1);
				engineGraphicEngineSetMatrix(&tempmat);
				e3dImageDraw(&this->enemyHitArea);
			}
		}
		for(int i = 0; i < DATAENEMY_WEAKNUM; i++){
			if(dataEnemy->weak[i].radius > 0){
				engineMathMat4Copy(&tempmat, &e3dMatOrtho);
				engineMathMat4Translate(&tempmat, global.screen.w * 0.5, global.screen.h, 0.0);
				engineMathMat4Translate(&tempmat, this->enemyData.x + dataEnemy->weak[i].x, this->enemyData.y + dataEnemy->weak[i].y, 0.0);
				engineMathMat4Scale(&tempmat, dataEnemy->weak[i].radius, dataEnemy->weak[i].radius, 1);
				engineGraphicEngineSetMatrix(&tempmat);
				e3dImageDraw(&this->enemyHitArea);
			}
		}
		engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_NORMAL);
		engineGraphicEngineBindVertVBO(this->e3dIdVert);
		engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
		engineGraphicEngineBindFaceIBO(this->e3dIdFace);
	}

	// エフェクト描画
	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
	for(int i = 0; i < PAGEJUMPMAIN_EFFECTNUM; i++){
		double r = 1.0;
		double g = 1.0;
		double b = 1.0;
		switch(this->effectList[i].attributionKind){
			case DATAATTRIBUTIONKIND_WHITE: r = 1.0; g = 1.0; b = 1.0; break;
			case DATAATTRIBUTIONKIND_RED: r = 1.0; g = 0.0; b = 0.0; break;
			case DATAATTRIBUTIONKIND_GREEN: r = 0.0; g = 1.0; b = 0.0; break;
			case DATAATTRIBUTIONKIND_BLUE: r = 0.0; g = 0.0; b = 1.0; break;
			case DATAATTRIBUTIONKIND_YELLOW: r = 1.0; g = 1.0; b = 0.0; break;
		}
		if(this->effectList[i].type == PAGRJUMPMAINEFFECTTYPE_DAMAGE){
			// ダメージ数値
			engineMathMat4Copy(&tempmat, &e3dMatOrtho);
			engineMathMat4Translate(&tempmat, global.screen.w * 0.5, global.screen.h, 0.0);
			engineMathMat4Translate(&tempmat, this->effectList[i].x, this->effectList[i].y, 0.8);
			engineGraphicEngineSetColorRgba(r, g, b, 1.0);
			e3dNumberDraw(&this->effectDamage, &tempmat, this->effectList[i].power);
		}else if(this->effectList[i].type == PAGRJUMPMAINEFFECTTYPE_BOMB){
			// 爆発エフェクト
			engineMathMat4Copy(&tempmat, &e3dMatOrtho);
			engineMathMat4Translate(&tempmat, global.screen.w * 0.5, global.screen.h, 0.0);
			engineMathMat4Translate(&tempmat, this->effectList[i].x, this->effectList[i].y, 0.8);
			engineMathMat4Scale(&tempmat, this->effectList[i].power, this->effectList[i].power, 1.0);
			engineGraphicEngineSetMatrix(&tempmat);
			engineGraphicEngineSetColorRgba(r, g, b, 1.0);
			e3dImageDraw(&this->effectBomb[this->effectList[i].step]);
		}
	}

	// 2D描画準備
	engineGraphicEngineSetStencilMode(ENGINEGRAPHICENGINEMODESTENCIL_NONE);
	engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
	engineGraphicEngineBindVertVBO(this->e3dIdVert);
	engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
	engineGraphicEngineBindFaceIBO(this->e3dIdFace);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);

	// エネミーアイコンウインドウ描画
	if(this->enemyData.stepShake != 0){
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, this->enemyData.stepShakePowerx, this->enemyData.stepShakePowery, 0.0);
		engineGraphicEngineSetMatrix(&tempmat);
	}else{engineGraphicEngineSetMatrix(&e3dMatOrtho);}
	engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
	e3dImageDraw(&this->enemyWindow1);
	if(dataEnemy != NULL){
		engineGraphicEngineBindTexture(e3dIdEnemyImage);
		e3dImageDraw(&this->enemyIcon[(this->enemyData.stepShake != 0 || this->enemyData.isGiveup) ? 1 : 0]);
		engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
	}
	// エネミーステータスウインドウ描画
	engineGraphicEngineSetMatrix(&e3dMatOrtho);
	e3dImageDraw(&this->enemyWindow2);
	engineGraphicEngineSetColorRgba(0.5, 0.0, 0.0, 1.0);
	e3dImageDraw(&this->enemyGauge);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, 1.0);
	e3dImageDraw(&this->enemyName);
	if(dataEnemy != NULL){
		// エネミー体力ゲージ
		int health = storageEnemy->health;
		int healthMax = this->enemyData.healthMainBase * dataEnemy->healthRatio100 * 0.01;
		double healthPrev = this->enemyData.healthPrev;
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, (global.screen.w - 125), 0, 0);
		engineMathMat4Scale(&tempmat, healthPrev / healthMax, 1, 1);
		engineMathMat4Translate(&tempmat, -(global.screen.w - 125), 0, 0);
		engineGraphicEngineSetMatrix(&tempmat);
		engineGraphicEngineSetColorRgba(1.0, 0.0, 0.0, 1.0);
		e3dImageDraw(&this->enemyGauge);
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, (global.screen.w - 125), 0, 0);
		engineMathMat4Scale(&tempmat, (double)health / healthMax, 1, 1);
		engineMathMat4Translate(&tempmat, -(global.screen.w - 125), 0, 0);
		engineGraphicEngineSetMatrix(&tempmat);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 0.0, 1.0);
		e3dImageDraw(&this->enemyGauge);
	}

	// プレイヤーアイコンウインドウ描画
	engineGraphicEngineSetMatrix(&e3dMatOrtho);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
	e3dImageDraw(&this->playerWindow1);
	e3dImageDraw(&this->playerIcon[(this->playerData.stepShake != 0 || this->playerData.isGiveup) ? 1 : 0]);
	// プレイヤーステータスウインドウ描画
	e3dImageDraw(&this->playerWindow2);
	e3dImageDraw(&this->playerWindow3);
	e3dImageDraw(&this->playerWindow4);
	engineGraphicEngineSetColorRgba(0.5, 0.0, 0.0, 1.0);
	e3dImageDraw(&this->playerGauge1);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.5, 1.0);
	e3dImageDraw(&this->playerGauge2);
	engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, 1.0);
	e3dImageDraw(&this->playerName);
	e3dImageDraw(&this->playerNum);
	e3dNumberDraw(&this->countNumPuppet1, &e3dMatOrtho, this->playerData.puppetNum);
	int tension = this->playerData.tension;
	int tensionMax = parameterPlayerTensionMax();
	if(tension >= tensionMax){
		// プレイヤーテンション描画
		engineGraphicEngineSetColorRgba(0.0, 1.0, 1.0, 1.0);
		for(int i = 0; i < PAGEJUMPMAIN_TENSIONNUM; i++){
			engineMathMat4Copy(&tempmat, &e3dMatOrtho);
			engineMathMat4Translate(&tempmat, this->playerData.tensionEffect[i].x, this->playerData.tensionEffect[i].y, 0);
			engineGraphicEngineSetMatrix(&tempmat);
			e3dImageDraw(&this->playerTension);
		}
	}
	// プレイヤー体力ゲージ
	int health = storageStatusGet()->health;
	int healthMax = this->playerData.healthMax;
	double healthPrev = this->playerData.healthPrev;
	engineMathMat4Copy(&tempmat, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat, 125, 0, 0);
	engineMathMat4Scale(&tempmat, healthPrev / healthMax, 1, 1);
	engineMathMat4Translate(&tempmat, -125, 0, 0);
	engineGraphicEngineSetMatrix(&tempmat);
	engineGraphicEngineSetColorRgba(1.0, 0.0, 0.0, 1.0);
	e3dImageDraw(&this->playerGauge1);
	engineMathMat4Copy(&tempmat, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat, 125, 0, 0);
	engineMathMat4Scale(&tempmat, (double)health / healthMax, 1, 1);
	engineMathMat4Translate(&tempmat, -125, 0, 0);
	engineGraphicEngineSetMatrix(&tempmat);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 0.0, 1.0);
	e3dImageDraw(&this->playerGauge1);
	// プレイヤー集中ゲージ
	engineMathMat4Copy(&tempmat, &e3dMatOrtho);
	engineMathMat4Translate(&tempmat, 125, 0, 0);
	engineMathMat4Scale(&tempmat, (double)tension / tensionMax, 1, 1);
	engineMathMat4Translate(&tempmat, -125, 0, 0);
	engineGraphicEngineSetMatrix(&tempmat);
	engineGraphicEngineSetColorRgba(0.0, 1.0, 1.0, 1.0);
	e3dImageDraw(&this->playerGauge2);

	// ボタン描画
	partsButtonBoxDraw(&this->btnBack, &e3dMatOrtho, 1.0);

	// 掌握パペット情報描画
	int grabbedPuppetKind = this->playerData.grabbedPuppetKind;
	if(grabbedPuppetKind >= 0 && this->puppetBuffer[grabbedPuppetKind].bufferStatus > 0){
		engineGraphicEngineSetColorRgba(0.0, 0.0, 0.0, 1.0);
		e3dNumberDraw(&this->countNumPuppet2, &e3dMatOrtho, storagePuppetGet()->list[grabbedPuppetKind].num);

		engineGraphicEngineBindVertVBO(this->puppetBuffer[grabbedPuppetKind].e3dIdVert);
		engineGraphicEngineBindTexcVBO(this->puppetBuffer[grabbedPuppetKind].e3dIdTexc);
		engineGraphicEngineBindFaceIBO(this->puppetBuffer[grabbedPuppetKind].e3dIdFace);
		engineGraphicEngineSetMatrix(&e3dMatOrtho);
		e3dImageDraw(&this->puppetBuffer[grabbedPuppetKind].labelName);
		e3dImageDraw(&this->puppetBuffer[grabbedPuppetKind].labelData);

		int e3dIdPuppetImage;
		struct dataPuppet *dataPuppet = dataPuppetGet(grabbedPuppetKind, &e3dIdPuppetImage);
		engineGraphicEngineBindTexture(e3dIdPuppetImage);
		// ハコニワキャラクタ行列作成
		int wx4 = 5;
		int wy4 = global.screen.h - 10 - 25;
		double rotv = 3.1415 / 180 *  0;
		double roth = 3.1415 / 180 * 30;
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, wx4 + 21, wy4 + 20, 0);
		engineMathMat4Scale(&tempmat, 1.0, -1.0, 0.01);
		engineMathMat4RotateX(&tempmat, roth);
		engineMathMat4RotateY(&tempmat, rotv);
		// ハコニワキャラクタ描画
		engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_NORMAL);
		engineGraphicEngineClearDepth();
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
		hknwCharaListBind();
		struct hknwChara *chara = hknwCharaListGet(dataPuppet->charaKind);
		hknwCharaDrawChara(chara, &tempmat, rotv, roth, 0, 0, 0, 3.1415 / 180 * 90, 35.0, HKNWCHARAPOSESID_STAND, 0);
		engineGraphicEngineSetDrawMode(ENGINEGRAPHICENGINEMODEDRAW_2D);
	}

	// チュートリアル矢印描画
	if(this->stepTutorial11 > 0){
		engineGraphicEngineBindVertVBO(this->e3dIdVert);
		engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
		engineGraphicEngineBindFaceIBO(this->e3dIdFace);
		engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
		double num = (double)(this->stepTutorial11 % 60) / 30;
		if(num > 1.0){num = 1.0;}
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, global.screen.w * 0.5 + 120, global.screen.h - 120 - 240 * (1 - (1 - num) * (1 - num)), 0.0);
		engineGraphicEngineSetMatrix(&tempmat);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1 - num * num);
		e3dImageDraw(&this->tutorialArrow1);
	}
	if(this->stepTutorial12 > 0){
		engineGraphicEngineBindVertVBO(this->e3dIdVert);
		engineGraphicEngineBindTexcVBO(this->e3dIdTexc);
		engineGraphicEngineBindFaceIBO(this->e3dIdFace);
		engineGraphicEngineBindTexture(e3dIdSystemImageLinear);
		engineMathMat4Copy(&tempmat, &e3dMatOrtho);
		engineMathMat4Translate(&tempmat, -engineMathAbs(10 * engineMathSin(3.1415 * this->stepTutorial12 * 0.05)), 0, 0);
		engineGraphicEngineSetMatrix(&tempmat);
		engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
		e3dImageDraw(&this->tutorialArrow2);
	}

	// ポップアップ描画
	if(this->popupEscape != NULL){this->popupEscape->draw(this->popupEscape);}
	if(this->popupResult != NULL){this->popupResult->draw(this->popupResult);}
	if(this->popupTutorial != NULL){this->popupTutorial->draw(this->popupTutorial);}

	// メモリートレースボタン描画
	partsButtonMemoryDraw();
	
	engineGraphicEngineFlush();
}

// ----------------------------------------------------------------

// 一時停止
static void pause(struct pageCartridgeJumpMain *this){
}

// 破棄
static void dispose(struct pageCartridgeJumpMain *this){
	if(this->popupEscape != NULL){this->popupEscape->dispose(this->popupEscape);}
	if(this->popupResult != NULL){this->popupResult->dispose(this->popupResult);}
	if(this->popupTutorial != NULL){this->popupTutorial->dispose(this->popupTutorial);}
	engineGraphicObjectVBODispose(this->e3dIdVert);
	engineGraphicObjectVBODispose(this->e3dIdTexc);
	engineGraphicObjectIBODispose(this->e3dIdFace);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------

// ページカートリッジ作成
struct pageCartridge *pageJumpMain(struct pageJumpStatus *stat){
	struct pageCartridgeJumpMain *this = (struct pageCartridgeJumpMain*)engineUtilMemoryCalloc(1, sizeof(struct pageCartridgeJumpMain));
	this->stat = stat;

	struct pageCartridge *cartridge = (struct pageCartridge*)this;
	cartridge->init = (void(*)(struct pageCartridge*))init;
	cartridge->calc = (int(*)(struct pageCartridge*))calc;
	cartridge->draw = (void(*)(struct pageCartridge*))draw;
	cartridge->pause = (void(*)(struct pageCartridge*))pause;
	cartridge->dispose = (void(*)(struct pageCartridge*))dispose;
	return cartridge;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

