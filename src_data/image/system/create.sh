#!/bin/bash

# ----------------------------------------------------------------

TARGET=system
#TESTMODE=1
#RELEASEMODE=1
IMGW=1024
IMGH=1024

. ../util.sh

# ----------------------------------------------------------------

echo \#define TEXPOS_SPACE ${SP} >> ${HEADERFILE}

# ----------------------------------------------------------------

convert -size ${IMGW}x${IMGH} xc:'rgba(0, 0, 0, 0)' ${TEMPFILE}

convert -size 32x32 xc:'rgba(255, 255, 255, 1)' png:- | convert ${TEMPFILE} png:- -geometry +0+0 -composite ${TEMPFILE}

X0=0
Y0=0
putSprite IMAGETITLE image/title.png $((${X0} + (41 + ${SP}) * 2)) $((${Y0})) 240 200
putSprite BOXBASIC             box/basic.png              $((${X0})) $((${Y0} + 32 + ${SP})) 41 41
putSprite BUTTONBASICNORMAL    button/basic/normal.png    $((${X0} + 41 + ${SP})) $((${Y0} + (49 + ${SP}) * 0)) 41 49
putSprite BUTTONBASICACTIVE    button/basic/active.png    $((${X0} + 41 + ${SP})) $((${Y0} + (49 + ${SP}) * 1)) 41 49
putSprite BUTTONBASICSELECT    button/basic/select.png    $((${X0} + 41 + ${SP})) $((${Y0} + (49 + ${SP}) * 2)) 41 49
putSprite BUTTONBASICINACTIVE  button/basic/inactive.png  $((${X0} + 41 + ${SP})) $((${Y0} + (49 + ${SP}) * 3)) 41 49
putSprite BUTTONCTRLBTNNORMAL  button/ctrl/btn/normal.png $((${X0})) $((${Y0} + 32 + ${SP} + 41 + ${SP} + (49 + ${SP}) * 0)) 41 49
putSprite BUTTONCTRLBTNACTIVE  button/ctrl/btn/active.png $((${X0})) $((${Y0} + 32 + ${SP} + 41 + ${SP} + (49 + ${SP}) * 1)) 41 49

X0=$(((41 + ${SP}) * 2))
Y0=$((200 + ${SP}))
putSprite BUTTONPICKERNORMAL   button/picker/normal.png   $((${X0} + (56 + ${SP}) * 0)) $((${Y0})) 56 60
putSprite BUTTONPICKERACTIVE   button/picker/active.png   $((${X0} + (56 + ${SP}) * 1)) $((${Y0})) 56 60
putSprite BUTTONPICKERSELECT   button/picker/select.png   $((${X0} + (56 + ${SP}) * 2)) $((${Y0})) 56 60
putSprite BUTTONPICKERINACTIVE button/picker/inactive.png $((${X0} + (56 + ${SP}) * 3)) $((${Y0})) 56 60

X0=0
Y0=$((200 + ${SP} + 60 + ${SP}))
putSprite BUTTONCTRLUPNORMAL button/ctrl/up/normal.png $((${X0} +  0 +     0)) $((${Y0} +  0 +     0)) 68 96
putSprite BUTTONCTRLDNNORMAL button/ctrl/dn/normal.png $((${X0} + 92 + ${SP})) $((${Y0} + 72 + ${SP})) 68 96
putSprite BUTTONCTRLRTNORMAL button/ctrl/rt/normal.png $((${X0} + 68 + ${SP})) $((${Y0} +  0 +     0)) 92 72
putSprite BUTTONCTRLLTNORMAL button/ctrl/lt/normal.png $((${X0} +  0 +     0)) $((${Y0} + 96 + ${SP})) 92 72
X0=$((${X0} + 68 + ${SP} + 92 + ${SP}))
putSprite BUTTONCTRLUPACTIVE button/ctrl/up/active.png $((${X0} +  0 +     0)) $((${Y0} +  0 +     0)) 68 96
putSprite BUTTONCTRLDNACTIVE button/ctrl/dn/active.png $((${X0} + 92 + ${SP})) $((${Y0} + 72 + ${SP})) 68 96
putSprite BUTTONCTRLRTACTIVE button/ctrl/rt/active.png $((${X0} + 68 + ${SP})) $((${Y0} +  0 +     0)) 92 72
putSprite BUTTONCTRLLTACTIVE button/ctrl/lt/active.png $((${X0} +  0 +     0)) $((${Y0} + 96 + ${SP})) 92 72

