import UIKit
import GLKit

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

class FuhahaGLViewController: GLKViewController{
	var context: EAGLContext? = nil;
	var fuhahaEvent: FuhahaEvent? = nil;

	// ----------------------------------------------------------------

	// viewの始まり
	override func viewDidLoad(){
		super.viewDidLoad();

		// openGL
		self.context = EAGLContext(API: .OpenGLES2);
		self.preferredFramesPerSecond = 60;
		let view: GLKView = self.view as! GLKView;
		view.context = self.context!;
		view.drawableDepthFormat = .Format24;
		view.drawableStencilFormat = .Format8;
		EAGLContext.setCurrentContext(self.context);

		// controller
		self.fuhahaEvent = FuhahaEvent();

		gameMainSurfaceCreated();
		gameMainEventInit();
	}

	// 描画時
	override func glkView(view: GLKView, drawInRect rect: CGRect){
		gameMainSurfaceDrawFrame();
	}

	// 画面サイズ変更時
	override func viewDidLayoutSubviews(){
		super.viewDidLayoutSubviews();
		gameMainEventScreen(Int32(self.view.frame.size.width), Int32(self.view.frame.size.height), 1.0);
	}

	// ----------------------------------------------------------------

	// ライフタイムイベント アプリ閉じそう
	static internal func applicationWillResignActive(){
	}

	// ライフタイムイベント アプリ閉じた
	static internal func applicationDidEnterBackground(){
		gameMainSurfacePause();
	}

	// ライフタイムイベント アプリ開きそう
	static internal func applicationWillEnterForeground(){
	}

	// ライフタイムイベント アプリ開いた
	static internal func applicationDidBecomeActive(){
	}

	// ライフタイムイベント フリックしてアプリを終了させた
	static internal func applicationWillTerminate(){
		gameMainSurfaceDestroy();
	}

	// ----------------------------------------------------------------

	// タッチ開始
	override func touchesBegan(touches: Set<UITouch>, withEvent event: UIEvent?){
		self.fuhahaEvent!.touchesBegan(self.view, touches: touches, withEvent: event);
	}

	// タッチ移動
	override func touchesMoved(touches: Set<UITouch>, withEvent event: UIEvent?){
		self.fuhahaEvent!.touchesMoved(self.view, touches: touches, withEvent: event);
	}

	// タッチ完了
	override func touchesEnded(touches: Set<UITouch>, withEvent event: UIEvent?){
		self.fuhahaEvent!.touchesEnded(self.view, touches: touches, withEvent: event);
	}

	// タッチ中止
	override func touchesCancelled(touches: Set<UITouch>!, withEvent event: UIEvent?){
		self.fuhahaEvent!.touchesCancelled(self.view, touches: touches, withEvent: event);
	}

	// ----------------------------------------------------------------

	// アプリ終了命令
	static internal func platformMainSurfaceExit(){
		exit(1);
	}

	// ----------------------------------------------------------------

	// メモリがやばい
	override func didReceiveMemoryWarning(){
		super.didReceiveMemoryWarning();
		gameMainSurfaceDestroy();
		if(self.isViewLoaded() && (self.view.window != nil)){
			self.view = nil;
			if(EAGLContext.currentContext() === self.context){
				EAGLContext.setCurrentContext(nil);
			}
			self.context = nil;
		}
	}

	// 解放時
	deinit{
		gameMainSurfaceDestroy();
		if(EAGLContext.currentContext() === self.context){
			EAGLContext.setCurrentContext(nil);
		}
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

