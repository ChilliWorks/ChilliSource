//
//  AmazonIAPSystem.cpp
//  Chilli Source
//  Created by Ian Copland on 10/12/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#ifdef CS_ANDROIDEXTENSION_AMAZON

#include <CSBackend/Platform/Android/Extensions/Amazon/AmazonIAPSystem.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Device.h>

namespace CSBackend
{
	namespace Android
	{
		namespace
		{
			const std::string k_amazonPrivateKeyKey("AmazonPrivateKey");

            //---------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param List of product registrations
            /// @param Product Id to find
            ///
            /// @return Whether a product reg info exists in the list
            /// with the given Id.
            //---------------------------------------------------------------
            bool ContainsProductId(const std::vector<CSNetworking::IAPSystem::ProductRegInfo>& in_productInfos, const std::string& in_productId)
            {
                for(u32 i=0; i<in_productInfos.size(); ++i)
                {
                    if(in_productInfos[i].m_id == in_productId)
                    {
                        return true;
                    }
                }

                return false;
            }
		}

		CS_DEFINE_NAMEDTYPE(AmazonIAPSystem);

    	//---------------------------------------------------------------
    	//---------------------------------------------------------------
		AmazonIAPSystem::AmazonIAPSystem(const CSCore::ParamDictionary& in_params)
		{
			CS_ASSERT(in_params.HasKey(k_amazonPrivateKeyKey) == true, "Cannot create Amazon IAP system without store key - AmazonPrivateKey");
			m_privateKey = in_params.GetValue(k_amazonPrivateKeyKey);
		}
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        bool AmazonIAPSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == CSNetworking::IAPSystem::InterfaceID || in_interfaceId == AmazonIAPSystem::InterfaceID;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPSystem::OnInit()
        {
			m_javaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<AmazonIAPJavaInterface>();
			if (m_javaInterface == nullptr)
			{
				CSCore::Device* device = CSCore::Application::Get()->GetSystem<CSCore::Device>();
				m_javaInterface = AmazonIAPJavaInterfaceSPtr(new AmazonIAPJavaInterface(m_privateKey, device->GetUDID()));
	        	JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_javaInterface);
			}
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPSystem::RegisterProducts(const std::vector<ProductRegInfo>& in_productInfos)
        {
        	CS_ASSERT(in_productInfos.empty() == false, "Must register at least one product");
            m_productRegInfos = in_productInfos;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
		std::string AmazonIAPSystem::GetProviderID() const
		{
			return "AmazonAppStore";
		}
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        bool AmazonIAPSystem::IsPurchasingEnabled()
        {
        	return m_javaInterface->IsPurchasingEnabled();
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPSystem::StartListeningForTransactionUpdates(const TransactionStatusDelegate& in_delegate)
        {
        	CS_ASSERT(in_delegate != nullptr, "Cannot have null transaction delegate");
        	m_javaInterface->StartListeningForTransactionUpdates(in_delegate);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPSystem::StopListeningForTransactionUpdates()
        {
        	m_javaInterface->StopListeningForTransactionUpdates();
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPSystem::RequestProductDescriptions(const std::vector<std::string>& in_productIds, const ProductDescDelegate& in_delegate)
        {
            CS_ASSERT(in_productIds.empty() == false, "Cannot request no product descriptions");
            CS_ASSERT(in_delegate != nullptr, "Cannot have null product description delegate");
        	m_javaInterface->RequestProductDescriptions(in_productIds, in_delegate);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPSystem::RequestAllProductDescriptions(const ProductDescDelegate& in_delegate)
        {
            std::vector<std::string> productIds;
            productIds.reserve(m_productRegInfos.size());

            for(u32 i=0; i<m_productRegInfos.size(); ++i)
            {
            	productIds.push_back(m_productRegInfos[i].m_id);
            }

            RequestProductDescriptions(productIds, in_delegate);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPSystem::CancelProductDescriptionsRequest()
        {
        	m_javaInterface->CancelProductDescriptionsRequest();
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPSystem::RequestProductPurchase(const std::string& in_productId)
        {
        	CS_ASSERT(ContainsProductId(m_productRegInfos, in_productId), "Products must be registered with the IAP system before purchasing");
        	m_javaInterface->RequestProductPurchase(in_productId);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPSystem::CloseTransaction(const TransactionSPtr& in_transaction, const TransactionCloseDelegate& in_delegate)
        {
        	m_javaInterface->CloseTransaction(in_transaction->m_productId, in_transaction->m_transactionId, in_delegate);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPSystem::RestoreManagedPurchases()
        {
        	m_javaInterface->RestoreManagedPurchases();
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPSystem::OnDestroy()
        {
        	m_javaInterface = nullptr;
            m_productRegInfos.clear();
            m_productRegInfos.shrink_to_fit();
        }
	}
}

#endif

#endif
