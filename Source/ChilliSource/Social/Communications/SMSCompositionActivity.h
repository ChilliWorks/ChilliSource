/*
 *  SMSCompositionActivity.h
 *  moFlow
 *
 *  Created by Stuart McGaw on 08/06/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_SOCIAL_COMMUNICATIONS_SMSCOMPOSITIONSCREEN_H_
#define _MOFLO_SOCIAL_COMMUNICATIONS_SMSCOMPOSITIONSCREEN_H_

#include <ChilliSource/Core/Base/Activity.h>
#include <ChilliSource/Core/String/UTF8String.h>
#include <ChilliSource/Core/Event/GenericEvent.h>
#include <vector>

namespace moFlo {
	namespace Social {
		
		
		
		class ISMSCompositionActivity : public IActivity{
		public:

			DECLARE_NAMED_INTERFACE(ISMSCompositionActivity);
			
			enum class SendResult
            {
                k_succeed,
                k_failed,
                k_cancelled
			};
			
			typedef fastdelegate::FastDelegate1<SendResult> SendResultDelegate;
			
			virtual void Present(const DYNAMIC_ARRAY<CUTF8String> & inastrRecipientNumbers, const CUTF8String & instrContents, const SendResultDelegate & inCallback) = 0;
			virtual void Dismiss() = 0;
		};
		
	}
}

#endif