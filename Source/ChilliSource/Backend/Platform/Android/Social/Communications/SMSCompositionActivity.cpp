/*
 *  SMSCompositionActivity.cpp
 *  moFlow
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Social/Communications/SMSCompositionActivity.h>

#include <ChilliSource/Backend/Platform/Android/Social/Communications/SMSCompositionJavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{
		Social::SMSCompositionActivity::SendResultDelegate SMSCompositionActivity::mCallback;

		bool SMSCompositionActivity::IsA(Core::InterfaceIDType inID) const
		{
			return inID == SMSCompositionActivity::InterfaceID || inID == Social::SMSCompositionActivity::InterfaceID;
		}

		bool SMSCompositionActivity::SupportedByDevice()
		{
			return true;
		}
		void SMSCompositionActivity::Present(const std::vector<Core::UTF8String> & inastrRecipientNumbers, const Core::UTF8String & instrContents, const SendResultDelegate & inCallback)
		{
			mCallback = inCallback;
			SMSCompositionJavaInterface::Present(inastrRecipientNumbers,instrContents);
		}
		void SMSCompositionActivity::Dismiss()
		{
			//TODO: Should this be commented back in?
			//EmailCompositionJavaInterface::Dismiss();
		}

		void SMSCompositionActivity::OnSMSClosed()
		{
			if(mCallback)
				mCallback(SMSCompositionActivity::SendResult::k_succeed);
		}

	}
}
