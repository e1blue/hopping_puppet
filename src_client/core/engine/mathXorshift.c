#include "engine.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static uint64_t x = 123456789;
static uint64_t y = 362436069;
static uint64_t z = 521288629;
static uint64_t w = 88675123;

// 乱数初期化
void mathRandomInit(uint64_t seed){
	w ^= seed;
}

// 乱数獲得
uint64_t mathRandomGet(){
	uint64_t t = (x ^ (x << 11));
	x = y;
	y = z;
	z = w;
	w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	return w;
}

// 乱数種取得
void mathRandomGetSeed(uint64_t seed[]){
	seed[0] = x;
	seed[1] = y;
	seed[2] = z;
	seed[3] = w;
}

// 乱数種設定
void mathRandomSetSeed(uint64_t seed[]){
	x = seed[0];
	y = seed[1];
	z = seed[2];
	w = seed[3];
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

