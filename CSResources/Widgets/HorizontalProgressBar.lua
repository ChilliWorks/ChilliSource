--
--  HorizontalProgressBar.lua
--  Chilli Source
--  Created by Scott Downie on 30/09/2014.
--
--  The MIT License (MIT)
--
--  Copyright (c) 2014 Tag Games Limited
--
--  Permission is hereby granted, free of charge, to any person obtaining a copy
--  of this software and associated documentation files (the "Software"), to deal
--  in the Software without restriction, including without limitation the rights
--  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
--  copies of the Software, and to permit persons to whom the Software is
--  furnished to do so, subject to the following conditions:
--
--  The above copyright notice and this permission notice shall be included in
--  all copies or substantial portions of the Software.
--
--  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
--  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
--  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
--  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
--  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
--  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
--  THE SOFTWARE.
--

-------------------------------------------------------------
-- This is the logic that describes a horizontal progress bar.
-- The progress bar consists of 2 widgets the bar and the background.
-- The bar will stretch to fit the background based on the current
-- progress which is a normalised percentage 0 - 1
--
-- @author S Downie
-------------------------------------------------------------

--Caching the functions locally means we don't have to peform a lookup for each call
local getFinalSize = Widget.getFinalSize
local setRelativeSize = Widget.setRelativeSize
local getLocalRelativeSize = Widget.getLocalRelativeSize
local getFloatCustomProperty = Widget.getFloatCustomProperty
local getInternalWidget = Widget.getInternalWidget

local barWidget = nil

-----------------------------------------------------
-- Called when the widget is added to the canvas
--
-- @author S Downie
-----------------------------------------------------
function onAddedToCanvas()
    barWidget = getInternalWidget(this, "Bar")
    progress = getFloatCustomProperty(this, "Progress")
    updateBar(progress)
end
-----------------------------------------------------
-- Called when a widget custom property is changed
--
-- @author S Downie
--
-- @param Custom property name
-- @param Custom property value
-----------------------------------------------------
function onCustomPropertyChanged(in_propName, in_propValue)
    if barWidget ~= nil and in_propName == "Progress" then
        updateBar(in_propValue)
    end
end
-----------------------------------------------------
-- Stretch the bar to the correct size based on the progress
--
-- @author S Downie
--
-- @param Normalised progress
-----------------------------------------------------
function updateBar(in_progress)
    barWidth, barHeight = getLocalRelativeSize(barWidget)
    setRelativeSize(barWidget, in_progress, barHeight)
end



