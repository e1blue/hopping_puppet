#ifndef __hknw_h_
#define __hknw_h_

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ハコニワマップ構造体
struct hknwMap{
	int xsize;
	int ysize;
	int zsize;
	int *mapchip;

	int e3dIdVert;
	int e3dIdClor;
	int e3dIdTexc;
	int e3dIdFace;
	int faceNum;
};

struct hknwMap *hknwMapInit(int xsize, int ysize, int zsize, int *mapchip);
void hknwMapDraw(struct hknwMap *this, struct engineMathMatrix44 *mat);
void hknwMapDispose(struct hknwMap *this);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ハコニワキャラクタのパーツ名
enum hknwCharaPartsId{
	HKNWCHARAPARTSID_HUMAN_HEAD,
	HKNWCHARAPARTSID_HUMAN_BODY,
	HKNWCHARAPARTSID_HUMAN_FTR1,
	HKNWCHARAPARTSID_HUMAN_FTL1,
	HKNWCHARAPARTSID_HUMAN_FTR2,
	HKNWCHARAPARTSID_HUMAN_FTL2,
	HKNWCHARAPARTSID_HUMAN_HNDR,
	HKNWCHARAPARTSID_HUMAN_HNDL,
	HKNWCHARAPARTSID_DRAGON_HED1,
	HKNWCHARAPARTSID_DRAGON_HED2,
	HKNWCHARAPARTSID_DRAGON_NOSE,
	HKNWCHARAPARTSID_DRAGON_BODY,
	HKNWCHARAPARTSID_DRAGON_TAIL,
	HKNWCHARAPARTSID_DRAGON_FTFR,
	HKNWCHARAPARTSID_DRAGON_FTFL,
	HKNWCHARAPARTSID_DRAGON_FTBR,
	HKNWCHARAPARTSID_DRAGON_FTBL,
	HKNWCHARAPARTSID_DRAGON_FOOT,
	HKNWCHARAPARTSID_CART_BODY,
	HKNWCHARAPARTSID_CART_BEAK,
	HKNWCHARAPARTSID_CART_TIRE,
	HKNWCHARAPARTSID_BALL_BOD1,
	HKNWCHARAPARTSID_BALL_BOD2,
	HKNWCHARAPARTSID_BALL_BOD3,
	HKNWCHARAPARTSID_BALL_BOD4
};

// ハコニワキャラクタのポーズ名
enum hknwCharaPosesId{
	HKNWCHARAPOSESID_STAND,
	HKNWCHARAPOSESID_WALK
};

// ハコニワキャラクタ構造体
struct hknwChara{
	int faceIndex;
	int faceNum;
	int shadowIndex;

	int imgw;
	int imgh;

	int partsNum;
	struct hknwCharaParts{
		enum hknwCharaPartsId partsId;
		double x;
		double y;
		double z;
		int texx;
		int texy;
		int texs;
		int type;
	} *parts;

	int posesNum;
	struct hknwCharaPoses{
		enum hknwCharaPosesId posesId;
		enum hknwCharaPartsId partsId;
		int stepIndex;
		double x;
		double y;
		double z;
		double r;
		int type;
	} *poses;

	struct hknwCharaShadow{
		int texx;
		int texy;
		int texs;
	} shadow;
};

struct hknwChara *hknwCharaInit(enum hknwCharaKind charaKind);
void hknwCharaCreateArray(struct hknwChara *this);
int hknwCharaGetPoseStepNum(struct hknwChara *this, enum hknwCharaPosesId poseId);
void hknwCharaDrawChara(struct hknwChara *this, struct engineMathMatrix44 *mat, double rotv, double roth, double x, double y, double z, double r, double s, enum hknwCharaPosesId poseId, int step);
void hknwCharaDrawShadow(struct hknwChara *this, struct engineMathMatrix44 *mat, double x, double y, double z, double s);
void hknwCharaDispose(struct hknwChara *this);

// ----------------------------------------------------------------

// ハコニワキャラクタ構造体管理関数
void hknwCharaListActive(bool active);
struct hknwChara *hknwCharaListGet(enum hknwCharaKind charaKind);
void hknwCharaListBind();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#endif

