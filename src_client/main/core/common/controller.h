#ifndef __controller_h_
#define __controller_h_

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// コントローラボタン

extern bool ctrlBtnKup;
extern bool ctrlBtnKdn;
extern bool ctrlBtnKrt;
extern bool ctrlBtnKlt;

void ctrlBtnArrowCreateArray(int *faceIndex);
void ctrlBtnCalc();
void ctrlBtnDraw(int faceIndex);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#endif

