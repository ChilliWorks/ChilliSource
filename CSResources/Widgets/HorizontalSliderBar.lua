sliderWidget = Widget.getInternalWidget(thisWidget, "Slider")

function onPointerDown()
    io.write("HorizontalSliderBar: Pointer down\n")
end

function onPointerMoved()
    io.write("HorizontalSliderBar: Pointer moved\n")
    --absoluteMoveBy(10, 10)
    --slider.absoluteMoveBy(currX - prevX, 0)
end

function onPointerUp()
    io.write("HorizontalSliderBar: Pointer up\n")
end

moveDir = -1
function onUpdate(in_timeSinceLastUpdate)
    x, y = Widget.getLocalRelativePosition(sliderWidget);
    
    if x > 1.0  or x <= 0.0 then
        moveDir = -moveDir;
    end
    
    Widget.relativeMoveBy(sliderWidget, in_timeSinceLastUpdate * 0.1 * moveDir, 0)
end


