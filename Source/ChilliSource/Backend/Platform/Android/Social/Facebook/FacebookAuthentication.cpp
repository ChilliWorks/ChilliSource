/**
 * CFacebookAuthentication.cpp
 * moFlow
 *
 * Created by Robert Henning on 01/05/2012
 * Copyright �2012 Tag Games Limited - All rights reserved
 */

#include <ChilliSource/Platform/Android/Social/Facebook/FacebookAuthentication.h>
#include <ChilliSource/Platform/Android/Social/Facebook/FacebookJavaInterface.h>
#include <ChilliSource/Platform/Android/JavaInterface/JavaInterfaceManager.h>

namespace moFlo
{
	using namespace Social;
	using namespace Core;

	namespace AndroidPlatform
	{
		DEFINE_NAMED_INTERFACE(CFacebookAuthenticationSystem);

		CFacebookAuthenticationSystem::CFacebookAuthenticationSystem()
		: mAuthDelegate(NULL), mAuthReadDelegate(NULL), mAuthWriteDelegate(NULL)
		{
			mpJavaInterface = SHARED_PTR<CFacebookJavaInterface>(new CFacebookJavaInterface());
			mpJavaInterface->SetAuthenticationSystem(this);
	        moFlo::AndroidPlatform::CJavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpJavaInterface);
		}

		bool CFacebookAuthenticationSystem::IsA(Core::InterfaceIDType inID) const
		{
			return (inID == CFacebookAuthenticationSystem::InterfaceID) || (inID == IFacebookAuthenticationSystem::InterfaceID);
		}

		void CFacebookAuthenticationSystem::Authenticate(const DYNAMIC_ARRAY<std::string>& inastrReadPermissions, const AuthenticationCompleteDelegate& inDelegate)
		{
			mAuthDelegate = inDelegate;
			mpJavaInterface->Authenticate(inastrReadPermissions);
		}

		bool CFacebookAuthenticationSystem::IsSignedIn() const
		{
			return mpJavaInterface->IsSignedIn();
		}

		std::string CFacebookAuthenticationSystem::GetActiveToken() const
		{
			return mpJavaInterface->GetActiveToken();
		}

        void CFacebookAuthenticationSystem::AuthoriseWritePermissions(const DYNAMIC_ARRAY<std::string> & inaWritePerms, const AuthenticationCompleteDelegate& inDelegate)
        {
        	mAuthWriteDelegate = inDelegate;
        	mpJavaInterface->AuthoriseWritePermissions(inaWritePerms);
        }

        void CFacebookAuthenticationSystem::AuthoriseReadPermissions(const DYNAMIC_ARRAY<std::string> & inaReadPerms, const AuthenticationCompleteDelegate& inDelegate)
        {
        	mAuthReadDelegate = inDelegate;
        	mpJavaInterface->AuthoriseReadPermissions(inaReadPerms);
        }

        bool CFacebookAuthenticationSystem::HasPermission(const std::string& instrPermission) const
        {
        	return mpJavaInterface->HasPermission(instrPermission);
        }

		void CFacebookAuthenticationSystem::SignOut()
		{
			mpJavaInterface->SignOut();
		}

		void CFacebookAuthenticationSystem::PublishInstall()
		{
			mpJavaInterface->PublishInstall();
		}

		void CFacebookAuthenticationSystem::OnAuthenticationComplete(bool inbSuccess)
		{
			if(mAuthDelegate)
			{
	            AuthenticateResponse sResponse;
				if(inbSuccess)
				{
					sResponse.strToken = GetActiveToken();
		            sResponse.eResult = IFacebookAuthenticationSystem::AR_SUCCESS;
				}
				else
				{
		            sResponse.eResult = IFacebookAuthenticationSystem::AR_FAILED;
				}

				mAuthDelegate(sResponse);
				mAuthDelegate = NULL;
			}
		}

		void CFacebookAuthenticationSystem::OnAuthoriseReadPermissionsComplete(bool inbSuccess)
		{
			if(mAuthReadDelegate)
			{
	            AuthenticateResponse sResponse;
				if(inbSuccess)
				{
					sResponse.strToken = GetActiveToken();
		            sResponse.eResult = IFacebookAuthenticationSystem::AR_SUCCESS;
				}
				else
				{
		            sResponse.eResult = IFacebookAuthenticationSystem::AR_FAILED;
				}

				mAuthReadDelegate(sResponse);
				mAuthReadDelegate = NULL;
			}
		}

		void CFacebookAuthenticationSystem::OnAuthoriseWritePermissionsComplete(bool inbSuccess)
		{
			if(mAuthWriteDelegate)
			{
	            AuthenticateResponse sResponse;
				if(inbSuccess)
				{
					sResponse.strToken = GetActiveToken();
		            sResponse.eResult = IFacebookAuthenticationSystem::AR_SUCCESS;
				}
				else
				{
		            sResponse.eResult = IFacebookAuthenticationSystem::AR_FAILED;
				}

				mAuthWriteDelegate(sResponse);
				mAuthWriteDelegate = NULL;
			}
		}

		const SHARED_PTR<CFacebookJavaInterface>& CFacebookAuthenticationSystem::GetJavaInterface() const
		{
			return mpJavaInterface;
		}
	}
}
