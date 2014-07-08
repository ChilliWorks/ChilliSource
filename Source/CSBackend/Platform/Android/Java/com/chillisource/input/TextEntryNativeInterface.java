/**
 * TextEntryNativeInterface.java
 * Chilli Source
 * Created by Ian Copland on 04/02/2014.
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

package com.chillisource.input;

import android.content.res.Configuration;
import android.view.ViewGroup;

import com.chillisource.core.CSApplication;
import com.chillisource.core.Logging;
import com.chillisource.core.InterfaceIDType;
import com.chillisource.input.NativeKeyboardInputView.KeyboardCapitalisation;
import com.chillisource.input.NativeKeyboardInputView.KeyboardType;
import com.chillisource.core.INativeInterface;

//========================================================
/// TextEntry Native Interface
///
/// A native interface for handling keyboard input. 
//========================================================
public class TextEntryNativeInterface extends INativeInterface
{
	//-------------------------------------------------
	/// Member data
	//-------------------------------------------------
	public static final InterfaceIDType InterfaceID = new InterfaceIDType("CKeyboardNativeInterface");
	private NativeKeyboardInputView mKeyboardView = null;
	private boolean mbHardwareKeyboardOpen = false;
	private volatile KeyboardType meKeyboardType = KeyboardType.TEXT;
	private volatile KeyboardCapitalisation meKeyboardCapitalisation = KeyboardCapitalisation.NONE;

	//-------------------------------------------------
	/// Is A
	///
	/// @return whether or not this implements the given 
	/// interface.
	//-------------------------------------------------
	@Override public boolean IsA(InterfaceIDType inInterfaceType) 
	{
		return (inInterfaceType == InterfaceID);
	}
	//-------------------------------------------------
	/// Activate
	///
	/// Displays the software keyboard on screen and 
	/// starts reading from the hardware keyboard 
	/// if possible.
	//-------------------------------------------------
	public void Activate()
	{
		final TextEntryNativeInterface thisNativeInterface = this;
		
		Runnable task = new Runnable() 
		{
			@Override public void run() 
			{
				if (mKeyboardView == null)
				{
					mKeyboardView = new NativeKeyboardInputView(thisNativeInterface, meKeyboardType, meKeyboardCapitalisation);
					CSApplication.get().addView(mKeyboardView);
					
					if (mbHardwareKeyboardOpen == true)
					{
						mKeyboardView.SetHardwareKeyboardOpen();
					}
					else
					{
						mKeyboardView.SetHardwareKeyboardClosed();
					}
					
					mKeyboardView.ActivateKeyboard();
				}
			}
		};
		CSApplication.get().scheduleUIThreadTask(task);
	}
	//-------------------------------------------------
	/// Deactivate
	///
	/// Hides the software keyboard and stops reading 
	/// from the hardware keyboard if there is one.
	//-------------------------------------------------
	public void Deactivate()
	{
		Runnable task = new Runnable() 
		{
			@Override public void run() 
			{
				if (mKeyboardView != null)
				{
					mKeyboardView.DeactivateKeyboard();
					
					ViewGroup parent = (ViewGroup)mKeyboardView.getParent();
					if (parent != null)
					{
						parent.removeView(mKeyboardView);
					}
					
					mKeyboardView = null;
				}
			}
		};
		CSApplication.get().scheduleUIThreadTask(task);
	}
	//-------------------------------------------------
	/// Set Keyboard Type
	///
	/// Sets the keyboard type that should be used
	/// the next time a keyboard is created.
	///
	/// @param the keyboard type in integer form (so it
	/// can be passed from native)
	//-------------------------------------------------
    public void SetKeyboardType(int indwKeyboardType)
    {
    	meKeyboardType = IntegerToKeyboardType(indwKeyboardType);
    }
	//-------------------------------------------------
	/// Set Capitalisation Method
	///
	/// Sets the capitalisation method that should be
    /// used the next time a keyboard is created.
	///
	/// @param the capitalisation method in integer form 
    /// (so it can be passed from native)
	//-------------------------------------------------
    public void SetCapitalisationMethod(int indwKeyboardCapitalisation)
    {
    	meKeyboardCapitalisation = IntegerToKeyboardCapitalisation(indwKeyboardCapitalisation);
    }
	//-----------------------------------------------------
	/// Set Hardware Keyboard Open
	///
	/// Informs the system that the hardware keyboard is 
	/// now open.
	//-----------------------------------------------------
	public void SetHardwareKeyboardOpen()
	{
		Runnable task = new Runnable() 
		{
			@Override public void run() 
			{
				mbHardwareKeyboardOpen = true;
				if (mKeyboardView != null)
				{
					mKeyboardView.SetHardwareKeyboardOpen();
				}
			}
		};
		CSApplication.get().scheduleUIThreadTask(task);
	}
	//----------------------------------------------------
	/// Set Hardware Keyboard Closed
	///
	/// Informs the system that the hardware keyboard is 
	/// now closed.
	//----------------------------------------------------
	public void SetHardwareKeyboardClosed()
	{
		Runnable task = new Runnable() 
		{
			@Override public void run() 
			{
				mbHardwareKeyboardOpen = false;
				if (mKeyboardView != null)
				{
					mKeyboardView.SetHardwareKeyboardClosed();
				}
			}
		};
		CSApplication.get().scheduleUIThreadTask(task);
	}
	//---------------------------------------------------
	/// On Text Added
	///
	/// Passes an event down to the native side of the
	/// engine for adding new text from the keyboard.
	///
	/// @param The new text received.
	//---------------------------------------------------
	public void OnTextAdded(final String instrText)
	{
		//This can be called on any thread as the native side of
		//the engine will pass it on as a "main thread" task.
		NativeOnTextAdded(instrText);
	}
	//---------------------------------------------------
	/// On Text Deleted
	///
	/// Passes an event down to the native side of the
	/// engine that will delete a single character from 
	/// the keyboard.
	///
	/// @param The new text received.
	//---------------------------------------------------
	public void OnTextDeleted()
	{
		//This can be called on any thread as the native side of
		//the engine will pass it on as a "main thread" task.
		NativeOnTextDeleted();
	}
	//----------------------------------------------------
	/// On Keyboard Dismissed
	///
	/// Lets the native side of the engine know know that 
	/// the keyboard has been dismissed.
	//----------------------------------------------------
	public void OnKeyboardDismissed()
	{
		Runnable task = new Runnable() 
		{
			@Override public void run() 
			{
				if (mKeyboardView != null)
				{
					mKeyboardView.DeactivateKeyboard();
					
					ViewGroup parent = (ViewGroup)mKeyboardView.getParent();
					if (parent != null)
					{
						parent.removeView(mKeyboardView);
					}
					
					mKeyboardView = null;
				}
			}
		};
		CSApplication.get().scheduleUIThreadTask(task);
		
		//Call this on any thread as this native side of the engine
		//will create a "main thread" task for it.
		NativeOnKeyboardDismissed();
	}
	//-----------------------------------------------
	/// Integer To Keyboard Type
	///
	/// Converts from an integer to the keyboard
	/// type enum.
	///
	/// @param The keyboard type in integer form.
	/// @return The keyboard type.
	//-----------------------------------------------
	private NativeKeyboardInputView.KeyboardType IntegerToKeyboardType(int indwKeyboardType)
	{
		switch (indwKeyboardType)
		{
			case 0:
				return KeyboardType.TEXT;
			case 1:
				return KeyboardType.NUMERIC;
			default:
				Logging.logError("Invalid keyboard type integer, cannot be converted.");
				return KeyboardType.TEXT;
		}
	}
	//-----------------------------------------------
	/// Integer To Keyboard Capitalisation
	///
	/// Converts from an integer to the keyboard
	/// capitalisation enum.
	///
	/// @param The keyboard capitalisation in integer 
	/// form.
	/// @return The keyboard capitalisation.
	//-----------------------------------------------
	private NativeKeyboardInputView.KeyboardCapitalisation IntegerToKeyboardCapitalisation(int indwKeyboardCapitalisation)
	{
		switch (indwKeyboardCapitalisation)
		{
			case 0:
				return KeyboardCapitalisation.NONE;
			case 1:
				return KeyboardCapitalisation.SENTENCES;
			case 2:
				return KeyboardCapitalisation.WORDS;
			case 3:
				return KeyboardCapitalisation.ALL;
			default:
				Logging.logError("Invalid keyboard capitalisation integer, cannot be converted.");
				return KeyboardCapitalisation.NONE;
		}
	}
	/**
	 * Triggered when the activity config changes such as orientation,
	 * hardware keyboards, etc.
	 * 
	 * @author S Downie
	 * 
	 * @param New config
	 */
    @Override public void onActivityConfigurationChanged(Configuration in_config)
    {
    	// Checks whether a hardware keyboard is available    
    	if (in_config.hardKeyboardHidden == Configuration.HARDKEYBOARDHIDDEN_NO) 
    	{   
    		SetHardwareKeyboardOpen();
    	} 
    	else if (in_config.hardKeyboardHidden == Configuration.HARDKEYBOARDHIDDEN_YES) 
    	{
    		SetHardwareKeyboardClosed();
    	}
    }
	//-------------------------------------------------
	/// Native On Text Added
	///
	/// A native call for passing additional keyboard
	/// text down to the native side of the engine.
	///
	/// @param The updated keyboard text.
	//-------------------------------------------------
	native private boolean NativeOnTextAdded(final String instrText);
	//-------------------------------------------------
	/// Native On Text Deleted
	///
	/// A native call for passing text deletion events
	/// down to the native side of the engine.
	///
	/// @param The updated keyboard text.
	//-------------------------------------------------
	native private boolean NativeOnTextDeleted();
	//-------------------------------------------------
	/// Native On Keyboard Dismissed
	///
	/// A native call for passing the keyboard dismissed
	/// event down to the native side of the engine.
	//-------------------------------------------------
	native private void NativeOnKeyboardDismissed();
}
