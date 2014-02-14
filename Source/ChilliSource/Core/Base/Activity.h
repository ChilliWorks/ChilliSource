/*
 *  Activity.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 10/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_ACTIVITY_H_
#define _MOFLO_CORE_ACTIVITY_H_

#include <ChilliSource/Core/Base/QueryableInterface.h>

namespace moFlo{

	/*
	 IActivity represents a class offering UI to perform a specific task
	*/
	class IActivity : public Core::IQueryableInterface
	{
	public:
		virtual ~IActivity(){}
	};
	
	typedef fastdelegate::FastDelegate1<IActivity*> ActivityDismissedEvent;
}


#endif