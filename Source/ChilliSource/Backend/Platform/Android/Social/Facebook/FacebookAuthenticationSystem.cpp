//
// FacebookAuthenticationSystem.cpp
// Chilli Source
//
// Created by Robert Henning on 03/05/2012
// Copyright 2012 Tag Games Limited - All rights reserved
//

#include <ChilliSource/Backend/Platform/Android/Social/Facebook/FacebookAuthenticationSystem.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(FacebookAuthenticationSystem);

		//----------------------------------------------------
		//----------------------------------------------------
		bool FacebookAuthenticationSystem::IsA(Core::InterfaceIDType in_interfaceID) const
		{
			return (in_interfaceID == FacebookAuthenticationSystem::InterfaceID) || (in_interfaceID == Social::FacebookAuthenticationSystem::InterfaceID);
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
		void FacebookAuthenticationSystem::Authenticate(const std::vector<std::string>& in_readPermissions, const AuthenticationCompleteDelegate& in_delegate)
		{
            CS_ASSERT(m_authDelegate == nullptr, "Cannot authenticate more than once at the same time");

            m_authDelegate = in_delegate;

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
        void FacebookAuthenticationSystem::AuthoriseWritePermissions(const std::vector<std::string>& in_writePermissions, const AuthenticationCompleteDelegate& in_delegate)
        {
            CS_ASSERT(m_authWriteDelegate == nullptr, "Only one write permission request can be active at a time");
            CS_ASSERT(IsSignedIn() == true, "Must be authenticated");

        	m_authWriteDelegate = in_delegate;
        	m_javaInterface->AuthoriseWritePermissions(in_writePermissions);
        }
		//----------------------------------------------------
		//----------------------------------------------------
        void FacebookAuthenticationSystem::AuthoriseReadPermissions(const std::vector<std::string>& in_readPermissions, const AuthenticationCompleteDelegate& in_delegate)
        {
            CS_ASSERT(m_authReadDelegate == nullptr, "Only one read permission request can be active at a time");
            CS_ASSERT(IsSignedIn() == true, "Must be authenticated");

        	m_authReadDelegate = in_delegate;
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
			if(m_authDelegate)
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

				m_authDelegate(response);
				m_authDelegate = nullptr;
			}
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookAuthenticationSystem::OnAuthoriseReadPermissionsComplete(bool in_success)
		{
			if(m_authReadDelegate)
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

				m_authReadDelegate(response);
				m_authReadDelegate = nullptr;
			}
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookAuthenticationSystem::OnAuthoriseWritePermissionsComplete(bool in_success)
		{
			if(m_authWriteDelegate)
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

				m_authWriteDelegate(response);
				m_authWriteDelegate = nullptr;
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
