//
//  FacebookAuthenticationSystem.cpp
//  Chilli Source
//  Created by Robert Henning on 03/05/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#ifdef CS_ANDROIDEXTENSION_FACEBOOK

#include <CSBackend/Platform/Android/Extensions/Facebook/FacebookAuthenticationSystem.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(FacebookAuthenticationSystem);

		//----------------------------------------------------
		//----------------------------------------------------
		bool FacebookAuthenticationSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == FacebookAuthenticationSystem::InterfaceID) || (in_interfaceId == CSSocial::FacebookAuthenticationSystem::InterfaceID);
		}
        //----------------------------------------------------
        //----------------------------------------------------
        void FacebookAuthenticationSystem::OnInit()
        {
        	m_javaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<FacebookJavaInterface>();
        	if(m_javaInterface == nullptr)
        	{
        		m_javaInterface = FacebookJavaInterfaceSPtr(new FacebookJavaInterface());
        		CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_javaInterface);
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

#endif
