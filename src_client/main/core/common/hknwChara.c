#include "../includeAll.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ハコニワキャラクタ作成用データのパーツ作成
static void setParts(struct hknwCharaParts *this, enum hknwCharaPartsId partsId, double x, double y, double z, int texx, int texy, int texs, int type){
	this->partsId = partsId;
	this->x = x;
	this->y = y;
	this->z = z;
	this->texx = texx;
	this->texy = texy;
	this->texs = texs;
	this->type = type;
}

// ハコニワキャラクタ作成用データのポーズ作成
static void setPoses(struct hknwCharaPoses *this, enum hknwCharaPosesId posesId, int stepIndex, enum hknwCharaPartsId partsId, double x, double y, double z, double r, int type){
	this->posesId = posesId;
	this->partsId = partsId;
	this->stepIndex = stepIndex;
	this->x = x;
	this->y = y;
	this->z = z;
	this->r = r;
	this->type = type;
}

// ----------------------------------------------------------------

// 人間系キャラの作成
static void createHumanData(struct hknwChara *this){
	this->partsNum = 12;
	this->posesNum = 6 * 5;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD,  0.00,  0.00,  0.00,  0,   0, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.06,  0.20, -0.02,  0,  96, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.06, -0.20, -0.02,  0,  96, 32, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.19,  0.00, -0.09,  0, 128, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.16,  0.00,  0.08,  0, 160, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.00,  0,  32, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.00,  0.00,  0,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00,  0.00,  0.00,  0,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR2,  0.00,  0.00,  0.00, 64,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL2,  0.00,  0.00,  0.00, 64,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDR,  0.00,  0.00,  0.00,  0,  80, 16, 0x01);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDL,  0.00,  0.00,  0.00,  0,  80, 16, 0x11);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.00,  0.00,  0.52, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_BODY, -0.02,  0.00,  0.27, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTR1,  0.02,  0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTL1, -0.02, -0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDR, -0.02,  0.20,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.02, -0.20,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.45, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTR1,  0.10,  0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTL2, -0.20, -0.07,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDR, -0.10,  0.15,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.10, -0.15,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.47, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.26, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00, -0.07,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDR, -0.05,  0.18,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDL,  0.05, -0.18,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.45, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTR2, -0.20,  0.07,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTL1,  0.10, -0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDR,  0.10,  0.15,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDL, -0.10, -0.15,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.47, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.26, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.07,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00, -0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDR,  0.05,  0.18,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDL, -0.05, -0.18,  0.25, 3.1415 * 0, 0x00);
}

// 人間系キャラの作成 その2
static void createHuman2Data(struct hknwChara *this){
	this->partsNum = 11;
	this->posesNum = 6 * 5;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD,  0.00,  0.00,  0.00,  0,   0, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.15,  0.15, -0.15,  0,  96, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.15, -0.15, -0.15,  0,  96, 32, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD,  0.10,  0.15,  0.10,  0, 128, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.00,  0,  32, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.00,  0.00,  0,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00,  0.00,  0.00,  0,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR2,  0.00,  0.00,  0.00, 64,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL2,  0.00,  0.00,  0.00, 64,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDR,  0.00,  0.00,  0.00,  0,  80, 16, 0x01);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDL,  0.00,  0.00,  0.00,  0,  80, 16, 0x11);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.00,  0.00,  0.52, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_BODY, -0.02,  0.00,  0.27, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTR1,  0.02,  0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTL1, -0.02, -0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDR, -0.02,  0.20,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.02, -0.20,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.45, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTR1,  0.10,  0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTL2, -0.20, -0.07,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDR, -0.10,  0.15,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.10, -0.15,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.47, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.26, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00, -0.07,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDR, -0.05,  0.18,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDL,  0.05, -0.18,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.45, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTR2, -0.20,  0.07,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTL1,  0.10, -0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDR,  0.10,  0.15,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDL, -0.10, -0.15,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.47, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.26, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.07,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00, -0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDR,  0.05,  0.18,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDL, -0.05, -0.18,  0.25, 3.1415 * 0, 0x00);
}

