//
//  CKeyboardNativeInterface.java
//  moFlow
//
//  Created by Ian Copland on 04/02/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//

package com.chillisource.input;

import android.view.ViewGroup;

import com.chillisource.core.InterfaceIDType;
import com.chillisource.input.NativeKeyboardInputView.KeyboardCapitalisation;
import com.chillisource.input.NativeKeyboardInputView.KeyboardType;
import com.chillisource.core.INativeInterface;

//========================================================
/// Keyboard Native Interface
///
/// A native interface for handling keyboard input. 
//========================================================
public class KeyboardNativeInterface extends INativeInterface
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
	/// Constructor
	//-------------------------------------------------
	public KeyboardNativeInterface()
	{
	}
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
		final KeyboardNativeInterface thisKeyboardNativeInterface = this;
		
		Runnable task = new Runnable() 
		{
			@Override public void run() 
			{
				if (mKeyboardView == null)
				{
					mKeyboardView = new NativeKeyboardInputView(mActivity, thisKeyboardNativeInterface, meKeyboardType, meKeyboardCapitalisation);
					mActivity.GetViewContainer().addView(mKeyboardView);
					
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
		mActivity.runOnUiThread(task);
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
		mActivity.runOnUiThread(task);
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
		mActivity.runOnUiThread(task);
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
		mActivity.runOnUiThread(task);
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
		mActivity.runOnUiThread(task);
		
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
				android.util.Log.e("moFlow", "Invalid keyboard type integer, cannot be converted.");
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
				android.util.Log.e("moFlow", "Invalid keyboard capitalisation integer, cannot be converted.");
				return KeyboardCapitalisation.NONE;
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
