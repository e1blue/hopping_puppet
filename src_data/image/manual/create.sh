#!/bin/bash

# ----------------------------------------------------------------

TARGET=manual
#TESTMODE=1
#RELEASEMODE=1
IMGW=1024
IMGH=1024

. ../util.sh

# ----------------------------------------------------------------

convert -size ${IMGW}x${IMGH} xc:'rgba(0, 0, 0, 0)' ${TEMPFILE}

X0=0
Y0=0
putSprite MANUAL00 manual00.png $((${X0} + (200 + ${SP}) * 0)) $((${Y0} + (200 + ${SP}) * 0)) 200 200
putSprite MANUAL01 manual01.png $((${X0} + (200 + ${SP}) * 1)) $((${Y0} + (200 + ${SP}) * 0)) 200 200
putSprite MANUAL02 manual02.png $((${X0} + (200 + ${SP}) * 2)) $((${Y0} + (200 + ${SP}) * 0)) 200 200
putSprite MANUAL03 manual03.png $((${X0} + (200 + ${SP}) * 3)) $((${Y0} + (200 + ${SP}) * 0)) 200 200
putSprite MANUAL04 manual04.png $((${X0} + (200 + ${SP}) * 0)) $((${Y0} + (200 + ${SP}) * 1)) 200 200
putSprite MANUAL05 manual05.png $((${X0} + (200 + ${SP}) * 1)) $((${Y0} + (200 + ${SP}) * 1)) 200 200
putSprite MANUAL06 manual06.png $((${X0} + (200 + ${SP}) * 2)) $((${Y0} + (200 + ${SP}) * 1)) 200 200
putSprite MANUAL07 manual07.png $((${X0} + (200 + ${SP}) * 3)) $((${Y0} + (200 + ${SP}) * 1)) 200 200
putSprite MANUAL08 manual08.png $((${X0} + (200 + ${SP}) * 0)) $((${Y0} + (200 + ${SP}) * 2)) 200 200
putSprite MANUAL09 manual09.png $((${X0} + (200 + ${SP}) * 1)) $((${Y0} + (200 + ${SP}) * 2)) 200 200
putSprite MANUAL10 manual10.png $((${X0} + (200 + ${SP}) * 2)) $((${Y0} + (200 + ${SP}) * 2)) 200 200
putSprite MANUAL11 manual11.png $((${X0} + (200 + ${SP}) * 3)) $((${Y0} + (200 + ${SP}) * 2)) 200 200
putSprite MANUAL12 manual12.png $((${X0} + (200 + ${SP}) * 0)) $((${Y0} + (200 + ${SP}) * 3)) 200 200
putSprite MANUAL13 manual13.png $((${X0} + (200 + ${SP}) * 1)) $((${Y0} + (200 + ${SP}) * 3)) 200 200
putSprite MANUAL14 manual14.png $((${X0} + (200 + ${SP}) * 2)) $((${Y0} + (200 + ${SP}) * 3)) 200 200
putSprite MANUAL15 manual15.png $((${X0} + (200 + ${SP}) * 3)) $((${Y0} + (200 + ${SP}) * 3)) 200 200

optimize ${IMGDIR}/${TARGET}.png

# ----------------------------------------------------------------

