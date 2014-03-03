/*
 *  SMSCompositionActivity.cpp
 *  moFlow
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Social/Communications/SMSCompositionActivity.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/SMSCompositionJavaInterface.h>

namespace ChilliSource
{
	namespace Android
	{
		Social::SMSCompositionActivity::SendResultDelegate CSMSCompositionActivity::mCallback;

		bool CSMSCompositionActivity::IsA(Core::InterfaceIDType inID) const
		{
			return inID == CSMSCompositionActivity::InterfaceID || inID == Social::SMSCompositionActivity::InterfaceID;
		}

		bool CSMSCompositionActivity::SupportedByDevice()
		{
			return true;
		}

		CSMSCompositionActivity::CSMSCompositionActivity()
		{

		}
		CSMSCompositionActivity::~CSMSCompositionActivity()
		{

		}
		void CSMSCompositionActivity::Present(const std::vector<Core::UTF8String> & inastrRecipientNumbers, const Core::UTF8String & instrContents, const SendResultDelegate & inCallback)
		{
			mCallback = inCallback;
			SCSMSCompositionJavaInterface::Present(inastrRecipientNumbers,instrContents);
		}
		void CSMSCompositionActivity::Dismiss()
		{
			//TODO: Should this be commented back in?
			//SCEmailCompositionJavaInterface::Dismiss();
		}

		void CSMSCompositionActivity::OnSMSClosed()
		{
			if(mCallback)
				mCallback(SMSCompositionActivity::SendResult::k_succeed);
		}

	}
}
