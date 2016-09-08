#ifndef __data_h_
#define __data_h_

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ハコニワキャラクタの種類名
enum hknwCharaKind{
	HKNWCHARAKIND_HUMAN,
	HKNWCHARAKIND_HUMAN2,
	HKNWCHARAKIND_HUMAN3,
	HKNWCHARAKIND_GUDAGUDA,
	HKNWCHARAKIND_HANDSTAND,
	HKNWCHARAKIND_KUMA,
	HKNWCHARAKIND_BEAR,
	HKNWCHARAKIND_ROBO,
	HKNWCHARAKIND_DRAGON,
	HKNWCHARAKIND_BUTA,
	HKNWCHARAKIND_CART,
	HKNWCHARAKIND_ROCKET,
	HKNWCHARAKIND_BALL,
	HKNWCHARAKIND_COOKIE
};

// フィールドイベントタイプ
enum dataFieldEventType{
	DATAFIELDEVENTTYPE_NONE = 0,
	DATAFIELDEVENTTYPE_ICON,
	DATAFIELDEVENTTYPE_PREV,
	DATAFIELDEVENTTYPE_NEXT,
	DATAFIELDEVENTTYPE_TRIAL,
	DATAFIELDEVENTTYPE_MONEY,
	DATAFIELDEVENTTYPE_ITEM
};

// アイテム種類
enum dataItemKind{
	DATAITEMKIND_HEALTH = 0,
	DATAITEMKIND_TENSION = 1,
	DATAITEMKIND_MONSTERMOVE = 2,
	DATAITEMKIND_CANVASFULL = 3,
	DATAITEMKIND_CANVASOPEN = 4,
	DATAITEMKIND_WORKSPACEOPEN = 5
};

// 属性種類
enum dataAttributionKind{
	DATAATTRIBUTIONKIND_WHITE = 0,
	DATAATTRIBUTIONKIND_RED = 1,
	DATAATTRIBUTIONKIND_GREEN = 2,
	DATAATTRIBUTIONKIND_BLUE = 3,
	DATAATTRIBUTIONKIND_YELLOW = 4
};

// エネミーの動き方タイプ
enum dataEnemyMotionType{
	DATAENEMYMOTIONTYPE_STOP,
	DATAENEMYMOTIONTYPE_HORIZON,
	DATAENEMYMOTIONTYPE_VERTICAL,
	DATAENEMYMOTIONTYPE_SLASH,
	DATAENEMYMOTIONTYPE_INFINIT,
	DATAENEMYMOTIONTYPE_WARP
};

// エネミー攻撃の動き方タイプ
enum dataEnemyWeaponMotionType{
	DATAENEMYWEAPONMOTIONTYPE_DEFAULT,
	DATAENEMYWEAPONMOTIONTYPE_SPEED,
	DATAENEMYWEAPONMOTIONTYPE_MEANDERING,
	DATAENEMYWEAPONMOTIONTYPE_SHIELD1,
	DATAENEMYWEAPONMOTIONTYPE_SHIELD2
};

// 動的データ配列サイズ定数 ver.00.01
#define STORAGESTATUS0001_ITEMKIND 6
#define STORAGEPUPPET0001_KIND 40
#define STORAGEMEISTER0001_KIND 16
#define STORAGEATELIER0001_WORKSPACEXMAX 4
#define STORAGEATELIER0001_CANVASXMAX 16
#define STORAGEATELIER0001_CANVASYMAX 4
#define STORAGEFIELD0001_KIND 20
#define STORAGEFIELD0001_MAPMAX 256
#define STORAGEFIELD0001_ENEMYMAX 3

// 動的データ配列サイズ定数 ver.01.01
#define STORAGESTATUS0101_GACHAKIND 3

// 静的データ配列サイズ定数
#define DATAMEISTER_PRODUCTMAX 4
#define DATAGACHA_PRODUCTMAX 8
#define DATAFIELD_MAPMAX STORAGEFIELD0001_MAPMAX
#define DATAFIELD_EVENTMAX 32
#define DATAFIELD_PRODUCTMAX 4
#define DATAENEMY_ATTRIBUTIONKIND 5
#define DATAENEMY_WEAPONNUM 4
#define DATAENEMY_WEAKNUM 3

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// base64処理

char *base64encode(unsigned char *data, int length);
unsigned char *base64decodeChar(char *data, int *length);
unsigned char *base64decode(unsigned char *data, int length);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 静的データ構造体

// パペットデータ
struct dataPuppet{
	char code[16];
	char name[5 * 3 + 1];
	char fullname[15 * 3 + 1];
	char profile[(15 * 3 + 1) * 10];
	enum hknwCharaKind charaKind;
	// 戦闘パラメータ
	unsigned short attackMain;
	unsigned short attackSub;
	enum dataAttributionKind attributionKind;
	unsigned char tensionRatio10;
	unsigned char levelRatio10;
	signed char weight;
};

// マイスターデータ
struct dataMeister{
	char code[16];
	char name[5 * 3 + 1];
	char fullname[15 * 3 + 1];
	char profile[(15 * 3 + 1) * 10];
	// 出現するパペット
	unsigned short charge;
	struct dataMeisterProduct{
		signed short puppetKind;
		unsigned short frequency;
		unsigned char openLevel;
	} product[DATAMEISTER_PRODUCTMAX];
};

// ガチャデータ
struct dataGacha{
	char code[16];
	char name[15 * 3 + 1];
	unsigned int money;
	// 生成可能物
	struct dataGachaProduct{
		signed short meisterKind;
		unsigned short frequency;
		unsigned char isFirst;
	} product[DATAGACHA_PRODUCTMAX];
};

