/*
 *  INativeInterface.java
 *  moFlow
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.taggames.moflow.nativeinterface;

import com.taggames.moflow.CMoFlowActivity;
import com.taggames.moflow.core.IQueryableInterface;

//=============================================================
/// Native Interface
///
/// Base class for all native interfaces.
//=============================================================
public abstract class INativeInterface extends IQueryableInterface
{
	protected CMoFlowActivity mActivity;
	
	//---------------------------------------------------------
	/// Constructor
	//---------------------------------------------------------
	public INativeInterface()
	{
		CNativeInterfaceManager.GetSingleton().AddNativeInterface(this);
		mActivity = CMoFlowActivity.GetActivity();
	}
}
