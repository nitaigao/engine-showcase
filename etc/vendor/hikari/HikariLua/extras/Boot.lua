
function onOpacityChange(caller,o)
	caller:setOpacity(o)
end

function onColorChange(caller,c)
	SetViewportColor(c)
end

function onExitClick(caller)
	QuitProgram();
end

controls = FlashControl()
controls:init("Controls", 350, 400, "Center");
controls:load("controls.swf");
controls:setTransparent(true, true);
controls:bind("opacityChange", "onOpacityChange");
controls:bind("colorChange", "onColorChange");
controls:bind("exitClick", "onExitClick");
