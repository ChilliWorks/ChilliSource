//
//  FacebookPostSystem.cpp
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

#include <CSBackend/Platform/Android/Main/JNI/Social/Facebook/FacebookPostSystem.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Main/JNI/Social/Facebook/FacebookAuthenticationSystem.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace CSBackend
{
	namespace Android
	{
		namespace
		{
			//----------------------------------------------------
			/// @author R Henning
			///
			/// @param Post description
			/// @param [Out] Key value array in the form key, value, key, value
			//----------------------------------------------------
			void PostDescToKeyValueArray(const ChilliSource::FacebookPostSystem::PostDesc& in_desc, std::vector<std::string>& out_keyValues)
			{
				out_keyValues.push_back("link");
				out_keyValues.push_back(in_desc.m_url);

				out_keyValues.push_back("picture");
				out_keyValues.push_back(in_desc.m_picUrl);

				out_keyValues.push_back("name");
				out_keyValues.push_back(in_desc.m_name);

				out_keyValues.push_back("caption");
				out_keyValues.push_back(in_desc.m_caption);

				out_keyValues.push_back("description");
				out_keyValues.push_back(in_desc.m_description);
			}
			//----------------------------------------------------
			/// @author A Mackie
			///
			/// @param Request description
			/// @param [Out] Key value array in the form key, value, key, value
			//----------------------------------------------------
			void RequestDescToKeyValueArray(const ChilliSource::FacebookPostSystem::RequestDesc& in_desc, std::vector<std::string>& out_keyValues)
			{
				out_keyValues.push_back("caption");
				out_keyValues.push_back(in_desc.m_caption);

				out_keyValues.push_back("message");
				out_keyValues.push_back(in_desc.m_description);

	            std::string recipients;
	            ChilliSource::StringUtils::ToCSV(in_desc.m_recipients, recipients);

	            std::string requestType = "to";

	            switch (in_desc.m_type)
	            {
	                case ChilliSource::FacebookPostSystem::RequestRecipientMode::k_fixed:
	                    requestType = "to";
	                    break;
	                case ChilliSource::FacebookPostSystem::RequestRecipientMode::k_optional:
	                    requestType = "suggestions";
	                    break;
	            }

	            out_keyValues.push_back(requestType);
	            out_keyValues.push_back(recipients);
			}
		}

		CS_DEFINE_NAMEDTYPE(FacebookPostSystem);

		//----------------------------------------------------
		//----------------------------------------------------
		bool FacebookPostSystem::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return ChilliSource::FacebookPostSystem::InterfaceID == in_interfaceId || FacebookPostSystem::InterfaceID == in_interfaceId;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookPostSystem::OnInit()
		{
        	m_javaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<FacebookJavaInterface>();
        	if(m_javaInterface == nullptr)
        	{
        		m_javaInterface = FacebookJavaInterfaceSPtr(new FacebookJavaInterface());
        		CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_javaInterface);
        	}

			m_javaInterface->SetPostSystem(this);

            // Cache the Facebook authentication system.
            m_authSystem = ChilliSource::Application::Get()->GetSystem<ChilliSource::FacebookAuthenticationSystem>();
		}
		//----------------------------------------------------
		//----------------------------------------------------
        void FacebookPostSystem::OnDestroy()
        {
			m_javaInterface->SetPostSystem(nullptr);
			m_javaInterface = nullptr;

			m_authSystem = nullptr;
        }
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookPostSystem::Post(const PostDesc& in_desc, PostResultDelegate::Connection&& in_delegateConnection)
		{
            ChilliSource::TaskScheduler* taskScheduler = ChilliSource::Application::Get()->GetTaskScheduler();

            CS_ASSERT(m_postCompleteDelegateConnection == nullptr, "Cannot post more than once at a time");
            CS_ASSERT(taskScheduler && taskScheduler->IsMainThread(), "You must post in the main thread");

            m_postCompleteDelegateConnection = std::move(in_delegateConnection);

            //If we aren't posting to anyone we are posting to the
            //signed in user feed.
            std::string graphPath = "me/feed";
            if(!in_desc.m_to.empty())
            {
            	graphPath = in_desc.m_to + "/feed";
            }

            std::vector<std::string> postParamsKeyValue;
            PostDescToKeyValueArray(in_desc, postParamsKeyValue);

            // Post to the Java interface.
            m_javaInterface->Post(graphPath, postParamsKeyValue);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookPostSystem::SendRequest(const RequestDesc& in_desc, PostResultDelegate::Connection&& in_delegateConnection)
		{
            CS_ASSERT(m_requestCompleteDelegateConnection == nullptr, "Cannot request more than once at a time");
            CS_ASSERT(m_authSystem->IsSignedIn() == true, "User must be authenticated to request");

            m_requestCompleteDelegateConnection = std::move(in_delegateConnection);

            std::vector<std::string> requestParamsKeyValue;
            RequestDescToKeyValueArray(in_desc, requestParamsKeyValue);
            m_javaInterface->TryPostRequest(requestParamsKeyValue);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookPostSystem::OnPostToFeedComplete(PostResult in_result)
		{
			if (m_postCompleteDelegateConnection != nullptr)
			{
	            auto delegateConnection = std::move(m_postCompleteDelegateConnection);
	            m_postCompleteDelegateConnection = nullptr;
	            delegateConnection->Call(in_result);
			}
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookPostSystem::OnPostRequestComplete(PostResult in_result)
		{
			if (m_requestCompleteDelegateConnection != nullptr)
			{
	            auto delegateConnection = std::move(m_requestCompleteDelegateConnection);
	            m_requestCompleteDelegateConnection = nullptr;
	            delegateConnection->Call(in_result);
			}
		}
	}
}

#endif
