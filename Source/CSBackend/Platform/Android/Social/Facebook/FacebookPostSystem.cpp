//
// FacebookPostSystem.cpp
// Chilli Source
//
// Created by Robert Henning on 03/05/2012
// Copyright 2012 Tag Games Limited - All rights reserved
//

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Social/Facebook/FacebookPostSystem.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Social/Facebook/FacebookAuthenticationSystem.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>

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
			void PostDescToKeyValueArray(const ChilliSource::Social::FacebookPostSystem::PostDesc& in_desc, std::vector<std::string>& out_keyValues)
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
			void RequestDescToKeyValueArray(const ChilliSource::Social::FacebookPostSystem::RequestDesc& in_desc, std::vector<std::string>& out_keyValues)
			{
				out_keyValues.push_back("caption");
				out_keyValues.push_back(in_desc.m_caption);

				out_keyValues.push_back("message");
				out_keyValues.push_back(in_desc.m_description);

	            std::string recipients;
	            CSCore::StringUtils::ToCSV(in_desc.m_recipients, recipients);

	            std::string requestType = "to";

	            switch (in_desc.m_type)
	            {
	                case ChilliSource::Social::FacebookPostSystem::RequestRecipientMode::k_fixed:
	                    requestType = "to";
	                    break;
	                case ChilliSource::Social::FacebookPostSystem::RequestRecipientMode::k_optional:
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
		FacebookPostSystem::FacebookPostSystem(ChilliSource::Social::FacebookAuthenticationSystem* in_authSystem)
		: m_authSystem(in_authSystem)
		{

		}
		//----------------------------------------------------
		//----------------------------------------------------
		bool FacebookPostSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return ChilliSource::Social::FacebookPostSystem::InterfaceID == in_interfaceId || FacebookPostSystem::InterfaceID == in_interfaceId;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookPostSystem::OnInit()
		{
			m_javaInterface = m_javaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<FacebookJavaInterface>();
			CS_ASSERT(m_javaInterface != nullptr, "Must have the auth system created");
			m_javaInterface->SetPostSystem(this);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookPostSystem::Post(const PostDesc& in_desc, PostResultDelegate::Connection&& in_delegateConnection)
		{
            CS_ASSERT(m_postCompleteDelegateConnection == nullptr, "Cannot post more than once at a time");
            CS_ASSERT(m_authSystem->IsSignedIn() == true, "User must be authenticated to post");

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
            m_javaInterface->TryPostToFeed(graphPath, postParamsKeyValue);
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
			if(!m_postCompleteDelegateConnection)
			{
				return;
			}

            auto delegateConnection = std::move(m_postCompleteDelegateConnection);
            m_postCompleteDelegateConnection = nullptr;
            delegateConnection->Call(in_result);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookPostSystem::OnPostRequestComplete(PostResult in_result)
		{
			if(!m_requestCompleteDelegateConnection)
			{
				return;
			}

            auto delegateConnection = std::move(m_requestCompleteDelegateConnection);
            m_requestCompleteDelegateConnection = nullptr;
            delegateConnection->Call(in_result);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void FacebookPostSystem::OnDestroy()
		{
			m_javaInterface->SetPostSystem(nullptr);
			m_javaInterface = nullptr;
		}
	}
}

#endif
