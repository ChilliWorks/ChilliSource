/*
 *  SMSCompositionActivity.cpp
 *  moFlow
 *
 *  Created by Steven Hendrie on 20/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include "SMSCompositionActivity.h"
#include "Platform/Android/JavaInterface/SMSCompositionJavaInterface.h"

namespace ChilliSource
{
	namespace Android
	{
		ChilliSource::Social::ISMSCompositionActivity::SendResultDelegate CSMSCompositionActivity::mCallback;

		bool CSMSCompositionActivity::IsA(Core::InterfaceIDType inID) const
		{
			return inID == CSMSCompositionActivity::InterfaceID;
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
			mCallback = NULL;
		}
		void CSMSCompositionActivity::Present(const std::vector<CUTF8String> & inastrRecipientNumbers, const CUTF8String & instrContents, const SendResultDelegate & inCallback)
		{
			DEBUG_LOG("<<<<<<<<<< CSMSCompositionActivity::Present 1 >>>>>>>>>");
			mCallback = inCallback;
			DEBUG_LOG("<<<<<<<<<< CSMSCompositionActivity::Present 2 >>>>>>>>>");
			SCSMSCompositionJavaInterface::Present(inastrRecipientNumbers,instrContents);
			DEBUG_LOG("<<<<<<<<<< CSMSCompositionActivity::Present 3 >>>>>>>>>");
		}
		void CSMSCompositionActivity::Dismiss()
		{
			//SCEmailCompositionJavaInterface::Dismiss();
		}

		void CSMSCompositionActivity::OnSMSClosed()
		{
			if(mCallback)
				mCallback(ISMSCompositionActivity::SR_SUCCEED);
		}

	}
}
