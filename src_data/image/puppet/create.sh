#!/bin/bash

# ----------------------------------------------------------------

TARGET=puppet
#TESTMODE=1
#RELEASEMODE=1
IMGW=512
IMGH=512

. ../util.sh

# ----------------------------------------------------------------

create(){
	convert -size ${IMGW}x${IMGH} xc:'rgba(0, 0, 0, 0)' ${TEMPFILE}

	convert -size 32x32 xc:'rgba(0, 0, 0, 0)' -draw 'fill black roundRectangle 4,4 27,27 12,12' png:- | convert ${TEMPFILE} png:- -geometry +0+256 -composite ${TEMPFILE}

	putSprite DOT     ${1}/dot.png     0 0 256 256
	putSprite PROFILE ${1}/profile.png $((256 + ${SP})) 0 240 320

	optimize ${IMGDIR}/${TARGET}/${1}.png
}

# ----------------------------------------------------------------

create 00kuma
create 01papi
create 02pasu
create 03poni
create 04botu
create 05ribb
create 06pebu
create 07skul
create 08dete
create 09dogg
create 10fang
create 11mami
create 12holy
create 13bone
create 14armo
create 15kobu
create 16kobo
create 17hand
create 18rinn
create 19debu
create 20tare
create 21weak
create 22bcoo
create 23ccoo
create 24tcoo
create 25guru
create 26koni
create 27pebe
create 28buta
create 29koga
create 30bear
create 31titi
create 32ruck
create 33guda
create 34fadd
create 35abso
create 36robo
create 37rock
create 38pika
create 39copp
create 99test

# ----------------------------------------------------------------

