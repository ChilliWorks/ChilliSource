//
//  AmazonIAPSystem.cpp
//  Chilli Source
//
//  Created by Ian Copland on 10/12/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Android/Extensions/Amazon/AmazonIAPSystem.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Core/Base/Device.h>

namespace ChilliSource
{
	namespace Android
	{
		namespace
		{
			const std::string k_amazonPrivateKeyKey("AmazonPrivateKey");

            //---------------------------------------------------------------
            /// @author I Copland
            ///
            /// @param List of product registrations
            /// @param Product Id to find
            ///
            /// @return Whether a product reg info exists in the list
            /// with the given Id.
            //---------------------------------------------------------------
            bool ContainsProductId(const std::vector<Networking::IAPSystem::ProductRegInfo>& in_productInfos, const std::string& in_productId)
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
		AmazonIAPSystem::AmazonIAPSystem(const Core::ParamDictionary& in_params)
		{
			CS_ASSERT(in_params.HasValue(k_amazonPrivateKeyKey) == true, "Cannot create Amazon IAP system without store key - AmazonPrivateKey");
			m_privateKey = in_params.ValueForKey(k_amazonPrivateKeyKey);
		}
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        bool AmazonIAPSystem::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == Networking::IAPSystem::InterfaceID || in_interfaceId == AmazonIAPSystem::InterfaceID;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void AmazonIAPSystem::OnInit()
        {
			m_javaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<AmazonIAPJavaInterface>();
			if (m_javaInterface == nullptr)
			{
				m_javaInterface = AmazonIAPJavaInterfaceSPtr(new AmazonIAPJavaInterface(m_privateKey, Core::Device::GetUDID()));
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
			return "Amazon";
		}
        //---------------------------------------------------------------
        //---------------------------------------------------------------
		std::string AmazonIAPSystem::GetProviderName() const
		{
			return "Amazon";
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
