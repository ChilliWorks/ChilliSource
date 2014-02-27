/*
 *  InformationProvider.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 10/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_INFORMATIONPROVIDER_H_
#define _MOFLO_CORE_INFORMATIONPROVIDER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

namespace ChilliSource
{
	namespace Core
    {
        /*
         IInformationProvider is the base class for objects representing interfaces into a data store of some kind
         */
        class IInformationProvider : public Core::QueryableInterface
            {
        public:
            virtual ~IInformationProvider(){}
            
        };
	}
}

#endif