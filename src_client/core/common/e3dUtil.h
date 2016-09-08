#ifndef __engine3d_h_
#define __engine3d_h_

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// システム画像
extern engineGraphicObjectTexId e3dIdSystemImageLinear;
extern engineGraphicObjectTexId e3dIdSystemImageNearest;

// 行列データ保管場所
extern struct engineMathMatrix44 e3dMatOrtho;
extern struct engineMathMatrix44 e3dMatFrustum;

// ----------------------------------------------------------------

void e3dUtilGameInit();
void e3dUtilGameFinal();

// ----------------------------------------------------------------

// 描画構造体
struct e3dImage{
	int faceIndex;
	int faceNum;
};

// 配列に画像の描画情報を追加
void e3dImageCreateArray(struct e3dImage *this, int imgw, int imgh, double x, double y, double w, double h, int tu, int tv, int tw, int th);
#define e3dImageWhiteCreateArray(this, x, y, w, h) e3dImageCreateArray(this, TEXSIZ_SYSTEM_W, TEXSIZ_SYSTEM_H, x, y, w, h, 2, 2, 28, 28)

// 配列に箱の描画情報を追加
void e3dImageBoxCreateArray(struct e3dImage *this, double x, double y, double w, double h, int tu, int tv, int tw, int th);
#define e3dImageBoxDefaultCreateArray(this, x, y, w, h) e3dImageBoxCreateArray(this, x, y, w, h, TEXPOS_SYSTEM_BOXBASIC_X, TEXPOS_SYSTEM_BOXBASIC_Y, TEXPOS_SYSTEM_BOXBASIC_W, TEXPOS_SYSTEM_BOXBASIC_H)

// 配列に文字列(utf8)の描画情報を追加
void e3dImageTextCreateArray(struct e3dImage *this, double x, double y, char *text, double scale, int xalign, int yalign);

// 描画
void e3dImageDraw(struct e3dImage *this);

// ----------------------------------------------------------------

// 数列構造体
struct e3dNumber{
	int faceIndex;
	double width;
	int xalign;
};

// 配列に数列の描画情報を追加
void e3dNumberCreateArray(struct e3dNumber *this, double x, double y, int tu, int tv, int tw, int th, double scale, int xalign, int yalign);
#define e3dNumberDefaultCreateArray(this, x, y, scale, xalign, yalign) e3dNumberCreateArray(this, x, y, TEXPOS_SYSTEM_FONTABCD_X, TEXPOS_SYSTEM_FONTABCD_Y + 30, 20 * 10, 30, scale, xalign, yalign)
#define e3dNumberOutlineCreateArray(this, x, y, scale, xalign, yalign) e3dNumberCreateArray(this, x, y, TEXPOS_SYSTEM_FONTNUMBER_X, TEXPOS_SYSTEM_FONTNUMBER_Y, 20 * 10, 30, scale, xalign, yalign)

// 描画
void e3dNumberDraw(struct e3dNumber *this, struct engineMathMatrix44 *mat, int number);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#endif

