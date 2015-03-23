/**
 * DeviceButtonJavaInterface.h
 * Chilli Source
 * Created by Ian Copland on 16/06/2014.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Tag Games Limited
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package com.chilliworks.chillisource.input;

import com.chilliworks.chillisource.core.INativeInterface;
import com.chilliworks.chillisource.core.InterfaceIDType;

/**
 * A native interface for passing information on physical device buttons
 * down to the native side of the engine. For example this passes on
 * back button events.
 * 
 * @author Ian Copland
 */
public final class DeviceButtonNativeInterface extends INativeInterface
{
	public static InterfaceIDType InterfaceID = new InterfaceIDType("DeviceButtonNativeInterface");
	
	/**
	 * An enum describing the different possible device buttons. This
	 * should have a direct 1 to 1 mapping with the equivalent in the
	 * DeviceButtonSystem.
	 * 
	 * @author Ian Copland
	 */
	public enum DeviceButton
	{
		k_backButton
	}
	/**
	 * Allows querying of whether or not this implements the interface
	 * with the given Id.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The interface Id.
	 * 
	 * @return Whether or not the interface is implemented.
	 */
	@Override public boolean IsA(InterfaceIDType in_interfaceId) 
	{
		return in_interfaceId.Equals(InterfaceID);
	}
	/**
	 * Called when a device button triggered event is received.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The button the event was received for.
	 */
	public void onTriggered(DeviceButton in_button) 
	{
		onTriggered(in_button.ordinal());
	}
	/**
	 * Called when a device button triggered event is triggered.
	 * 
	 * @author Ian Copland
	 * 
	 * @param An integer representing the button.
	 */
	private native void onTriggered(int in_button);
}
