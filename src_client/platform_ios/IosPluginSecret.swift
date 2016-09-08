import Foundation

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// プラグインクラス
class IosPluginSecret: NSObject{
	// ----------------------------------------------------------------

	static internal func iosPluginSecretGet(key: String, id: Int32) -> String{
		let cstr: [CChar] = key.cStringUsingEncoding(NSUTF8StringEncoding)!;
		let value: String? = String.fromCString(UnsafePointer<CChar>(gamePluginSecretGet(cstr, id)));
		return value!;
	}

	// ----------------------------------------------------------------
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

