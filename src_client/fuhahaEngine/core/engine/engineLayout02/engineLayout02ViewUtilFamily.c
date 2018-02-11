#include "../../library.h"
#include "../../engine/engineLayout02/engineLayout02.h"

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static struct{
	bool isViewRootInit;
	struct engineLayout02View viewRoot;
} localGlobal = {0};

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// 表示要素構造体子要素 全ての要素の親となる要素を取得
static struct engineLayout02View *viewRootGet(){
	if(!localGlobal.isViewRootInit){
		localGlobal.isViewRootInit = true;
		engineLayout02ViewUtilPositionSetLtRtTpBm(&localGlobal.viewRoot, 0, 0, 0, 0);
	}
	return &localGlobal.viewRoot;
}

// ----------------------------------------------------------------

// 双方向リストにおいて境界条件を無視するためのダミー設定
static void dummySet(struct engineLayout02View *this, struct engineLayout02View *dummy){
	if(this->family.childrenHead != NULL && this->family.childrenTail != NULL){
		dummy->family.next = this->family.childrenHead;
		dummy->family.prev = this->family.childrenTail;
		dummy->family.next->family.prev = dummy;
		dummy->family.prev->family.next = dummy;
	}else{
		// 要素がない場合
		dummy->family.next = dummy;
		dummy->family.prev = dummy;
	}
}

// ダミー設定を本体に反映
static void dummyUnset(struct engineLayout02View *this, struct engineLayout02View *dummy){
	if(dummy != dummy->family.next && dummy != dummy->family.prev){
		dummy->family.next->family.prev = NULL;
		dummy->family.prev->family.next = NULL;
		this->family.childrenHead = dummy->family.next;
		this->family.childrenTail = dummy->family.prev;
	}else{
		// 要素がない場合
		this->family.childrenHead = NULL;
		this->family.childrenTail = NULL;
	}
}

// ----------------------------------------------------------------

// 子要素追加
static void familyAdd(struct engineLayout02View *this, struct engineLayout02View *child){
	if(this == NULL){return;}
	if(child->family.parent == this){return;}
	if(child == viewRootGet()){return;}

	struct engineLayout02View dummy;
	dummySet(this, &dummy);

	// 子要素の親設定
	child->family.parent = this;

	// childをdummyの前に挿入
	child->family.prev = dummy.family.prev;
	child->family.next = &dummy;
	dummy.family.prev->family.next = child;
	dummy.family.prev = child;

	dummyUnset(this, &dummy);
}

// 子要素除外
static void familyRemove(struct engineLayout02View *this, struct engineLayout02View *child){
	if(this == NULL){return;}
	if(child->family.parent != this){return;}

	struct engineLayout02View dummy;
	dummySet(this, &dummy);

	// childをリストから外す
	child->family.prev->family.next = child->family.next;
	child->family.next->family.prev = child->family.prev;
	child->family.prev = NULL;
	child->family.next = NULL;

	// 子要素の親設定
	child->family.parent = NULL;

	dummyUnset(this, &dummy);
}

// ----------------------------------------------------------------

// 表示要素構造体子要素 初期化
void engineLayout02ViewUtilFamilyInit(struct engineLayout02View *this){
	// ルートに貼り付ける
	familyAdd(viewRootGet(), this);
}

// 表示要素構造体子要素 子要素追加
void engineLayout02ViewUtilFamilyAdd(struct engineLayout02View *this, struct engineLayout02View *child){
	if(child->family.parent == this){return;}
	familyRemove(child->family.parent, child);
	familyAdd(this, child);
}

// 表示要素構造体子要素 子要素除外
void engineLayout02ViewUtilFamilyRemove(struct engineLayout02View *this, struct engineLayout02View *child, bool isDispose){
	if(child->family.parent != this){return;}
	familyRemove(this, child);
	if(isDispose){
		// 子要素破棄
		child->pause(child);
		child->dispose(child);
	}else{
		// ルートに張り替える
		familyAdd(viewRootGet(), child);
	}
}