// 人間系キャラの作成 その3
static void createHuman3Data(struct hknwChara *this){
	this->partsNum = 12;
	this->posesNum = 6 * 5;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD,  0.00,  0.00,  0.00,  0,   0, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.01,  0.18,  0.08,  0,  96, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.01, -0.18,  0.08,  0,  96, 32, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD,  0.14,  0.08,  0.00, 64,  80, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD,  0.14, -0.08,  0.00, 64,  80, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.00,  0,  32, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.00,  0.00,  0,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00,  0.00,  0.00,  0,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR2,  0.00,  0.00,  0.00, 64,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL2,  0.00,  0.00,  0.00, 64,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDR,  0.00,  0.00,  0.00,  0,  80, 16, 0x01);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDL,  0.00,  0.00,  0.00,  0,  80, 16, 0x11);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.00,  0.00,  0.52, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_BODY, -0.02,  0.00,  0.27, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTR1,  0.02,  0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTL1, -0.02, -0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDR, -0.02,  0.20,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.02, -0.20,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.45, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTR1,  0.10,  0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTL2, -0.20, -0.07,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDR, -0.10,  0.15,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.10, -0.15,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.47, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.26, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00, -0.07,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDR, -0.05,  0.18,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDL,  0.05, -0.18,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.45, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTR2, -0.20,  0.07,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTL1,  0.10, -0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDR,  0.10,  0.15,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDL, -0.10, -0.15,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.47, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.26, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.07,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00, -0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDR,  0.05,  0.18,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDL, -0.05, -0.18,  0.25, 3.1415 * 0, 0x00);
}

// 人間系グダグダキャラの作成
static void createGudagudaData(struct hknwChara *this){
	this->partsNum = 11;
	this->posesNum = 6 * 5;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD,  0.00,  0.00,  0.00,  0,   0, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD,  0.14,  0.08,  0.00, 64,  80, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD,  0.14, -0.08,  0.00, 64,  80, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.19,  0.00, -0.09,  0, 128, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.00,  0,  32, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.00,  0.00,  0,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00,  0.00,  0.00,  0,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR2,  0.00,  0.00,  0.00, 64,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL2,  0.00,  0.00,  0.00, 64,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDR,  0.00,  0.00,  0.00,  0,  80, 16, 0x01);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDL,  0.00,  0.00,  0.00,  0,  80, 16, 0x11);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.06,  0.00,  0.38, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_BODY, -0.02,  0.00,  0.17, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTR2,  0.12,  0.10,  0.10, 3.1415 * 0, 0x10);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTL2,  0.12, -0.10,  0.10, 3.1415 * 0, 0x10);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDR,  0.15,  0.10,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.15, -0.10,  0.20, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.40, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTR1,  0.06,  0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTL1, -0.06, -0.07,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDR,  0.15,  0.10,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.15, -0.10,  0.15, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.42, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.26, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00, -0.07,  0.12, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDR,  0.15,  0.10,  0.17, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDL,  0.15, -0.10,  0.17, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.40, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTR1, -0.06,  0.07,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTL1,  0.06, -0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDR,  0.15,  0.10,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDL,  0.15, -0.10,  0.15, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.42, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.26, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.07,  0.12, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00, -0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDR,  0.15,  0.10,  0.17, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDL,  0.15, -0.10,  0.17, 3.1415 * 0, 0x00);
}

// 人間系逆立ちキャラの作成
static void createHandstandData(struct hknwChara *this){
	this->partsNum = 8;
	this->posesNum = 6 * 5;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD,  0.00,  0.00,  0.00,  0,   0, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.00,  0,  32, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.00,  0.00,  0,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00,  0.00,  0.00,  0,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR2,  0.00,  0.00,  0.00, 64,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL2,  0.00,  0.00,  0.00, 64,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDR,  0.00,  0.00,  0.00,  0,  80, 16, 0x01);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDL,  0.00,  0.00,  0.00,  0,  80, 16, 0x11);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.04,  0.00,  0.31, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_BODY, -0.10,  0.00,  0.47, 3.1415 * 0, 0x01);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTR2,  0.02,  0.10,  0.60, 3.1415 * 0, 0x11);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTL2, -0.02, -0.10,  0.60, 3.1415 * 0, 0x11);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDR, -0.02,  0.13,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.02, -0.13,  0.10, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.04,  0.00,  0.31, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_BODY, -0.10,  0.00,  0.47, 3.1415 * 0, 0x01);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTR2,  0.02,  0.10,  0.62, 3.1415 * 0, 0x11);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTL2, -0.02, -0.10,  0.58, 3.1415 * 0, 0x11);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDR,  0.10,  0.13,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDL, -0.10, -0.13,  0.10, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HEAD,  0.04,  0.00,  0.33, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_BODY, -0.10,  0.00,  0.49, 3.1415 * 0, 0x01);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTR2,  0.02,  0.10,  0.62, 3.1415 * 0, 0x11);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTL2, -0.02, -0.10,  0.62, 3.1415 * 0, 0x11);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDR, -0.05,  0.13,  0.12, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDL,  0.05, -0.13,  0.12, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HEAD,  0.04,  0.00,  0.31, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_BODY, -0.10,  0.00,  0.47, 3.1415 * 0, 0x01);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTR2,  0.02,  0.10,  0.58, 3.1415 * 0, 0x11);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTL2, -0.02, -0.10,  0.62, 3.1415 * 0, 0x11);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDR, -0.10,  0.13,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDL,  0.10, -0.13,  0.10, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HEAD,  0.04,  0.00,  0.33, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_BODY, -0.10,  0.00,  0.49, 3.1415 * 0, 0x01);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTR2,  0.02,  0.10,  0.62, 3.1415 * 0, 0x11);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTL2, -0.02, -0.10,  0.62, 3.1415 * 0, 0x11);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDR,  0.05,  0.13,  0.12, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDL, -0.05, -0.13,  0.12, 3.1415 * 0, 0x00);
}

