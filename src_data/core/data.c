#include <string.h>
#include <stdlib.h>
#include "data.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 揮発性一時バッファ 返値領域は解放禁止
static void *temporaryBuffer(size_t size){
	static void *tempBuff = NULL;
	static size_t tempBuffSize = 0;
	if(tempBuffSize < size){
		if(tempBuffSize > 0){free(tempBuff);}
		tempBuffSize = size;
		tempBuff = malloc(tempBuffSize);
	}
	return tempBuff;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#ifdef __TRIAL__
static char localGlobal_enc[0x41] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
#else
static char localGlobal_enc[0x41] = "Awt0zEH2qfKFS/lcsQhaXDYZJVkr3CO74e+1uvix8LNRbMU59dnPp6GTIogmyjBW=";
#endif
static char localGlobal_dec[0x80] = "";

// base64形式に変換 返値文字列は揮発性バッファで解放禁止
char *base64encode(unsigned char *data, int length){
	int rawLen = length;
	int b64Len = ((rawLen + 2) / 3) * 4;
	size_t buffSize = (b64Len + 1) * sizeof(unsigned char);
	unsigned char *buff = (unsigned char*)temporaryBuffer(buffSize);

	// base64エンコード
	int x = 0;
	int i = 0;
	int j = 0;
	while(i < rawLen){
		x = (x << 8) | data[i++];
		switch(i % 3){
			case 1: buff[j++] = localGlobal_enc[(x >> 2) & 0x3f]; break;
			case 2: buff[j++] = localGlobal_enc[(x >> 4) & 0x3f]; break;
			case 0: buff[j++] = localGlobal_enc[(x >> 6) & 0x3f]; buff[j++] = localGlobal_enc[x & 0x3f]; break;
		}
	}
	switch(i % 3){
		case 1: buff[j++] = localGlobal_enc[(x << 4) & 0x3f]; buff[j++] = localGlobal_enc[0x40]; buff[j++] = localGlobal_enc[0x40]; break;
		case 2: buff[j++] = localGlobal_enc[(x << 2) & 0x3f]; buff[j++] = localGlobal_enc[0x40]; break;
		case 0: break;
	}
	buff[j++] = '\0';

	return (char*)buff;
}

// base64形式から解読 返値データは揮発性バッファで解放禁止
unsigned char *base64decodeChar(char *data, int *length){
	int b64Len = (data == NULL) ? 0 : (int)strlen(data);

	int rawLen = (b64Len / 4) * 3;
	if(rawLen > 0 && data[b64Len - 1] == localGlobal_enc[0x40]){rawLen -= 1;}
	if(rawLen > 0 && data[b64Len - 2] == localGlobal_enc[0x40]){rawLen -= 1;}
	if(length != NULL){*length = rawLen;}

	// データが存在しなかった場合
	if(rawLen <= 0){return NULL;}

	return base64decode((unsigned char*)data, rawLen);
}
unsigned char *base64decode(unsigned char *data, int length){
	int rawLen = length;
	size_t buffSize = (rawLen + 1) * sizeof(unsigned char);
	unsigned char *buff = (unsigned char*)temporaryBuffer(buffSize);

	// デコードテーブル作成
	for(int i = 0; i < 0x41; i++){localGlobal_dec[localGlobal_enc[i]] = i % 0x40;}

	// base64デコード
	for(int i = 0, j = 0; i < rawLen; i++){
		switch(i % 3){
			case 0: buff[i] = localGlobal_dec[data[j + 0]] << 2 | localGlobal_dec[data[j + 1]] >> 4; break;
			case 1: buff[i] = localGlobal_dec[data[j + 1]] << 4 | localGlobal_dec[data[j + 2]] >> 2; break;
			case 2: buff[i] = localGlobal_dec[data[j + 2]] << 6 | localGlobal_dec[data[j + 3]]; j += 4; break;
		}
	}
	buff[rawLen] = '\0';

	return buff;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

