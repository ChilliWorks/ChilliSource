
-----------------------------------------------------
-- Called when the widget is added to the canvas
--
-- @author S Downie
-----------------------------------------------------
function onAddedToCanvas()
    io.write("HorizontalSliderBar: onAddedToCanvas\n")
    sliderWidget = Widget.getInternalWidget(thisWidget, "Slider")
end

moveDir = -1
-----------------------------------------------------
-- Called when the widget is updated
--
-- @author S Downie
--
-- @param Time in seconds since last update
-----------------------------------------------------
function onUpdate(in_timeSinceLastUpdate)
    x, y = Widget.getLocalRelativePosition(sliderWidget);
    
    if x > 1.0  or x <= 0.0 then
        moveDir = -moveDir;
    end

    Widget.relativeMoveBy(sliderWidget, in_timeSinceLastUpdate * 0.1 * moveDir, 0)
end
-----------------------------------------------------
-- Called when the widget is removed from the canvas
--
-- @author S Downie
-----------------------------------------------------
function onRemovedFromCanvas()
    io.write("HorizontalSliderBar: onRemovedFromCanvas\n")
end


