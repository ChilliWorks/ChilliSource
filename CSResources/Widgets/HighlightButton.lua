
function onAddedToCanvas()
    io.write("HighlightButton: onAddedToCanvas\n")
    drawable = Widget.getDrawable(thisWidget)
    highlightLocation = Widget.getStorageLocationProperty(thisWidget, "HighlightTextureLocation")
    highlightPath = Widget.getStringProperty(thisWidget, "HighlightTexturePath")
    TextureDrawable.setTexture(drawable, highlightLocation, highlightPath)
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
    io.write("HighlightButton: onRemovedFromCanvas\n")
end
