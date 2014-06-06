//
// FacebookAuthenticationSystem.cpp
// Chilli Source
//
// Created by Robert Henning on 03/05/2012
// Copyright 2012 Tag Games Limited - All rights reserved
//

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Social/Facebook/FacebookAuthenticationSystem.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(FacebookAuthenticationSystem);

		//----------------------------------------------------
		//----------------------------------------------------
		bool FacebookAuthenticationSystem::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == FacebookAuthenticationSystem::InterfaceID) || (in_interfaceId == Social::FacebookAuthenticationSystem::InterfaceID);
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void FacebookAuthenticationSystem::OnInit()
        {
        	m_javaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<FacebookJavaInterface>();

        	if(m_javaInterface == nullptr)
        	{
        		m_javaInterface = FacebookJavaInterfaceSPtr(new FacebookJavaInterface());
        		ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_javaInterface);
        	}

        	m_javaInterface->SetAuthenticationSystem(this);
        }
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookAuthenticationSystem::Authenticate(const std::vector<std::string>& in_readPermissions, AuthenticationCompleteDelegate::Connection&& in_delegateConnection)
		{
            CS_ASSERT(m_authDelegateConnection == nullptr, "Cannot authenticate more than once at the same time");

            m_authDelegateConnection = std::move(in_delegateConnection);

            m_javaInterface->Authenticate(in_readPermissions);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		bool FacebookAuthenticationSystem::IsSignedIn() const
		{
			return m_javaInterface->IsSignedIn();
		}
		//----------------------------------------------------
		//----------------------------------------------------
		std::string FacebookAuthenticationSystem::GetActiveToken() const
		{
			return m_javaInterface->GetActiveToken();
		}
		//----------------------------------------------------
		//----------------------------------------------------
        void FacebookAuthenticationSystem::AuthoriseWritePermissions(const std::vector<std::string>& in_writePermissions, AuthenticationCompleteDelegate::Connection&& in_delegateConnection)
        {
            CS_ASSERT(m_authWriteDelegateConnection == nullptr, "Only one write permission request can be active at a time");
            CS_ASSERT(IsSignedIn() == true, "Must be authenticated");

            m_authWriteDelegateConnection = std::move(in_delegateConnection);

        	m_javaInterface->AuthoriseWritePermissions(in_writePermissions);
        }
		//----------------------------------------------------
		//----------------------------------------------------
        void FacebookAuthenticationSystem::AuthoriseReadPermissions(const std::vector<std::string>& in_readPermissions, AuthenticationCompleteDelegate::Connection&& in_delegateConnection)
        {
            CS_ASSERT(m_authReadDelegateConnection == nullptr, "Only one read permission request can be active at a time");
            CS_ASSERT(IsSignedIn() == true, "Must be authenticated");

            m_authReadDelegateConnection = std::move(in_delegateConnection);

        	m_javaInterface->AuthoriseReadPermissions(in_readPermissions);
        }
		//----------------------------------------------------
		//----------------------------------------------------
        bool FacebookAuthenticationSystem::HasPermission(const std::string& in_permission) const
        {
        	return m_javaInterface->HasPermission(in_permission);
        }
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookAuthenticationSystem::SignOut()
		{
			m_javaInterface->SignOut();
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookAuthenticationSystem::OnAuthenticationComplete(bool in_success)
		{
			if(m_authDelegateConnection)
			{
	            AuthenticateResponse response;
				if(in_success)
				{
					response.m_token = GetActiveToken();
					response.m_result = AuthenticateResult::k_success;
				}
				else
				{
					response.m_result = AuthenticateResult::k_failed;
				}

	            auto delegateConnection = std::move(m_authDelegateConnection);
	            m_authDelegateConnection = nullptr;
	            delegateConnection->Call(response);
			}
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookAuthenticationSystem::OnAuthoriseReadPermissionsComplete(bool in_success)
		{
			if(m_authReadDelegateConnection)
			{
	            AuthenticateResponse response;
				if(in_success)
				{
					response.m_token = GetActiveToken();
					response.m_result = AuthenticateResult::k_success;
				}
				else
				{
					response.m_result = AuthenticateResult::k_failed;
				}

	            auto delegateConnection = std::move(m_authReadDelegateConnection);
	            m_authReadDelegateConnection = nullptr;
	            delegateConnection->Call(response);
			}
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookAuthenticationSystem::OnAuthoriseWritePermissionsComplete(bool in_success)
		{
			if(m_authWriteDelegateConnection)
			{
	            AuthenticateResponse response;
				if(in_success)
				{
					response.m_token = GetActiveToken();
					response.m_result = AuthenticateResult::k_success;
				}
				else
				{
					response.m_result = AuthenticateResult::k_failed;
				}

	            auto delegateConnection = std::move(m_authWriteDelegateConnection);
	            m_authWriteDelegateConnection = nullptr;
	            delegateConnection->Call(response);
			}
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void FacebookAuthenticationSystem::OnDestroy()
        {
        	m_javaInterface->SetAuthenticationSystem(nullptr);
	        m_javaInterface = nullptr;
        }
	}
}

#endif