// クマ系キャラの作成
static void createKumaData(struct hknwChara *this){
	this->partsNum = 12;
	this->posesNum = 6 * 5;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD,  0.00,  0.00,  0.00,  0,   0, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.05,  0.10,  0.10,  0,  96, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.05, -0.10,  0.10,  0,  96, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD,  0.10,  0.00, -0.02,  0, 112, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.00,  0,  32, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_BODY, -0.15,  0.00,  0.10,  0, 128, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.00,  0.00,  0,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00,  0.00,  0.00,  0,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR2,  0.00,  0.00,  0.00, 64,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL2,  0.00,  0.00,  0.00, 64,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDR,  0.00,  0.00,  0.00,  0,  80, 16, 0x01);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDL,  0.00,  0.00,  0.00,  0,  80, 16, 0x11);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.03,  0.00,  0.45, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_BODY, -0.02,  0.00,  0.27, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTR1,  0.02,  0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTL1, -0.02, -0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDR, -0.02,  0.20,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.02, -0.20,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.09,  0.00,  0.38, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTR1,  0.10,  0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTL2, -0.20, -0.07,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDR, -0.10,  0.15,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.10, -0.15,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HEAD,  0.09,  0.00,  0.40, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.26, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00, -0.07,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDR, -0.05,  0.18,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDL,  0.05, -0.18,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HEAD,  0.09,  0.00,  0.38, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTR2, -0.20,  0.07,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTL1,  0.10, -0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDR,  0.10,  0.15,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDL, -0.10, -0.15,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HEAD,  0.09,  0.00,  0.40, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.26, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.07,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00, -0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDR,  0.05,  0.18,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDL, -0.05, -0.18,  0.25, 3.1415 * 0, 0x00);
}

// たまご系キャラの作成
static void createBearData(struct hknwChara *this){
	this->partsNum = 10;
	this->posesNum = 5 * 5;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.00,  0,   0, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_BODY,  0.18,  0.00,  0.10,  0,  32, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.13,  0.24,  0,  46, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_BODY,  0.00, -0.13,  0.24,  0,  46, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.00,  0.00,  0,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00,  0.00,  0.00,  0,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR2,  0.00,  0.00,  0.00, 64,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTL2,  0.00,  0.00,  0.00, 64,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDR,  0.00,  0.00,  0.00,  0,  80, 16, 0x01);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDL,  0.00,  0.00,  0.00,  0,  80, 16, 0x11);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_BODY, -0.02,  0.00,  0.32, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTR1,  0.02,  0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTL1, -0.02, -0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDR, -0.02,  0.20,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.02, -0.20,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.28, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTR1,  0.10,  0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTL2, -0.20, -0.07,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDR, -0.10,  0.15,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.10, -0.15,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.31, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00, -0.07,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDR, -0.05,  0.18,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDL,  0.05, -0.18,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.28, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTR2, -0.20,  0.07,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_FTL1,  0.10, -0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDR,  0.10,  0.15,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_HUMAN_HNDL, -0.10, -0.15,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.31, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.07,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_FTL1,  0.00, -0.07,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDR,  0.05,  0.18,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_HUMAN_HNDL, -0.05, -0.18,  0.25, 3.1415 * 0, 0x00);
}

// ポンコツロボット系キャラの作成
static void createRoboData(struct hknwChara *this){
	this->partsNum = 9;
	this->posesNum = 6 * 3;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD,  0.00,  0.00,  0.00,  0,   0, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.06,  0.20, -0.02,  0,  96, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.06, -0.20, -0.02,  0,  96, 32, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.19,  0.00, -0.09,  0, 128, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HEAD, -0.16,  0.00,  0.08,  0, 160, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.00,  0,  32, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_FTR1,  0.00,  0.00,  0.00,  0,  64, 16, 0x02);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDR,  0.00,  0.00,  0.00,  0,  80, 16, 0x01);
	setParts(ptrParts++, HKNWCHARAPARTSID_HUMAN_HNDL,  0.00,  0.00,  0.00,  0,  80, 16, 0x11);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.00,  0.00,  0.44, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_BODY, -0.02,  0.00,  0.19, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTR1,  0.02,  0.15,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_FTR1, -0.02, -0.15,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDR, -0.02,  0.17,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_HUMAN_HNDL,  0.02, -0.17,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.37, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTR1,  0.02,  0.15,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_FTR1, -0.02, -0.15,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDR, -0.12,  0.12,  0.25, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_HUMAN_HNDL, -0.12, -0.12,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HEAD,  0.12,  0.00,  0.39, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_BODY,  0.00,  0.00,  0.18, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTR1,  0.02,  0.15,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_FTR1, -0.02, -0.15,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDR, -0.12,  0.12,  0.27, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_HUMAN_HNDL, -0.12, -0.12,  0.27, 3.1415 * 0, 0x00);
}

