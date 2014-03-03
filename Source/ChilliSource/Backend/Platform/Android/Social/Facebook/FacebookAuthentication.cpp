/**
 * CFacebookAuthentication.cpp
 * moFlow
 *
 * Created by Robert Henning on 01/05/2012
 * Copyright �2012 Tag Games Limited - All rights reserved
 */

#include <ChilliSource/Backend/Platform/Android/Social/Facebook/FacebookAuthentication.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Social/Facebook/FacebookJavaInterface.h>

namespace ChilliSource
{
	using namespace Social;
	using namespace Core;

	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(CFacebookAuthenticationSystem);

		CFacebookAuthenticationSystem::CFacebookAuthenticationSystem()
		: mAuthDelegate(NULL), mAuthReadDelegate(NULL), mAuthWriteDelegate(NULL)
		{
			mpJavaInterface = std::shared_ptr<CFacebookJavaInterface>(new CFacebookJavaInterface());
			mpJavaInterface->SetAuthenticationSystem(this);
	        ChilliSource::Android::CJavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpJavaInterface);
		}

		bool CFacebookAuthenticationSystem::IsA(Core::InterfaceIDType inID) const
		{
			return (inID == CFacebookAuthenticationSystem::InterfaceID) || (inID == FacebookAuthenticationSystem::InterfaceID);
		}

		void CFacebookAuthenticationSystem::Authenticate(const std::vector<std::string>& inastrReadPermissions, const AuthenticationCompleteDelegate& inDelegate)
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

        void CFacebookAuthenticationSystem::AuthoriseWritePermissions(const std::vector<std::string> & inaWritePerms, const AuthenticationCompleteDelegate& inDelegate)
        {
        	mAuthWriteDelegate = inDelegate;
        	mpJavaInterface->AuthoriseWritePermissions(inaWritePerms);
        }

        void CFacebookAuthenticationSystem::AuthoriseReadPermissions(const std::vector<std::string> & inaReadPerms, const AuthenticationCompleteDelegate& inDelegate)
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
		            sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_success;
				}
				else
				{
		            sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_failed;
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
		            sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_success;
				}
				else
				{
		            sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_failed;
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
		            sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_success;
				}
				else
				{
		            sResponse.eResult = FacebookAuthenticationSystem::AuthenticateResult::k_failed;
				}

				mAuthWriteDelegate(sResponse);
				mAuthWriteDelegate = NULL;
			}
		}

		const std::shared_ptr<CFacebookJavaInterface>& CFacebookAuthenticationSystem::GetJavaInterface() const
		{
			return mpJavaInterface;
		}
	}
}
