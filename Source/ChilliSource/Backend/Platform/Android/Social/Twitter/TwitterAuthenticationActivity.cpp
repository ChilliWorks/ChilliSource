/*
 *  TwitterAuthenticationActivity.cpp
 *  moFlow
 *
 *  Created by Robert Henning on 09/05/2012.
 *  Copyright (c) 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Social/Twitter/TwitterAuthenticationActivity.h>

#include <ChilliSource/Backend/Platform/Android/Social/Twitter/TwitterAuthenticationActivityJavaInterface.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Social/Twitter/TwitterPostSystem.h>

namespace ChilliSource
{
	namespace Android
	{
		//-----------------------------------------------
		/// Constructor
		///
		//-----------------------------------------------
		TwitterAuthenticationActivity::TwitterAuthenticationActivity() : mvUnifiedSize(1.0f,1.0f,0.0f,0.0f)
		{
			TwitterAuthenticationActivityJavaInterface::RegisterActivity(this);
		}
		//-----------------------------------------------
		/// Begin
		//-----------------------------------------------
		void TwitterAuthenticationActivity::Present()
		{
			TwitterAuthenticationActivityJavaInterface::Present();
		}
		//-----------------------------------------------
		/// Presents the authorise URL
		//-----------------------------------------------
		void TwitterAuthenticationActivity::PresentURL(const std::string& instrURL)
		{
			if(!instrURL.empty())
			{
				TwitterAuthenticationActivityJavaInterface::PresentURL(instrURL);
			}
		}
		//-----------------------------------------------
		/// Save PIN entered by user
		//-----------------------------------------------
		bool TwitterAuthenticationActivity::SavePIN(const std::string& instrPIN)
		{
			bool bResult = false;
			std::string strPIN;
			
			strPIN.assign(instrPIN);

			if(strPIN.empty() || Social::TwitterPIN::kudwTwitterPINLength != strPIN.size())
			{
				// Handle error here
				CS_LOG_ERROR("Invalid PIN length!");
				strPIN.clear();
			}
			else
			{
				bResult = true;
			}

			mstrPIN.assign(strPIN);

			return bResult;
		}
		//-----------------------------------------------
		/// Dismiss
		//-----------------------------------------------
		void TwitterAuthenticationActivity::Dismiss()
		{
		}
		//-----------------------------------------------
        /// On PIN Complete
        //-----------------------------------------------
		void TwitterAuthenticationActivity::OnPINComplete()
		{
			if(mOnPINResultDelegate)
			{
				Social::TwitterAuthenticationActivity::AuthenticationPINResult sResult;

				sResult.strPIN.assign(mstrPIN);
				mOnPINResultDelegate(sResult);
			}
			Dismiss();
		}
	}
}

