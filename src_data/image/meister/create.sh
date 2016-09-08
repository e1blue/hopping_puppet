#!/bin/bash

# ----------------------------------------------------------------

TARGET=meister
#TESTMODE=1
#RELEASEMODE=1
IMGW=512
IMGH=512

. ../util.sh

# ----------------------------------------------------------------

create(){
	convert -size ${IMGW}x${IMGH} xc:'rgba(0, 0, 0, 0)' ${TEMPFILE}

	putSprite WORKSPACE ${1}/workspace.png 0 0 128 184
	putSprite PROFILE   ${1}/profile.png   $((128 + ${SP})) 0 280 360

	optimize ${IMGDIR}/${TARGET}/${1}.png
}

# ----------------------------------------------------------------

create 00copp
create 01poko
create 02mugi
create 03enis
create 04dogg
create 05alic
create 06ghos
create 07caro
create 08papi
create 09baba
create 10rena
create 11peng
create 12amie
create 13hnya
create 14fuha
create 15fact
create 99test

# ----------------------------------------------------------------

