/*
 *  ForwardDeclarations.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 23/05/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_NETWORKING_FORWARDDECLARATIONS_H_
#define _MOFLO_NETWORKING_FORWARDDECLARATIONS_H_

namespace ChilliSource 
{
	namespace Networking 
    {
        class IContentDownloader;
        
		class CMoConnectSystem;
		class CMoMetricsSystem;
        class CMoMetricsSession;
        class IExternalMetrics;
        class CMoContentDownloader;
        class CContentManagementSystem;
		
		struct HttpRequestDetails;
		class IHttpRequest;
		class IHttpConnectionSystem;
       
        class IIAPSystem;
	}
}

#endif
