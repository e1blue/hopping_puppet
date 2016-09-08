#!/bin/bash

# ----------------------------------------------------------------

TARGET=enemy
#TESTMODE=1
#RELEASEMODE=1
IMGW=1024
IMGH=1024

. ../util.sh

# ----------------------------------------------------------------

create(){
	convert -size ${IMGW}x${IMGH} xc:'rgba(0, 0, 0, 0)' ${TEMPFILE}

	putSprite BODY partsBody/${2}/body1.png $(((400 + ${SP}) * 0)) $(((400 + ${SP}) * 0)) 400 400
	putSprite BODY partsBody/${2}/body2.png $(((400 + ${SP}) * 1)) $(((400 + ${SP}) * 0)) 400 400
	putSprite ICON partsBody/${2}/icon1.png $(((400 + ${SP}) * 2 + (88 + ${SP}) * 0)) 0 88 88
	putSprite ICON partsBody/${2}/icon2.png $(((400 + ${SP}) * 2 + (88 + ${SP}) * 1)) 0 88 88
	putSprite FIELD partsBody/${2}/field.png $(((400 + ${SP}) * 2)) $((88 + ${SP})) 160 160
	putSprite WEAPON partsWeapon/${3}.png $(((200 + ${SP}) * 0)) $(((400 + ${SP}) * 1)) 200 200
	putSprite WEAPON partsWeapon/${4}.png $(((200 + ${SP}) * 1)) $(((400 + ${SP}) * 1)) 200 200
	putSprite WEAPON partsWeapon/${5}.png $(((200 + ${SP}) * 2)) $(((400 + ${SP}) * 1)) 200 200
	putSprite WEAPON partsWeapon/${6}.png $(((200 + ${SP}) * 3)) $(((400 + ${SP}) * 1)) 200 200

	optimize ${IMGDIR}/${TARGET}/${1}.png
}

# ----------------------------------------------------------------

create 00dragon     dragon1     fire      test2     test3     test4    
create 01dragon     dragon2     fire      test2     test3     test4    
create 02dragon     dragon3     fire      test2     test3     test4    
create 03fish       fish1       uni       test2     test3     test4    
create 04fish       fish2       uni       test2     test3     test4    
create 05fish       fish3       uni       test2     test3     test4    
create 06dog        dog1        dog       test2     test3     test4    
create 07dog        dog2        dog       test2     test3     test4    
create 08bird       bird1       bird      test2     test3     test4    
create 09bird       bird2       bird      test2     test3     test4    
create 10elephant   elephant1   ball      emerald   test3     test4    
create 11elephant   elephant2   ball      emerald   test3     test4    
create 12frog       frog1       snakeFire test2     test3     test4    
create 13frog       frog2       snakeFire test2     test3     test4    
create 14bug        bug         bug       test2     test3     test4    
create 15crab       crab1       crab1     sield     test3     test4    
create 16crab       crab2       crab2     sield     test3     test4    
create 17pteranodon pteranodon1 rapidFire test2     test3     test4    
create 18pteranodon pteranodon2 rapidFire test2     test3     test4    
create 19ufo        ufo         flare     test2     test3     test4    
create 99test       test        test1     test2     test3     test4    

# ----------------------------------------------------------------

