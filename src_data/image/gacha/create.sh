#!/bin/bash

# ----------------------------------------------------------------

TARGET=gacha
#TESTMODE=1
#RELEASEMODE=1
IMGW=128
IMGH=128

. ../util.sh

# ----------------------------------------------------------------

create(){
	convert -size ${IMGW}x${IMGH} xc:'rgba(0, 0, 0, 0)' ${TEMPFILE}

	putSprite PROMOTION ${1}/promotion.png 0 0 100 100

	optimize ${IMGDIR}/${TARGET}/${1}.png
}

# ----------------------------------------------------------------

create 00level1
create 01level2
create 02level3
create 99test

# ----------------------------------------------------------------

