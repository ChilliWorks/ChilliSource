isDown = false

function onAddedToCanvas()
    drawable = Widget.getDrawable(thisWidget)
    highlightLocation = Widget.getStorageLocationProperty(thisWidget, "HighlightTextureLocation")
    highlightPath = Widget.getStringProperty(thisWidget, "HighlightTexturePath")
    TextureDrawable.setTexture(drawable, highlightLocation, highlightPath)
end

function onPointerDown()
    io.write("HighlightButton: Pointer down\n")
    isDown = true
end

function onPointerMoved()
    io.write("HighlightButton: Pointer moved\n")
end

function onPointerUp()
    io.write("HighlightButton: Pointer up\n")
    if(isDown == true) then
        isDown = false
        io.write("HighlightButton: Button Pressed\n")
    end
end

rotateDir = 1
function onUpdate(timeSinceLastUpdate)
	rotation = Widget.getFinalRotation(thisWidget)

	if rotation > 3.141 or rotation < -3.141 then
		rotateDir = -rotateDir
	end
    Widget.rotateBy(thisWidget, timeSinceLastUpdate * rotateDir)

end

function onRemovedFromCanvas()
end
