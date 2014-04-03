//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 26/02/2014.
//  Copyright 2014 Tag Games. All rights reserved.
//
#ifndef _CHILLISOURCE_NETWORKING_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_NETWORKING_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource 
{
	namespace Networking 
    {
        //--------------------------------------------------
        /// Cloud
        //--------------------------------------------------
        //--------------------------------------------------
        /// Content Download
        //--------------------------------------------------
        CS_FORWARDDECLARE_CLASS(IContentDownloader);
        CS_FORWARDDECLARE_CLASS(ContentManagementSystem);
        CS_FORWARDDECLARE_CLASS(MoContentDownloader);
        //--------------------------------------------------
        /// Http
        //--------------------------------------------------
        CS_FORWARDDECLARE_CLASS(HttpRequestSystem);
        CS_FORWARDDECLARE_CLASS(HttpRequest);
        CS_FORWARDDECLARE_STRUCT(HttpRequestDetails);
        //--------------------------------------------------
        /// IAP
        //--------------------------------------------------
        CS_FORWARDDECLARE_CLASS(IAPSystem);
	}
}

#endif