X0=0
Y0=$((200 + ${SP} + 60 + ${SP} + 96 + ${SP} + 72 + ${SP}))
putSprite FONTABCD font/abcd.png $((${X0})) $((${Y0} + 180 * 0)) 320 180
putSprite FONTHIRA font/hira.png $((${X0})) $((${Y0} + 180 * 1)) 320 180
putSprite FONTKATA font/kata.png $((${X0})) $((${Y0} + 180 * 2)) 320 180
putSprite FONTNUMBER font/number.png $((${X0})) $((${Y0} + 180 * 3)) 200 30
putSprite FONTKANJI font/kanji.png $((${X0} + 200)) $((${Y0} + 180 * 3)) 200 30

X0=328
Y0=0
putSprite IMAGEPLAYERICON image/playerIcon1.png $((${X0} + (180 + ${SP}) * 3)) $((${Y0} + (88 + ${SP}) * 0)) 88 88
putSprite IMAGEPLAYERICON image/playerIcon2.png $((${X0} + (180 + ${SP}) * 3)) $((${Y0} + (88 + ${SP}) * 1)) 88 88
putSprite IMAGEPUPPETCAGE image/puppetCage.png $((${X0} + (180 + ${SP}) * 3)) $((${Y0} + (88 + ${SP}) * 2)) 120 120

X0=328
Y0=$((300 + ${SP}))
putSprite IMAGEMAPCHIP image/mapchip.png $((${X0})) $((${Y0})) 256 256
putSprite IMAGEMONEY image/itemMoney.png $((${X0} + 256 + ${SP} + (60 + ${SP}) * 0)) $((${Y0})) 60 60
putSprite IMAGEITEM image/itemHealth.png $((${X0} + 256 + ${SP} + (60 + ${SP}) * 1)) $((${Y0})) 60 60
putSprite IMAGEITEM image/itemTension.png $((${X0} + 256 + ${SP} + (60 + ${SP}) * 2)) $((${Y0})) 60 60
putSprite IMAGEITEM image/itemMonsterMove.png $((${X0} + 256 + ${SP} + (60 + ${SP}) * 3)) $((${Y0})) 60 60
putSprite IMAGEITEM image/itemCanvasFull.png $((${X0} + 256 + ${SP} + (60 + ${SP}) * 4)) $((${Y0})) 60 60
putSprite IMAGEITEM image/itemCanvasOpen.png $((${X0} + 256 + ${SP} + (60 + ${SP}) * 5)) $((${Y0})) 60 60
putSprite IMAGEITEM image/itemWorkspaceOpen.png $((${X0} + 256 + ${SP} + (60 + ${SP}) * 6)) $((${Y0})) 60 60
putSprite IMAGECARD image/card.png $((${X0} + 256 + ${SP})) $((${Y0} + 60 + ${SP})) 128 184
putSprite IMAGESLIDE image/slide.png $((${X0} + 256 + ${SP} + 128 + ${SP})) $((${Y0} + 60 + ${SP})) 270 180

X0=$((320 + ${SP}))
Y0=$((${Y0} + 256 + ${SP}))
putSprite IMAGERESULT image/resultWin.png $((${X0} + (240 + ${SP}) * 0)) $((${Y0})) 240 240
putSprite IMAGERESULT image/resultLose.png $((${X0} + (240 + ${SP}) * 1)) $((${Y0})) 240 240
putSprite IMAGETUTORIALNAVI image/tutorialNavi.png $((${X0} + (240 + ${SP}) * 2)) $((${Y0})) 120 160
putSprite IMAGETUTORIALARROW image/tutorialArrow.png $((${X0} + (240 + ${SP}) * 2)) $((${Y0} + 160 + ${SP})) 80 80
putSprite IMAGETUTORIALPOINT image/tutorialPoint.png $((${X0} + (240 + ${SP}) * 2)) $((${Y0} + 160 + ${SP} + 80 + ${SP})) 80 80

optimize ${IMGDIR}/${TARGET}.png

# ----------------------------------------------------------------

