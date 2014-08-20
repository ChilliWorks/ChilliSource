
function onAddedToCanvas()
    io.write("HorizontalSliderBar: onAddedToCanvas\n")
    sliderWidget = Widget.getInternalWidget(thisWidget, "Slider")
end

moveDir = -1
function onUpdate(in_timeSinceLastUpdate)
    io.write("HorizontalSliderBar: onUpdate\n")
    x, y = Widget.getLocalRelativePosition(sliderWidget);
    
    if x > 1.0  or x <= 0.0 then
        moveDir = -moveDir;
    end

    Widget.relativeMoveBy(sliderWidget, in_timeSinceLastUpdate * 0.1 * moveDir, 0)
end

function onRemovedFromCanvas()
    io.write("HorizontalSliderBar: onRemovedFromCanvas\n")
end


