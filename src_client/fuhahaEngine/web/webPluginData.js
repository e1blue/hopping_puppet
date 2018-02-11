
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// HTTP通信 コールバックバッファは要解放
	platformPluginDataHttp: function(param, url, request, callback){
		Module.nativePluginUtilLoadingIncrement();
		var callbackId = ccall("gamePluginDataHttpCallbackSet", null, [null, null], [param, callback]);
		var xhr = new XMLHttpRequest();
		xhr.open("POST", "../" + Pointer_stringify(url), true);
		//xhr.open("POST", Pointer_stringify(ccall("gamePluginUtilUrlGet")) + "/" + Pointer_stringify(url), true);
		xhr.setRequestHeader("Content-Type", "application/octet-stream");
		xhr.responseType = "arraybuffer";
		xhr.onreadystatechange = function(e){
			if(xhr.readyState == 4){
				var size = 0;
				var buff = null;
				if(xhr.status == 200 && xhr.response != null){
					// 通信成功
					var byteArray = new Uint8Array(xhr.response);
					size = byteArray.byteLength;
					if(size > 0){
						buff = ccall("gamePluginUtilMemoryMalloc", null, ["string", null], ["pluginData", size]); // バッファの解放はゲーム側で行う
						writeArrayToMemory(byteArray, buff);
					}
				}else{
					//// 通信失敗
					//var message = "failed";
					//size = lengthBytesUTF8(message) + 1;
					//buff = ccall("gamePluginUtilMemoryMalloc", null, ["string", null], ["pluginData", size]); // バッファの解放はゲーム側で行う
					//stringToUTF8(message, buff, size);
				}
				Module.nativePluginUtilLoadingDecrement();
				ccall("gamePluginDataHttpCallbackCall", null, [null, null, null], [callbackId, buff, size]);
			}
		};
		xhr.send(Pointer_stringify(request));
	},

	// ----------------------------------------------------------------

	// ローカルデータ読み込み コールバックバッファは要解放
	platformPluginDataLocal: function(param, src, callback){
		Module.nativePluginUtilLoadingIncrement();
		var callbackId = ccall("gamePluginDataLocalCallbackSet", null, [null, null], [param, callback]);
		var xhr = new XMLHttpRequest();
		xhr.open("GET", Pointer_stringify(src), true);
		xhr.setRequestHeader("Content-Type", "application/octet-stream");
		xhr.responseType = "arraybuffer";
		xhr.onreadystatechange = function(e){
			if(xhr.readyState == 4){
				var size = 0;
				var buff = null;
				if(xhr.status == 200 && xhr.response != null){
					// 通信成功
					var byteArray = new Uint8Array(xhr.response);
					size = byteArray.byteLength;
					if(size > 0){
						buff = ccall("gamePluginUtilMemoryMalloc", null, ["string", null], ["pluginData", size]); // バッファの解放はゲーム側で行う
						writeArrayToMemory(byteArray, buff);
					}
				}else{
					//// 通信失敗
					//var message = "failed";
					//size = lengthBytesUTF8(message) + 1;
					//buff = ccall("gamePluginUtilMemoryMalloc", null, ["string", null], ["pluginData", size]); // バッファの解放はゲーム側で行う
					//stringToUTF8(message, buff, size);
				}
				Module.nativePluginUtilLoadingDecrement();
				ccall("gamePluginDataLocalCallbackCall", null, [null, null, null], [callbackId, buff, size]);
			}
		};
		xhr.send();
	},

	// ----------------------------------------------------------------
});

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

