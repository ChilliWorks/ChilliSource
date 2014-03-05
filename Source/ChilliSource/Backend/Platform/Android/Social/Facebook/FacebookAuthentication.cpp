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
		CS_DEFINE_NAMEDTYPE(FacebookAuthenticationSystem);

		FacebookAuthenticationSystem::FacebookAuthenticationSystem()
		: mAuthDelegate(NULL), mAuthReadDelegate(NULL), mAuthWriteDelegate(NULL)
		{
			mpJavaInterface = std::shared_ptr<CFacebookJavaInterface>(new CFacebookJavaInterface());
			mpJavaInterface->SetAuthenticationSystem(this);
	        ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpJavaInterface);
		}

		bool FacebookAuthenticationSystem::IsA(Core::InterfaceIDType inID) const
		{
			return (inID == FacebookAuthenticationSystem::InterfaceID) || (inID == Social::FacebookAuthenticationSystem::InterfaceID);
		}

		void FacebookAuthenticationSystem::Authenticate(const std::vector<std::string>& inastrReadPermissions, const AuthenticationCompleteDelegate& inDelegate)
		{
			mAuthDelegate = inDelegate;
			mpJavaInterface->Authenticate(inastrReadPermissions);
		}

		bool FacebookAuthenticationSystem::IsSignedIn() const
		{
			return mpJavaInterface->IsSignedIn();
		}

		std::string FacebookAuthenticationSystem::GetActiveToken() const
		{
			return mpJavaInterface->GetActiveToken();
		}

        void FacebookAuthenticationSystem::AuthoriseWritePermissions(const std::vector<std::string> & inaWritePerms, const AuthenticationCompleteDelegate& inDelegate)
        {
        	mAuthWriteDelegate = inDelegate;
        	mpJavaInterface->AuthoriseWritePermissions(inaWritePerms);
        }

        void FacebookAuthenticationSystem::AuthoriseReadPermissions(const std::vector<std::string> & inaReadPerms, const AuthenticationCompleteDelegate& inDelegate)
        {
        	mAuthReadDelegate = inDelegate;
        	mpJavaInterface->AuthoriseReadPermissions(inaReadPerms);
        }

        bool FacebookAuthenticationSystem::HasPermission(const std::string& instrPermission) const
        {
        	return mpJavaInterface->HasPermission(instrPermission);
        }

		void FacebookAuthenticationSystem::SignOut()
		{
			mpJavaInterface->SignOut();
		}

		void FacebookAuthenticationSystem::PublishInstall()
		{
			mpJavaInterface->PublishInstall();
		}

		void FacebookAuthenticationSystem::OnAuthenticationComplete(bool inbSuccess)
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

		void FacebookAuthenticationSystem::OnAuthoriseReadPermissionsComplete(bool inbSuccess)
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

		void FacebookAuthenticationSystem::OnAuthoriseWritePermissionsComplete(bool inbSuccess)
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

		const std::shared_ptr<FacebookJavaInterface>& FacebookAuthenticationSystem::GetJavaInterface() const
		{
			return mpJavaInterface;
		}
	}
}