// ドラゴン系キャラの作成
static void createDragonData(struct hknwChara *this){
	this->partsNum = 8;
	this->posesNum = 7 * 9;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_HED1,  0.00,  0.00,  0.00,  0,   0, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_HED2,  0.00,  0.00,  0.00,  0,  32, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_BODY,  0.00,  0.00,  0.00,  0,  64, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_TAIL,  0.00,  0.00,  0.00,  0,  96, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_FTFR,  0.00,  0.00,  0.00,  0, 128, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_FTFL,  0.00,  0.00,  0.00,  0, 128, 32, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_FTBR,  0.00,  0.00,  0.00,  0, 160, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_FTBL,  0.00,  0.00,  0.00,  0, 160, 32, 0x10);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_DRAGON_HED1,  0.15,  0.00,  0.48, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_DRAGON_BODY,  0.00,  0.00,  0.30, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_DRAGON_TAIL, -0.18,  0.00,  0.21, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_DRAGON_FTFR,  0.10,  0.19,  0.18, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_DRAGON_FTFL,  0.10, -0.19,  0.18, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_DRAGON_FTBR, -0.20,  0.14,  0.14, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_DRAGON_FTBL, -0.20, -0.14,  0.14, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_DRAGON_HED1,  0.23,  0.00,  0.43, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_DRAGON_BODY,  0.00,  0.00,  0.28, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_DRAGON_TAIL, -0.18,  0.00,  0.19, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_DRAGON_FTFR,  0.15,  0.15,  0.18, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_DRAGON_FTFL,  0.05, -0.15,  0.18, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_DRAGON_FTBR, -0.25,  0.11,  0.14, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_DRAGON_FTBL, -0.15, -0.11,  0.14, 3.1415 * 0, 0x00);
                                                                                                                      
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_DRAGON_HED1,  0.23,  0.00,  0.44, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_DRAGON_BODY,  0.00,  0.00,  0.29, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_DRAGON_TAIL, -0.18,  0.00,  0.21, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_DRAGON_FTFR,  0.12,  0.17,  0.18, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_DRAGON_FTFL,  0.09, -0.17,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_DRAGON_FTBR, -0.22,  0.12,  0.19, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_DRAGON_FTBL, -0.19, -0.12,  0.14, 3.1415 * 0, 0x00);
                                                                                                                      
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_DRAGON_HED1,  0.23,  0.00,  0.45, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_DRAGON_BODY,  0.00,  0.00,  0.30, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_DRAGON_TAIL, -0.18,  0.00,  0.22, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_DRAGON_FTFR,  0.10,  0.18,  0.18, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_DRAGON_FTFL,  0.10, -0.18,  0.26, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_DRAGON_FTBR, -0.20,  0.13,  0.22, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_DRAGON_FTBL, -0.20, -0.13,  0.14, 3.1415 * 0, 0x00);
                                                                                                                      
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_DRAGON_HED1,  0.23,  0.00,  0.44, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_DRAGON_BODY,  0.00,  0.00,  0.29, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_DRAGON_TAIL, -0.18,  0.00,  0.21, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_DRAGON_FTFR,  0.09,  0.17,  0.18, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_DRAGON_FTFL,  0.12, -0.17,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_DRAGON_FTBR, -0.19,  0.12,  0.19, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_DRAGON_FTBL, -0.21, -0.12,  0.14, 3.1415 * 0, 0x00);
                                                                                                                      
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 4, HKNWCHARAPARTSID_DRAGON_HED1,  0.23,  0.00,  0.43, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 4, HKNWCHARAPARTSID_DRAGON_BODY,  0.00,  0.00,  0.28, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 4, HKNWCHARAPARTSID_DRAGON_TAIL, -0.18,  0.00,  0.19, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 4, HKNWCHARAPARTSID_DRAGON_FTFR,  0.05,  0.15,  0.18, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 4, HKNWCHARAPARTSID_DRAGON_FTFL,  0.15, -0.15,  0.18, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 4, HKNWCHARAPARTSID_DRAGON_FTBR, -0.15,  0.11,  0.14, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 4, HKNWCHARAPARTSID_DRAGON_FTBL, -0.25, -0.11,  0.14, 3.1415 * 0, 0x00);
                                                                                                                      
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 5, HKNWCHARAPARTSID_DRAGON_HED1,  0.23,  0.00,  0.44, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 5, HKNWCHARAPARTSID_DRAGON_BODY,  0.00,  0.00,  0.29, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 5, HKNWCHARAPARTSID_DRAGON_TAIL, -0.18,  0.00,  0.21, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 5, HKNWCHARAPARTSID_DRAGON_FTFR,  0.09,  0.17,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 5, HKNWCHARAPARTSID_DRAGON_FTFL,  0.12, -0.17,  0.18, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 5, HKNWCHARAPARTSID_DRAGON_FTBR, -0.19,  0.12,  0.14, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 5, HKNWCHARAPARTSID_DRAGON_FTBL, -0.21, -0.12,  0.19, 3.1415 * 0, 0x00);
                                                                                                                      
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 6, HKNWCHARAPARTSID_DRAGON_HED1,  0.23,  0.00,  0.45, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 6, HKNWCHARAPARTSID_DRAGON_BODY,  0.00,  0.00,  0.30, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 6, HKNWCHARAPARTSID_DRAGON_TAIL, -0.18,  0.00,  0.22, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 6, HKNWCHARAPARTSID_DRAGON_FTFR,  0.10,  0.18,  0.26, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 6, HKNWCHARAPARTSID_DRAGON_FTFL,  0.10, -0.18,  0.18, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 6, HKNWCHARAPARTSID_DRAGON_FTBR, -0.20,  0.13,  0.14, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 6, HKNWCHARAPARTSID_DRAGON_FTBL, -0.20, -0.13,  0.22, 3.1415 * 0, 0x00);
                                                                                                                      
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 7, HKNWCHARAPARTSID_DRAGON_HED1,  0.23,  0.00,  0.44, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 7, HKNWCHARAPARTSID_DRAGON_BODY,  0.00,  0.00,  0.29, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 7, HKNWCHARAPARTSID_DRAGON_TAIL, -0.18,  0.00,  0.21, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 7, HKNWCHARAPARTSID_DRAGON_FTFR,  0.12,  0.17,  0.23, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 7, HKNWCHARAPARTSID_DRAGON_FTFL,  0.09, -0.17,  0.18, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 7, HKNWCHARAPARTSID_DRAGON_FTBR, -0.22,  0.12,  0.14, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 7, HKNWCHARAPARTSID_DRAGON_FTBL, -0.19, -0.12,  0.19, 3.1415 * 0, 0x00);
}

