#ifndef __storage_h_
#define __storage_h_

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 構造体保存処理

void storageSet(char *key, void *data, int length);
int storageGet(char *key, void *data, int length);
void storageCommit();

void storageGameCalc();
void storageGamePause();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ゲーム静的データ

void dataPuppetActive(bool active);
struct dataPuppet *dataPuppetGet(int puppetKind, int *e3dIdImage);

void dataMeisterActive(bool active);
struct dataMeister *dataMeisterGet(int meisterKind, int *e3dIdImage);

void dataGachaActive(bool active);
struct dataGacha *dataGachaGet(int gachaKind, int *e3dIdImage);

void dataFieldActive(bool active);
struct dataField *dataFieldGet(int fieldKind, int *e3dIdImage);

void dataEnemyActive(bool active);
struct dataEnemy *dataEnemyGet(int enemyKind, int *e3dIdImage);

// ----------------------------------------------------------------
// ゲーム静的描画データ

void dataBackgroundActive(bool active);
void dataBackgroundPreset(int backgroundKind);
void dataBackgroundDraw(int backgroundKind);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ゲーム動的データ

void storageStatusVersion(unsigned char major, unsigned char miner);
void storageStatusActive(bool active);
struct storageStatus0101 *storageStatusGet();
void storageStatusUpdate();
void storageStatusWrite();

void storagePuppetVersion(unsigned char major, unsigned char miner);
void storagePuppetActive(bool active);
struct storagePuppet0001 *storagePuppetGet();
void storagePuppetUpdate();
void storagePuppetWrite();

void storageMeisterVersion(unsigned char major, unsigned char miner);
void storageMeisterActive(bool active);
struct storageMeister0001 *storageMeisterGet();
void storageMeisterUpdate();
void storageMeisterWrite();

void storageAtelierVersion(unsigned char major, unsigned char miner);
void storageAtelierActive(bool active);
struct storageAtelier0001 *storageAtelierGet();
void storageAtelierUpdate();
void storageAtelierWrite();

void storageFieldVersion(unsigned char major, unsigned char miner);
void storageFieldActive(bool active);
struct storageField0001 *storageFieldGet();
void storageFieldUpdate();
void storageFieldWrite();

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 設定関連

void settingGameInit();

// ----------------------------------------------------------------
// 音量設定

int settingBgmVolumeGet();
void settingBgmVolumeSet(int type);

int settingSeVolumeGet();
void settingSeVolumeSet(int type);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

#endif

