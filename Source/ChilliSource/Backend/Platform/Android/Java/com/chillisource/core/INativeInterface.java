/*
 *  INativeInterface.java
 *  moFlow
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;

import com.chillisource.core.ChilliSourceActivity;
import com.chillisource.core.IQueryableInterface;

//=============================================================
/// Native Interface
///
/// Base class for all native interfaces.
//=============================================================
public abstract class INativeInterface extends IQueryableInterface
{
	protected ChilliSourceActivity mActivity;
	
	//---------------------------------------------------------
	/// Constructor
	//---------------------------------------------------------
	public INativeInterface()
	{
		NativeInterfaceManager.GetSingleton().AddNativeInterface(this);
		mActivity = ChilliSourceActivity.GetActivity();
	}
}