// ブタ系キャラの作成
static void createButaData(struct hknwChara *this){
	this->partsNum = 7;
	this->posesNum = 6 * 5;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_HED1,  0.00,  0.00,  0.00,  0,   0, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_NOSE,  0.00,  0.00,  0.00,  0,  32, 32, 0x02);
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_FTFR,  0.00,  0.00,  0.00,  0,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_FTFL,  0.00,  0.00,  0.00,  0,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_FTBR,  0.00,  0.00,  0.00,  0,  64, 16, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_FTBL,  0.00,  0.00,  0.00,  0,  64, 16, 0x10);
	setParts(ptrParts++, HKNWCHARAPARTSID_DRAGON_FOOT,  0.00,  0.00,  0.00, 64,  64, 16, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_DRAGON_HED1,  0.00,  0.00,  0.29, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_DRAGON_NOSE,  0.20,  0.00,  0.29, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_DRAGON_FTFR,  0.12,  0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_DRAGON_FTFL,  0.12, -0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_DRAGON_FTBR, -0.12,  0.12,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_DRAGON_FTBL, -0.12, -0.12,  0.10, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_DRAGON_HED1,  0.00,  0.00,  0.29, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_DRAGON_NOSE,  0.20,  0.00,  0.29, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_DRAGON_FTFR,  0.19,  0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_DRAGON_FOOT,  0.09, -0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_DRAGON_FTBR, -0.13,  0.12,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_DRAGON_FTBL, -0.11, -0.12,  0.15, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_DRAGON_HED1,  0.00,  0.00,  0.27, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_DRAGON_NOSE,  0.20,  0.00,  0.27, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_DRAGON_FTFR,  0.11,  0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_DRAGON_FTFL,  0.13, -0.10,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_DRAGON_FOOT, -0.19,  0.12,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_DRAGON_FTBL, -0.09, -0.12,  0.10, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_DRAGON_HED1,  0.00,  0.00,  0.29, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_DRAGON_NOSE,  0.20,  0.00,  0.29, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_DRAGON_FOOT,  0.09,  0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_DRAGON_FTFL,  0.19, -0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_DRAGON_FTBR, -0.11,  0.12,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_DRAGON_FTBL, -0.13, -0.12,  0.10, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_DRAGON_HED1,  0.00,  0.00,  0.27, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_DRAGON_NOSE,  0.20,  0.00,  0.27, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_DRAGON_FTFR,  0.13,  0.10,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_DRAGON_FTFL,  0.11, -0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_DRAGON_FTBR, -0.09,  0.12,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_DRAGON_FOOT, -0.19, -0.12,  0.10, 3.1415 * 0, 0x00);
}

