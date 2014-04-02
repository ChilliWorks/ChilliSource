//
//  GooglePlayIAPSystem.cpp
//  Chilli Source
//
//  Created by Scott Downie on 14/06/2013.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifdef CS_ANDROIDEXTENSION_GOOGLEPLAY

#include <ChilliSource/Backend/Platform/Android/Extensions/GooglePlay/GooglePlayIAPSystem.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>

namespace ChilliSource
{
	namespace Android
	{
		namespace
		{
			const std::string k_playStorePublicKeyKey("GooglePlayPublicKey");

			//---------------------------------------------------------------
			/// Try and locate the product in the list and return its
			/// type - managed or unmanaged
			///
			/// @author S Downie
			///
			/// @param List of registered products
			/// @param Product ID to find
			/// @param [Out] Type if found
			///
			/// @return Whether product was found
			//---------------------------------------------------------------
			bool TryGetProductType(const std::vector<Networking::IAPSystem::ProductRegInfo>& in_productInfos, const std::string& in_productId, Networking::IAPSystem::ProductRegInfo::Type& out_type)
			{
				for(std::vector<Networking::IAPSystem::ProductRegInfo>::const_iterator it = in_productInfos.begin(); it != in_productInfos.end(); ++it)
				{
					if(it->m_id == in_productId)
					{
						out_type = it->m_type;
						return true;
					}
				}

				return false;
			}
		}

		CS_DEFINE_NAMEDTYPE(GooglePlayIAPSystem);

        //---------------------------------------------------------------
        //---------------------------------------------------------------
		GooglePlayIAPSystem::GooglePlayIAPSystem(const Core::ParamDictionary& in_params)
		{
			CS_ASSERT(in_params.HasValue(k_playStorePublicKeyKey) == true, "Cannot create GooglePlay IAP system without Play store public key - GooglePlayPublicKey");
			m_publicKey = in_params.ValueForKey(k_playStorePublicKeyKey);
		}
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        bool GooglePlayIAPSystem::IsA(Core::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == Networking::IAPSystem::InterfaceID || in_interfaceId == GooglePlayIAPSystem::InterfaceID;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
		void GooglePlayIAPSystem::OnInit()
		{
			m_javaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<GooglePlayIAPJavaInterface>();
			if(m_javaInterface == nullptr)
			{
				m_javaInterface = GooglePlayIAPJavaInterfaceSPtr(new GooglePlayIAPJavaInterface(m_publicKey));
				JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(m_javaInterface);
			}
		}
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::RegisterProducts(const std::vector<ProductRegInfo>& in_productInfos)
        {
        	CS_ASSERT(in_productInfos.empty() == false, "Must register at least one product");
            m_productRegInfos = in_productInfos;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
		std::string GooglePlayIAPSystem::GetProviderID() const
		{
			return "GooglePlayV3";
		}
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        bool GooglePlayIAPSystem::IsPurchasingEnabled()
        {
        	return m_javaInterface->IsPurchasingEnabled();
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::StartListeningForTransactionUpdates(const TransactionStatusDelegate& in_delegate)
        {
        	CS_ASSERT(in_delegate != nullptr, "Cannot listen for transactions with null delegate");
        	m_javaInterface->StartListeningForTransactionUpdates(in_delegate);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::StopListeningForTransactionUpdates()
        {
        	m_javaInterface->StopListeningForTransactionUpdates();
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::RequestProductDescriptions(const std::vector<std::string>& in_productIds, const ProductDescDelegate& in_delegate)
        {
            CS_ASSERT(in_productIds.empty() == false, "Cannot request no product descriptions");
            CS_ASSERT(in_delegate != nullptr, "Cannot have null product description delegate");

        	m_javaInterface->RequestProductDescriptions(in_productIds, in_delegate);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::RequestAllProductDescriptions(const ProductDescDelegate& in_delegate)
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
        void GooglePlayIAPSystem::CancelProductDescriptionsRequest()
        {
        	m_javaInterface->CancelProductDescriptionsRequest();
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::RequestProductPurchase(const std::string& in_productId)
        {
        	ProductRegInfo::Type type;
        	if(!TryGetProductType(m_productRegInfos, in_productId, type))
        	{
        		CS_LOG_FATAL("Cannot find IAP product with ID " + in_productId + ". Please register it");
        		return;
        	}

        	m_javaInterface->RequestProductPurchase(in_productId, type);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::CloseTransaction(const TransactionSPtr& in_transaction, const TransactionCloseDelegate& in_delegate)
        {
        	CS_ASSERT(in_delegate != nullptr, "Cannot have null transaction close delegate");

        	m_javaInterface->CloseTransaction(in_transaction->m_productId, in_transaction->m_transactionId, in_delegate);
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::RestoreManagedPurchases()
        {
        	m_javaInterface->RestoreManagedPurchases();
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void GooglePlayIAPSystem::OnDestroy()
        {
        	m_javaInterface = nullptr;
            m_productRegInfos.clear();
            m_productRegInfos.shrink_to_fit();
        }
	}
}

#endif
