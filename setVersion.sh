#!/bin/bash

IDENTIFIER=com.totetero.hopping
VERSIONCODE=8
VERSIONNAME=1.4.0

# --------------------------------

# make id設定
sed -i '' -e 's/IDENTIFIER = .*/IDENTIFIER = '${IDENTIFIER}'/g' Makefile

# android id設定
sed -i '' -e 's/project.ext.fuhahaApplicationId = ".*"/project.ext.fuhahaApplicationId = "'${IDENTIFIER}'"/g' src_client/platform_android/build.gradle

# ios id設定
sed -i '' -e 's/PRODUCT_BUNDLE_IDENTIFIER = .*;/PRODUCT_BUNDLE_IDENTIFIER = '${IDENTIFIER}';/g' src_client/platform_ios/fuhaha.xcodeproj/project.pbxproj

# android version設定
sed -i '' -e 's/project.ext.fuhahaVersionCode = .*/project.ext.fuhahaVersionCode = '${VERSIONCODE}'/g' src_client/platform_android/build.gradle
sed -i '' -e 's/project.ext.fuhahaVersionName = ".*"/project.ext.fuhahaVersionName = "'${VERSIONNAME}'"/g' src_client/platform_android/build.gradle

# ios version設定
cat src_client/platform_ios/fuhaha/Info.plist | perl -e '
	$true = 1;
	$false = 0;
	$isCode = $false;
	$isName = $false;
	while($line = <STDIN>){
		if($isCode){
			$isCode = $false;
			$line =~ s/<string>.*<\/string>/<string>'${VERSIONCODE}'<\/string>/;
		}elsif($isName){
			$isName = $false;
			$line =~ s/<string>.*<\/string>/<string>'${VERSIONNAME}'<\/string>/;
		}elsif($line =~ /CFBundleShortVersionString/){
			$isName = $true;
		}elsif($line =~ /CFBundleVersion/){
			$isCode = $true;
		}
		print $line
	}
' > temp.txt
mv temp.txt src_client/platform_ios/fuhaha/Info.plist

# --------------------------------