// カート系キャラの作成
static void createCartData(struct hknwChara *this){
	this->partsNum = 3;
	this->posesNum = 6 * 3;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_CART_BODY,  0.00,  0.00,  0.00,  0,   0, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_CART_BEAK,  0.00,  0.00,  0.00,  0,  32, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_CART_TIRE,  0.00,  0.00,  0.00,  0,  64, 16, 0x02);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_CART_BODY,  0.00,  0.00,  0.22, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_CART_BEAK,  0.15,  0.00,  0.17, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_CART_TIRE,  0.12, -0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_CART_TIRE,  0.12,  0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_CART_TIRE, -0.12, -0.12,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_CART_TIRE, -0.12,  0.12,  0.10, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_CART_BODY,  0.00,  0.00,  0.22, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_CART_BEAK,  0.15,  0.00,  0.17, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_CART_TIRE,  0.12, -0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_CART_TIRE,  0.12,  0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_CART_TIRE, -0.12, -0.12,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_CART_TIRE, -0.12,  0.12,  0.10, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_CART_BODY,  0.00,  0.00,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_CART_BEAK,  0.15,  0.00,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_CART_TIRE,  0.12, -0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_CART_TIRE,  0.12,  0.10,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_CART_TIRE, -0.12, -0.12,  0.10, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_CART_TIRE, -0.12,  0.12,  0.10, 3.1415 * 0, 0x00);
}

// ロケット系キャラの作成
static void createRocketData(struct hknwChara *this){
	this->partsNum = 2;
	this->posesNum = 5 * 5;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_CART_BODY,  0.00,  0.00,  0.00,  0,   0, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_CART_TIRE,  0.00,  0.00,  0.00,  0,  32, 32, 0x01);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_CART_BODY,  0.00,  0.00,  0.30, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_CART_TIRE,  0.15,  0.15,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_CART_TIRE,  0.15, -0.15,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_CART_TIRE, -0.15,  0.15,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_CART_TIRE, -0.15, -0.15,  0.15, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_CART_BODY,  0.00,  0.00,  0.31, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_CART_TIRE,  0.22,  0.15,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_CART_TIRE,  0.12, -0.15,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_CART_TIRE, -0.16,  0.15,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_CART_TIRE, -0.14, -0.15,  0.20, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_CART_BODY,  0.00,  0.00,  0.29, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_CART_TIRE,  0.14,  0.15,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_CART_TIRE,  0.16, -0.15,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_CART_TIRE, -0.22,  0.15,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_CART_TIRE, -0.12, -0.15,  0.15, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_CART_BODY,  0.00,  0.00,  0.31, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_CART_TIRE,  0.12,  0.15,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_CART_TIRE,  0.22, -0.15,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_CART_TIRE, -0.14,  0.15,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_CART_TIRE, -0.16, -0.15,  0.15, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_CART_BODY,  0.00,  0.00,  0.29, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_CART_TIRE,  0.16,  0.15,  0.20, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_CART_TIRE,  0.14, -0.15,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_CART_TIRE, -0.12,  0.15,  0.15, 3.1415 * 0, 0x00);
	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_CART_TIRE, -0.22, -0.15,  0.15, 3.1415 * 0, 0x00);
}

// ボール系キャラの作成
static void createBallData(struct hknwChara *this){
	this->partsNum = 2;
	this->posesNum = 1 * 5;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_BALL_BOD1,  0.00,  0.00,  0.00,  0,   0, 32, 0x00);
	setParts(ptrParts++, HKNWCHARAPARTSID_BALL_BOD2,  0.00,  0.00,  0.00,  0,  32, 32, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_BALL_BOD1,  0.00,  0.00,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_BALL_BOD1,  0.00,  0.00,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_BALL_BOD2,  0.00,  0.00,  0.25, 3.1415 * 0, 0x11);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_BALL_BOD1,  0.00,  0.00,  0.25, 3.1415 * 0, 0x11);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_BALL_BOD2,  0.00,  0.00,  0.25, 3.1415 * 0, 0x00);
}

