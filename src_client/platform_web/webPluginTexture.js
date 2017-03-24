
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
"use strict";

mergeInto(LibraryManager.library, {
	// ----------------------------------------------------------------

	// ローカルデータ読み込み
	platformPluginTextureLocal: function(param, src, callback){
		Module.nativePluginUtilLoadingIncrement();
		var callbackId = ccall("gamePluginTextureLocalCallbackSet", null, [null, null], [param, callback]);
		var gl = Module.ctx;
		var image = new Image();
		image.onload = function(){
			var texture = GLctx.createTexture();
			var glId = GL.getNewId(GL.textures);
			texture.name = glId;
			GL.textures[glId] = texture;
			ccall("corePluginTextureIsBind", null, [null], [glId]);
			gl.bindTexture(gl.TEXTURE_2D, GL.textures[glId]);
			gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, image);
			gl.generateMipmap(gl.TEXTURE_2D);
			Module.nativePluginUtilLoadingDecrement();
			ccall("gamePluginTextureLocalCallbackCall", null, [null, null, null, null], [callbackId, glId, image.width, image.height]);
		};
		image.src = Pointer_stringify(src);
	},

	// ----------------------------------------------------------------
});

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

