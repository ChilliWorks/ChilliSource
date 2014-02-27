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
        /// Account Management
        //--------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(MoConnectSystem);
        //--------------------------------------------------
        /// Cloud
        //--------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(CloudStorageSystem);
        //--------------------------------------------------
        /// Content Download
        //--------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(IContentDownloader);
        CS_FORWARD_DECLARE_CLASS(ContentManagementSystem);
        CS_FORWARD_DECLARE_CLASS(MoContentDownloader);
        //--------------------------------------------------
        /// Http
        //--------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(HttpConnectionSystem);
        CS_FORWARD_DECLARE_CLASS(HttpRequest);
        CS_FORWARD_DECLARE_CLASS(HttpRequestDetails);
        //--------------------------------------------------
        /// IAP
        //--------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(IAPSystem);
        //--------------------------------------------------
        /// Metrics
        //--------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(MoMetricsSession);
        CS_FORWARD_DECLARE_CLASS(MoMetricsSystem);
        CS_FORWARD_DECLARE_CLASS(IExternalMetrics);
	}
}

#endif