// クッキー系キャラの作成
static void createCookieData(struct hknwChara *this){
	this->partsNum = 4;
	this->posesNum = 1 * 5;
	struct hknwCharaParts *ptrParts = this->parts = (struct hknwCharaParts*)engineUtilMemoryCalloc(this->partsNum, sizeof(struct hknwCharaParts));
	struct hknwCharaPoses *ptrPoses = this->poses = (struct hknwCharaPoses*)engineUtilMemoryCalloc(this->posesNum, sizeof(struct hknwCharaPoses));
	setParts(ptrParts++, HKNWCHARAPARTSID_BALL_BOD1,  0.00,  0.00,  0.00,  0,   0, 32, 0x02);
	setParts(ptrParts++, HKNWCHARAPARTSID_BALL_BOD2,  0.00,  0.00,  0.00,  0,  32, 32, 0x02);
	setParts(ptrParts++, HKNWCHARAPARTSID_BALL_BOD3,  0.00,  0.00,  0.00,  0,  64, 32, 0x02);
	setParts(ptrParts++, HKNWCHARAPARTSID_BALL_BOD4,  0.00,  0.00,  0.00,  0,  96, 32, 0x02);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_STAND, 0, HKNWCHARAPARTSID_BALL_BOD1,  0.00,  0.00,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 0, HKNWCHARAPARTSID_BALL_BOD1,  0.00,  0.00,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 1, HKNWCHARAPARTSID_BALL_BOD2,  0.00,  0.00,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 2, HKNWCHARAPARTSID_BALL_BOD3,  0.00,  0.00,  0.25, 3.1415 * 0, 0x00);

	setPoses(ptrPoses++, HKNWCHARAPOSESID_WALK, 3, HKNWCHARAPARTSID_BALL_BOD4,  0.00,  0.00,  0.25, 3.1415 * 0, 0x00);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 初期化
struct hknwChara *hknwCharaInit(enum hknwCharaKind charaKind){
	struct hknwChara *this = (struct hknwChara*)engineUtilMemoryCalloc(1, sizeof(struct hknwChara));
	this->imgw = TEXSIZ_PUPPET_W;
	this->imgh = TEXSIZ_PUPPET_H;
	this->shadow.texx = 0;
	this->shadow.texy = 256;
	this->shadow.texs = 32;
	switch(charaKind){
		case HKNWCHARAKIND_HUMAN: createHumanData(this); break;
		case HKNWCHARAKIND_HUMAN2: createHuman2Data(this); break;
		case HKNWCHARAKIND_HUMAN3: createHuman3Data(this); break;
		case HKNWCHARAKIND_GUDAGUDA: createGudagudaData(this); break;
		case HKNWCHARAKIND_HANDSTAND: createHandstandData(this); break;
		case HKNWCHARAKIND_KUMA: createKumaData(this); break;
		case HKNWCHARAKIND_BEAR: createBearData(this); break;
		case HKNWCHARAKIND_ROBO: createRoboData(this); break;
		case HKNWCHARAKIND_DRAGON: createDragonData(this); break;
		case HKNWCHARAKIND_BUTA: createButaData(this); break;
		case HKNWCHARAKIND_CART: createCartData(this); break;
		case HKNWCHARAKIND_ROCKET: createRocketData(this); break;
		case HKNWCHARAKIND_BALL: createBallData(this); break;
		case HKNWCHARAKIND_COOKIE: createCookieData(this); break;
	}
	this->faceNum = 0;
	return this;
}


// 配列にハコニワキャラクタの描画情報を追加
void hknwCharaCreateArray(struct hknwChara *this){
	int w = this->imgw;
	int h = this->imgh;

	this->faceIndex = engineGraphicBufferFaceIndexGet();
	this->faceNum = (this->partsNum * 4 + 1) * 2;

	// パーツのテクスチャ座標データを作成
	for(int i = 0; i < this->partsNum; i++){
		struct hknwCharaParts *parts = this->parts + i;
		int s = parts->texs;
		double vs = s / 128.0;
		for(int j = 0; j < 4; j++){
			engineGraphicBufferPushTetraFace(engineGraphicBufferVretIndexGet());
			engineGraphicBufferPushTetraVert(-vs, -vs, vs * 2, vs * 2);
			engineGraphicBufferPushTetraTexc(w, h, parts->texx + s * j, parts->texy + s, s, -s);
		}
	}

	// 影のテクスチャ座標データを作成
	int s = this->shadow.texs;
	double vs = s / 128.0;
	engineGraphicBufferPushTetraFace(engineGraphicBufferVretIndexGet());
	engineGraphicBufferPushVert(-vs, 0.0, -vs);
	engineGraphicBufferPushVert(-vs, 0.0,  vs);
	engineGraphicBufferPushVert( vs, 0.0,  vs);
	engineGraphicBufferPushVert( vs, 0.0, -vs);
	engineGraphicBufferPushTetraTexc(w, h, this->shadow.texx, this->shadow.texy, s, s);
}

