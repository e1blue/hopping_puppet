DIR = $(shell pwd)
IDENTIFIER = com.example.fuhaha

default: web

# --------------------------------

clean: web-clean android-clean ios-clean

debug: web-debug android-debug ios-debug

# --------------------------------

copy:
	rsync -av --delete contents/ src_client/platform_android/assets/ --exclude='*.ogg'
	rsync -av --delete contents/ src_client/platform_ios/assets/ --exclude='*.ogg'
	rsync -av --delete contents/ src_client/platform_web/bin/ --exclude='game.js' --exclude='game.js.mem' --exclude='game.html' --exclude='frame.html'

# --------------------------------

web: copy web-debug web-node

web-node:
	node src_server/node/main.js

web-debug:
	$(MAKE) -C src_client/platform_web debug

web-release: copy
	$(MAKE) -C src_client/platform_web release

web-clean:
	$(MAKE) -C src_client/platform_web clean

# --------------------------------

android: copy android-debug
	adb install -r src_client/platform_android/build/outputs/apk/platform_android-all-debug.apk
	adb logcat

android-isIns:
	adb shell pm list package | grep $(IDENTIFIER)

android-debug:
	cd src_client/platform_android; ./gradlew assembleDebug

android-release: copy
	cd src_client/platform_android; ./gradlew assembleRelease
	ls src_client/platform_android/build/outputs/apk/platform_android-all-release.apk

android-clean:
	cd src_client/platform_android; ./gradlew clean

# --------------------------------

ios: copy ios-debug
	open /Applications/Xcode.app/Contents/Developer/Applications/Simulator.app
	xcrun simctl install booted src_client/platform_ios/build/Debug-iphonesimulator/fuhaha.app
	xcrun simctl launch booted $(IDENTIFIER)
 
ios-debug:
	xcodebuild build -project src_client/platform_ios/fuhaha.xcodeproj -scheme fuhaha -sdk iphonesimulator -configuration Debug

ios-clean:
	xcodebuild clean -project src_client/platform_ios/fuhaha.xcodeproj -scheme fuhaha -sdk iphonesimulator -configuration Debug

# --------------------------------

