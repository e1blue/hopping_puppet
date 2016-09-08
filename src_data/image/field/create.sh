#!/bin/bash

# ----------------------------------------------------------------

TARGET=field
#TESTMODE=1
#RELEASEMODE=1
IMGW=1024
IMGH=512

. ../util.sh

# ----------------------------------------------------------------

create(){
	convert -size ${IMGW}x${IMGH} xc:'rgba(0, 0, 0, 0)' ${TEMPFILE}

	putSprite FOOTHOLD fieldFoothold/${2}.png 0 $((140 + ${SP})) 640 320
	putSprite CIRCLE partsCircle/close.png $(((160 + ${SP}) * 0)) 0 160 140
	putSprite CIRCLE partsCircle/${3}.png $(((160 + ${SP}) * 1)) $(((140 + ${SP}) * 0)) 160 140
	putSprite CIRCLE partsCircle/${4}.png $(((160 + ${SP}) * 2)) $(((140 + ${SP}) * 0)) 160 140
	putSprite CIRCLE partsCircle/${5}.png $(((160 + ${SP}) * 3)) $(((140 + ${SP}) * 0)) 160 140
	putSprite DECORATION partsDecoration/next.png $(((160 + ${SP}) * 4 + ${SP} + (160 + ${SP}) * 0)) $(((160 + ${SP}) * 0)) 160 160
	putSprite DECORATION partsDecoration/prev.png $(((160 + ${SP}) * 4 + ${SP} + (160 + ${SP}) * 1)) $(((160 + ${SP}) * 0)) 160 160
	putSprite DECORATION partsDecoration/item.png $(((160 + ${SP}) * 4 + ${SP} + (160 + ${SP}) * 0)) $(((160 + ${SP}) * 1)) 160 160
	putSprite DECORATION partsDecoration/${6}.png $(((160 + ${SP}) * 4 + ${SP} + (160 + ${SP}) * 1)) $(((160 + ${SP}) * 1)) 160 160
	putSprite DECORATION partsDecoration/${7}.png $(((160 + ${SP}) * 4 + ${SP} + (160 + ${SP}) * 0)) $(((160 + ${SP}) * 2)) 160 160
	putSprite DECORATION partsDecoration/${8}.png $(((160 + ${SP}) * 4 + ${SP} + (160 + ${SP}) * 1)) $(((160 + ${SP}) * 2)) 160 160

	optimize ${IMGDIR}/${TARGET}/${1}.png
}

# ----------------------------------------------------------------

create 00grass   grass        grass1 soil1  test          start      tree       test       
create 01beach   sand         sand1  sand2  rock1         tropical   rock1      test       
create 02forest  soil         soil1  soil2  rock1         tree       forest     rock1      
create 03craggy  rock1        rock1  rock2  test          rock1      rock2      test       
create 04volcano rock3        rock3  rock2  test          rock3      rock2      test       
create 05snow    snow         snow1  soil1  test          snowman    snowTree   snowForest 
create 06cake    cake         cake1  cake2  cake3         berry      test       test       
create 99test    test         test   test   test          test       test       test       

# ----------------------------------------------------------------