// ポーズのステップ数確認
int hknwCharaGetPoseStepNum(struct hknwChara *this, enum hknwCharaPosesId poseId){
	int stepNum = 0;
	for(int i = 0; i < this->posesNum; i++){
		if(this->poses[i].posesId == poseId && stepNum < this->poses[i].stepIndex){
			stepNum = this->poses[i].stepIndex;
		}
	}
	return stepNum + 1;
}

// キャラクタの描画
void hknwCharaDrawChara(struct hknwChara *this, struct engineMathMatrix44 *mat, double rotv, double roth, double x, double y, double z, double r, double s, enum hknwCharaPosesId poseId, int step){
	if(this->faceNum <= 0){return;}

	struct engineMathMatrix44 tempmat1;
	struct engineMathMatrix44 tempmat2;

	// 行列作成
	engineMathMat4Copy(&tempmat1, mat);
	engineMathMat4Translate(&tempmat1, x, z, y);
	engineMathMat4RotateY(&tempmat1, -r);
	engineMathMat4Scale(&tempmat1, s, s, s);
	
	for(int i = 0; i < this->posesNum; i++){
		if(this->poses[i].posesId == poseId && step == this->poses[i].stepIndex){
			double x0 = this->poses[i].x;
			double y0 = this->poses[i].y;
			double z0 = this->poses[i].z;
			double r0 = this->poses[i].r;
			int type0 = this->poses[i].type;

			// テクスチャ垂直軸角度フレーム
			int anglev0 = (int)engineMathRound(1 + (180 / 3.1415 * (rotv - r - r0)) / 90) % 4;
			while(anglev0 < 0){anglev0 += 4;};
			// テクスチャ垂直軸角度フレーム タイヤ用
			int anglev1 = (int)engineMathRound(2 - (180 / 3.1415 * (rotv - r - r0)) / 45) % 4;
			while(anglev1 < 0){anglev1 += 4;};

			for(int j = 0; j < this->partsNum; j++){
				if(this->poses[i].partsId == this->parts[j].partsId){
					double x1 = this->parts[j].x;
					double y1 = this->parts[j].y;
					double z1 = this->parts[j].z;
					int type1 = this->parts[j].type;

					int yswap = 1;
					int zswap = 1;
					int avIdx = anglev0;
					if(!!(type0 & 0x01)){
						// パーツセットの上下反転
						zswap *= -1;
						z1 *= -1;
					}
					if(!!(type0 & 0x10)){
						// パーツセットの前後反転
						avIdx = (6 - avIdx) % 4;
						yswap *= -1;
						x1 *= -1;
					}
					if(!!(type1 & 0x10)){
						// パーツの左右反転
						avIdx = (4 - avIdx) % 4;
						yswap *= -1;
					}
					// 単フレーム画像確認
					if(!!(type1 & 0x01)){avIdx = 0;}
					if(!!(type1 & 0x02)){avIdx = anglev1;}

					// 行列作成
					engineMathMat4Copy(&tempmat2, &tempmat1);
					engineMathMat4Translate(&tempmat2, x0, z0, y0);
					engineMathMat4RotateY(&tempmat2, -r0);
					engineMathMat4Translate(&tempmat2, x1, z1, y1);
					engineMathMat4RotateY(&tempmat2, -rotv + r + r0);
					engineMathMat4RotateX(&tempmat2, -roth);
					engineMathMat4Scale(&tempmat2, yswap, zswap, 1);
					engineGraphicEngineSetMatrix(&tempmat2);

					engineGraphicEngineDrawIndex((this->faceIndex + (j * 4 + avIdx) * 2) * 3, 2 * 3);
				}
			}
		}
	}
}

// 影の描画
void hknwCharaDrawShadow(struct hknwChara *this, struct engineMathMatrix44 *mat, double x, double y, double z, double s){
	if(this->faceNum <= 0){return;}

	struct engineMathMatrix44 tempmat1;
	engineMathMat4Copy(&tempmat1, mat);
	engineMathMat4Translate(&tempmat1, x, z + 0.01, y);
	engineMathMat4Scale(&tempmat1, s, s, s);
	engineGraphicEngineSetMatrix(&tempmat1);
	engineGraphicEngineSetColorRgba(1.0, 1.0, 1.0, 1.0);
	engineGraphicEngineDrawIndex((this->faceIndex + (this->faceNum - 2)) * 3, 2 * 3);
}

// 破棄
void hknwCharaDispose(struct hknwChara *this){
	engineUtilMemoryFree(this->parts);
	engineUtilMemoryFree(this->poses);
	engineUtilMemoryFree(this);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