// 表示要素構造体子要素 破棄
void engineLayout02ViewUtilFamilyDispose(struct engineLayout02View *this){
	// 完全に取り除く
	familyRemove(this->family.parent, this);
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

static int compare(struct engineLayout02View *insert, struct engineLayout02View *find){
	return insert->family.zIndex - find->family.zIndex;
}

// 子要素を並び替える 挿入ソート
static void sortChildren(struct engineLayout02View *this){
	struct engineLayout02View dummy;
	dummySet(this, &dummy);

	struct engineLayout02View *insert = dummy.family.next;
	while(insert != &dummy){
		struct engineLayout02View *keepNext = insert->family.next;
		struct engineLayout02View *find = dummy.family.next;
		while(find != insert){
			if(compare(insert, find) < 0){
				// insertをリストから外す
				insert->family.prev->family.next = insert->family.next;
				insert->family.next->family.prev = insert->family.prev;
				// insertをfindの前に挿入
				insert->family.prev = find->family.prev;
				insert->family.next = find;
				find->family.prev->family.next = insert;
				find->family.prev = insert;
				break;
			}
			find = find->family.next;
		}
		insert = keepNext;
	}

	dummyUnset(this, &dummy);
}

// 表示要素構造体子要素 タッチ処理
bool engineLayout02ViewUtilChildrenTouch(struct engineLayout02View *this, int touchIndex, double x, double y, bool dn, bool mv, bool isCancel){
	if(this == viewRootGet()){return false;}
	bool isActive = false;
	// 子要素のタッチ処理 表示手前から先に処理する
	struct engineLayout02View *temp = this->family.childrenTail;
	while(temp != NULL){
		isCancel = isCancel || isActive || temp->family.isInactive;
		isActive = temp->touch(temp, touchIndex, x, y, dn, mv, isCancel);
		temp = temp->family.prev;
	}
	return isActive;
}

// 表示要素構造体子要素 計算
void engineLayout02ViewUtilChildrenCalc(struct engineLayout02View *this){
	if(this == viewRootGet()){return;}
	// 子要素の並べ替え
	sortChildren(this);
	// 子要素の計算
	struct engineLayout02View *temp = this->family.childrenHead;
	while(temp != NULL){
		if(!temp->family.isInactive){temp->calc(temp);}
		temp = temp->family.next;
	}
}

// 表示要素構造体子要素 描画
void engineLayout02ViewUtilChildrenDraw(struct engineLayout02View *this, struct engineMathMatrix44 *mat, struct engineMathVector4 *color){
	if(this == viewRootGet()){return;}
	// 子要素の描画 表示奥から先に処理する
	struct engineLayout02View *temp = this->family.childrenHead;
	while(temp != NULL){
		if(!temp->family.isInvisible){temp->draw(temp, mat, color);}
		temp = temp->family.next;
	}
}

// 表示要素構造体子要素 一時停止
void engineLayout02ViewUtilChildrenPause(struct engineLayout02View *this){
	if(this == viewRootGet()){return;}
	// 子要素の一時停止
	struct engineLayout02View *temp = this->family.childrenHead;
	while(temp != NULL){
		temp->pause(temp);
		temp = temp->family.next;
	}
}

// 表示要素構造体子要素 破棄
void engineLayout02ViewUtilChildrenDispose(struct engineLayout02View *this){
	if(this == viewRootGet()){return;}
	// 子要素の破棄
	struct engineLayout02View *temp = this->family.childrenHead;
	while(temp != NULL){
		struct engineLayout02View *dispose = temp;
		temp = temp->family.next;
		dispose->family.parent = NULL;
		dispose->family.prev = NULL;
		dispose->family.next = NULL;
		dispose->dispose(dispose);
	}
	this->family.childrenHead = NULL;
	this->family.childrenTail = NULL;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

