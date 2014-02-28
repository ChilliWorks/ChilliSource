/*
 *  TwitterAuthenticationActivity.cpp
 *  moFlow
 *
 *  Created by Robert Henning on 09/05/2012.
 *  Copyright (c) 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Base/Screen.h>

#include <ChilliSource/Social/Twitter/TwitterPostSystem.h>

#include <ChilliSource/Backend/Platform/Android/JavaInterface/TwitterAuthenticationActivityJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Social/Twitter/TwitterAuthenticationActivity.h>

namespace ChilliSource
{
	namespace Android
	{
		//-----------------------------------------------
		/// Constructor
		///
		//-----------------------------------------------
		CTwitterAuthenticationActivity::CTwitterAuthenticationActivity() : mvUnifiedSize(1.0f,1.0f,0.0f,0.0f)
		{
			SCTwitterAuthenticationActivityJavaInterface::RegisterActivity(this);
		}
		//-----------------------------------------------
		/// Begin
		//-----------------------------------------------
		void CTwitterAuthenticationActivity::Present()
		{
			SCTwitterAuthenticationActivityJavaInterface::Present();
		}
		//-----------------------------------------------
		/// Presents the authorise URL
		//-----------------------------------------------
		void CTwitterAuthenticationActivity::PresentURL(const std::string& instrURL)
		{
			if(!instrURL.empty())
			{
				SCTwitterAuthenticationActivityJavaInterface::PresentURL(instrURL);
			}
		}
		//-----------------------------------------------
		/// Save PIN entered by user
		//-----------------------------------------------
		bool CTwitterAuthenticationActivity::SavePIN(const std::string& instrPIN)
		{
			bool bResult = false;
			std::string strPIN;
			
			strPIN.assign(instrPIN);

			if(strPIN.empty() || Social::TwitterPIN::kudwTwitterPINLength != strPIN.size())
			{
				// Handle error here
				ERROR_LOG("Invalid PIN length!");
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
		void CTwitterAuthenticationActivity::Dismiss()
		{
		}
		//-----------------------------------------------
        /// On PIN Complete
        //-----------------------------------------------
		void CTwitterAuthenticationActivity::OnPINComplete()
		{
			if(mOnPINResultDelegate)
			{
				ChilliSource::Social::ITwitterAuthenticationActivity::AuthenticationPINResult sResult;

				sResult.strPIN.assign(mstrPIN);
				mOnPINResultDelegate(sResult);
			}
			Dismiss();
		}
	}
}