// フィールドデータ
struct dataField{
	char code[16];
	char name[15 * 3 + 1];
	signed short backgroundKindField;
	signed short backgroundKindEnemy;
	// マップ情報
	unsigned char xsize;
	unsigned char ysize;
	unsigned short openCost;
	struct dataFieldMap{
		unsigned char mapchip;
		unsigned char height;
	} map[DATAFIELD_MAPMAX];
	// イベント
	struct dataFieldEvent{
		enum dataFieldEventType type;
		signed char icon;
		unsigned char fieldx;
		unsigned char fieldy;
		union{
			struct{signed short nextFieldKind; unsigned char nextFieldx; unsigned char nextFieldy; unsigned char nextFieldr;};
			struct{unsigned short money;};
			struct{enum dataItemKind itemKind;};
		};
	} event[DATAFIELD_EVENTMAX];
	// 出現するエネミー
	unsigned short charge;
	struct dataFieldProduct{
		signed short enemyKind;
		unsigned short frequency;
		unsigned char level;
		enum dataItemKind itemKind;
		unsigned char itemPercent;
	} product[DATAFIELD_PRODUCTMAX];
};

// エネミーデータ
struct dataEnemy{
	char code[16];
	char name[5 * 3 + 1];
	char fullname[15 * 3 + 1];
	// 戦闘パラメータ
	unsigned char healthRatio100;
	enum dataEnemyMotionType motionType;
	unsigned char charge;
	struct{
		unsigned char imageIndex;
		unsigned char radius;
		unsigned char healthRatio10;
		unsigned char attackRatio100;
		enum dataEnemyWeaponMotionType motionType;
		enum dataAttributionKind attributionKind;
		unsigned char attributionRatio10[DATAENEMY_ATTRIBUTIONKIND];
	} weapon[DATAENEMY_WEAPONNUM];
	struct{
		signed char x;
		signed char y;
		unsigned char radius;
		unsigned char attributionRatio10[DATAENEMY_ATTRIBUTIONKIND];
	} weak[DATAENEMY_WEAKNUM];
	// 戦利品データ
	unsigned char moneyRatio100;
	unsigned char experienceRatio100;
};

// 背景データ
struct dataBackground{
	char code[16];
	unsigned short w;
	unsigned short h;
};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 動的データ構造体 ver.00.01

// ステータスデータ
struct storageStatus0001{
	unsigned int health;
	unsigned int experience;
	unsigned int money;
	unsigned char itemNum[STORAGESTATUS0001_ITEMKIND];
	unsigned char isTension;
	unsigned char isWeak;

	signed short fieldKind;
	unsigned char fieldx;
	unsigned char fieldy;
	unsigned char fieldr;
	signed char enemyIndex;
	long long int time;

	struct{
		unsigned char top01;
		unsigned char top02;
		unsigned char atelier01;
		unsigned char atelier02;
		unsigned char atelier03;
		unsigned char atelier04;
		unsigned char atelier05;
		unsigned char field01;
		unsigned char field02;
		unsigned char jump01;
		unsigned char jump02;
		unsigned char jump03;
	} tutorial;
};

// パペットデータ
struct storagePuppet0001{
	struct storagePuppetList{
		unsigned char num;
		unsigned char level;
	} list[STORAGEPUPPET0001_KIND];
};

// マイスターデータ
struct storageMeister0001{
	struct storageMeisterList{
		unsigned char level;
	} list[STORAGEMEISTER0001_KIND];
};

// アトリエデータ
struct storageAtelier0001{
	struct storageAtelierWorkspace{
		signed short meisterKind;
		unsigned short charge;
	} workspace[STORAGEATELIER0001_WORKSPACEXMAX];
	char workspaceSize;
	long long int time;

	struct storageAtelierCanvas{
		signed short puppetKind;
		unsigned char level;
	} canvas[STORAGEATELIER0001_CANVASXMAX][STORAGEATELIER0001_CANVASYMAX];
	char canvasSize;
};

// フィールドデータ
struct storageField0001{
	struct storageFieldList{
		long long int time;
		unsigned short charge;
		struct storageFieldMap{
			unsigned char open;
		} map[STORAGEFIELD0001_MAPMAX];
		struct storageFieldEnemy{
			unsigned char fieldx;
			unsigned char fieldy;
			signed short enemyKind;
			unsigned char level;
			unsigned int health;
			unsigned char healthPercent;
			enum dataItemKind itemKind;
			unsigned char isItemGet;
		} enemy[STORAGEFIELD0001_ENEMYMAX];
	} list[STORAGEFIELD0001_KIND];
};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 動的データ構造体 ver.01.01

// ステータスデータ
struct storageStatus0101{
	unsigned int health;
	unsigned int experience;
	unsigned int money;
	unsigned int gachaCount[STORAGESTATUS0101_GACHAKIND];
	unsigned char itemNum[STORAGESTATUS0001_ITEMKIND];
	unsigned char isTension;
	unsigned char isWeak;

	signed short fieldKind;
	unsigned char fieldx;
	unsigned char fieldy;
	unsigned char fieldr;
	signed char enemyIndex;
	long long int time;

	struct{
		unsigned char top01;
		unsigned char top02;
		unsigned char atelier01;
		unsigned char atelier02;
		unsigned char atelier03;
		unsigned char atelier04;
		unsigned char atelier05;
		unsigned char field01;
		unsigned char field02;
		unsigned char jump01;
		unsigned char jump02;
		unsigned char jump03;
	} tutorial;
};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#endif

