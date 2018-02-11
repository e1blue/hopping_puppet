
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

(function(jsonList){
	jsonList.push({
		"dst": "system.png",
		"tag": "SYSTEM",
		"prefixes": {"sys": "system/"},
		"width": 1024,
		"height": 1024,
		"imgs": [
			{"tag": "IMAGETITLE", "x": (41 + 2) * 2, "y": 0, "prefixId": "sys", "src": "image/title.png"},
			{"tag": "BOXWHITE", "x": 0, "y": 0, "isTurn": false, "prefixId": "sys", "src": "box/white.png"},
			{"tag": "BOXBASIC", "x": 0, "y": (32 + 2), "prefixId": "sys", "src": "box/basic.png"},
			{"tag": "BUTTONBASICNORMAL",   "x": (41 + 2), "y": (49 + 2) * 0, "prefixId": "sys", "src": "button/basic/normal.png"},
			{"tag": "BUTTONBASICACTIVE",   "x": (41 + 2), "y": (49 + 2) * 1, "prefixId": "sys", "src": "button/basic/active.png"},
			{"tag": "BUTTONBASICSELECT",   "x": (41 + 2), "y": (49 + 2) * 2, "prefixId": "sys", "src": "button/basic/select.png"},
			{"tag": "BUTTONBASICINACTIVE", "x": (41 + 2), "y": (49 + 2) * 3, "prefixId": "sys", "src": "button/basic/inactive.png"},
			{"tag": "BUTTONCTRLBTNNORMAL", "x": 0, "y": (32 + 2) + (41 + 2) + (49 + 2) * 0, "prefixId": "sys", "src": "button/ctrl/btn/normal.png"},
			{"tag": "BUTTONCTRLBTNACTIVE", "x": 0, "y": (32 + 2) + (41 + 2) + (49 + 2) * 1, "prefixId": "sys", "src": "button/ctrl/btn/active.png"},
			{"tag": "BUTTONPICKERNORMAL",   "x": (41 + 2) * 2 + (56 + 2) * 0, "y": (200 + 2), "prefixId": "sys", "src": "button/picker/normal.png"},
			{"tag": "BUTTONPICKERACTIVE",   "x": (41 + 2) * 2 + (56 + 2) * 1, "y": (200 + 2), "prefixId": "sys", "src": "button/picker/active.png"},
			{"tag": "BUTTONPICKERSELECT",   "x": (41 + 2) * 2 + (56 + 2) * 2, "y": (200 + 2), "prefixId": "sys", "src": "button/picker/select.png"},
			{"tag": "BUTTONPICKERINACTIVE", "x": (41 + 2) * 2 + (56 + 2) * 3, "y": (200 + 2), "prefixId": "sys", "src": "button/picker/inactive.png"},

			{"tag": "BUTTONCTRLUPNORMAL", "x": 0                             , "y": (200 + 2) + (60 + 2)           , "prefixId": "sys", "src": "button/ctrl/up/normal.png"},
			{"tag": "BUTTONCTRLDNNORMAL", "x":                       (92 + 2), "y": (200 + 2) + (60 + 2) + (72 + 2), "prefixId": "sys", "src": "button/ctrl/dn/normal.png"},
			{"tag": "BUTTONCTRLRTNORMAL", "x":                       (68 + 2), "y": (200 + 2) + (60 + 2)           , "prefixId": "sys", "src": "button/ctrl/rt/normal.png"},
			{"tag": "BUTTONCTRLLTNORMAL", "x": 0                             , "y": (200 + 2) + (60 + 2) + (96 + 2), "prefixId": "sys", "src": "button/ctrl/lt/normal.png"},
			{"tag": "BUTTONCTRLUPACTIVE", "x": (68 + 2) + (92 + 2)           , "y": (200 + 2) + (60 + 2)           , "prefixId": "sys", "src": "button/ctrl/up/active.png"},
			{"tag": "BUTTONCTRLDNACTIVE", "x": (68 + 2) + (92 + 2) + (92 + 2), "y": (200 + 2) + (60 + 2) + (72 + 2), "prefixId": "sys", "src": "button/ctrl/dn/active.png"},
			{"tag": "BUTTONCTRLRTACTIVE", "x": (68 + 2) + (92 + 2) + (68 + 2), "y": (200 + 2) + (60 + 2)           , "prefixId": "sys", "src": "button/ctrl/rt/active.png"},
			{"tag": "BUTTONCTRLLTACTIVE", "x": (68 + 2) + (92 + 2)           , "y": (200 + 2) + (60 + 2) + (96 + 2), "prefixId": "sys", "src": "button/ctrl/lt/active.png"},

			{"tag": "FONTABCD",   "x":   0, "y": (200 + 2) + (60 + 2) + (96 + 2) + (72 + 2) + 180 * 0, "prefixId": "sys", "src": "font/abcd.png"},
			{"tag": "FONTHIRA",   "x":   0, "y": (200 + 2) + (60 + 2) + (96 + 2) + (72 + 2) + 180 * 1, "prefixId": "sys", "src": "font/hira.png"},
			{"tag": "FONTKATA",   "x":   0, "y": (200 + 2) + (60 + 2) + (96 + 2) + (72 + 2) + 180 * 2, "prefixId": "sys", "src": "font/kata.png"},
			{"tag": "FONTNUMBER", "x":   0, "y": (200 + 2) + (60 + 2) + (96 + 2) + (72 + 2) + 180 * 3, "prefixId": "sys", "src": "font/number.png"},
			{"tag": "FONTKANJI",  "x": 200, "y": (200 + 2) + (60 + 2) + (96 + 2) + (72 + 2) + 180 * 3, "prefixId": "sys", "src": "font/kanji.png"},

			{"tag": "IMAGEPLAYERICON", "x": 328 + (180 + 2) * 3, "y": (88 + 2) * 0, "prefixId": "sys", "src": "image/playerIcon1.png"},
			{"tag": "IMAGEPLAYERICON", "x": 328 + (180 + 2) * 3, "y": (88 + 2) * 1, "prefixId": "sys", "src": "image/playerIcon2.png"},
			{"tag": "IMAGEPUPPETCAGE", "x": 328 + (180 + 2) * 3, "y": (88 + 2) * 2, "prefixId": "sys", "src": "image/puppetCage.png"},

			{"tag": "IMAGEMAPCHIP", "x": 328, "y": (300 + 2), "prefixId": "sys", "src": "image/mapchip.png"},
			{"tag": "IMAGEMONEY", "x": 328 + (256 + 2) + (60 + 2) * 0, "y": (300 + 2), "prefixId": "sys", "src": "image/itemMoney.png"},
			{"tag": "IMAGEITEM", "x": 328 + (256 + 2) + (60 + 2) * 1, "y": (300 + 2), "prefixId": "sys", "src": "image/itemHealth.png"},
			{"tag": "IMAGEITEM", "x": 328 + (256 + 2) + (60 + 2) * 2, "y": (300 + 2), "prefixId": "sys", "src": "image/itemTension.png"},
			{"tag": "IMAGEITEM", "x": 328 + (256 + 2) + (60 + 2) * 3, "y": (300 + 2), "prefixId": "sys", "src": "image/itemMonsterMove.png"},
			{"tag": "IMAGEITEM", "x": 328 + (256 + 2) + (60 + 2) * 4, "y": (300 + 2), "prefixId": "sys", "src": "image/itemCanvasFull.png"},
			{"tag": "IMAGEITEM", "x": 328 + (256 + 2) + (60 + 2) * 5, "y": (300 + 2), "prefixId": "sys", "src": "image/itemCanvasOpen.png"},
			{"tag": "IMAGEITEM", "x": 328 + (256 + 2) + (60 + 2) * 6, "y": (300 + 2), "prefixId": "sys", "src": "image/itemWorkspaceOpen.png"},
			{"tag": "IMAGECARD", "x": 328 + (256 + 2), "y": (300 + 2) + (60 + 2), "prefixId": "sys", "src": "image/card.png"},
			{"tag": "IMAGESLIDE", "x": 328 + (256 + 2) + (128 + 2), "y": (300 + 2) + (60 + 2), "prefixId": "sys", "src": "image/slide.png"},

			{"tag": "IMAGERESULT", "x": (320 + 2) + (240 + 2) * 0, "y": (300 + 2) + (256 + 2), "prefixId": "sys", "src": "image/resultWin.png"},
			{"tag": "IMAGERESULT", "x": (320 + 2) + (240 + 2) * 1, "y": (300 + 2) + (256 + 2), "prefixId": "sys", "src": "image/resultLose.png"},
			{"tag": "IMAGETUTORIALNAVI", "x": (320 + 2) + (240 + 2) * 2, "y": (300 + 2) + (256 + 2), "prefixId": "sys", "src": "image/tutorialNavi.png"},
			{"tag": "IMAGETUTORIALARROW", "x": (320 + 2) + (240 + 2) * 2, "y": (300 + 2) + (256 + 2) + (160 + 2), "prefixId": "sys", "src": "image/tutorialArrow.png"},
			{"tag": "IMAGETUTORIALPOINT", "x": (320 + 2) + (240 + 2) * 2, "y": (300 + 2) + (256 + 2) + (160 + 2) + (80 + 2), "prefixId": "sys", "src": "image/tutorialPoint.png"},
		]
	});
})(document.imageDataJsonList = document.imageDataJsonList || []);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

