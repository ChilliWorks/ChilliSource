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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Activity.h>
#include <ChilliSource/Core/String/UTF8String.h>

#include <functional>
#include <vector>

namespace ChilliSource
{
	namespace Social
    {
        //----------------------------------------------------
        /// An activity for displaying the platform specific
        /// SMS composition interface.
        //----------------------------------------------------
		class SMSCompositionActivity : public Core::Activity
        {
		public:
			CS_DECLARE_NAMEDTYPE(SMSCompositionActivity);
            //----------------------------------------------------
            /// An enum describing the results from the activity.
            //----------------------------------------------------
			enum class SendResult
            {
                k_succeed,
                k_failed,
                k_cancelled
			};
            //----------------------------------------------------
            /// Delegate
            //----------------------------------------------------
			typedef std::function<void(SendResult)> SendResultDelegate;
            //----------------------------------------------------
            /// Creates a new instance of the SYS Composition
            /// activity.
            ///
            /// @author I Copland
            ///
            /// @return The new instance of the activity.
            //----------------------------------------------------
            static SMSCompositionActivityUPtr Create();
            //----------------------------------------------------
            /// Presents the SMS Composition activity with the given
            /// message contents.
            ///
            /// @author I Copland
            ///
            /// @param A list of recipient phone numbers.
            /// @param The contents of the message.
            /// @param The completion callback.
            //----------------------------------------------------
			virtual void Present(const std::vector<Core::UTF8String> & in_recipientNumbers, const Core::UTF8String & in_contents, const SendResultDelegate & in_callback) = 0;
            //----------------------------------------------------
            /// Dismisses the activity if it was displayed.
            ///
            /// @author I Copland
            //----------------------------------------------------
			virtual void Dismiss() = 0;
            //----------------------------------------------------
            /// Destructor
            ///
            /// @author I Copland
            //----------------------------------------------------
            virtual ~SMSCompositionActivity(){};
        protected:
            //----------------------------------------------------
            /// Protected constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            //----------------------------------------------------
            SMSCompositionActivity(){};
		};
		
	}
}

#endif
