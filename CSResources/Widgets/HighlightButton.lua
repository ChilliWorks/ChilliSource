
-----------------------------------------------------
-- Called when the widget is added to the canvas
--
-- @author S Downie
-----------------------------------------------------
function onAddedToCanvas()
    io.write("HighlightButton: onAddedToCanvas\n")
    drawable = Widget.getDrawable(thisWidget)
    highlightLocation = Widget.getStorageLocationProperty(thisWidget, "HighlightTextureLocation")
    highlightPath = Widget.getStringProperty(thisWidget, "HighlightTexturePath")
    TextureDrawable.setTexture(drawable, highlightLocation, highlightPath)
end

rotateDir = 1
-----------------------------------------------------
-- Called when the widget is updated
--
-- @author S Downie
--
-- @param Time in seconds since last update
-----------------------------------------------------
function onUpdate(timeSinceLastUpdate)
	rotation = Widget.getFinalRotation(thisWidget)

	if rotation > 3.141 or rotation < -3.141 then
		rotateDir = -rotateDir
	end
    Widget.rotateBy(thisWidget, timeSinceLastUpdate * rotateDir)

end
-----------------------------------------------------
-- Called when the widget is removed from the canvas
--
-- @author S Downie
-----------------------------------------------------
function onRemovedFromCanvas()
    io.write("HighlightButton: onRemovedFromCanvas\n")
end
