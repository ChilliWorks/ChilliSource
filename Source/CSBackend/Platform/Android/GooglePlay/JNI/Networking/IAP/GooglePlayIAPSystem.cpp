//
//  GooglePlayIAPSystem.cpp
//  ChilliSource
//  Created by Scott Downie on 14/06/2013.
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

#ifdef CS_ANDROIDFLAVOUR_GOOGLEPLAY

#include <CSBackend/Platform/Android/GooglePlay/JNI/Networking/IAP/GooglePlayIAPSystem.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterfaceManager.h>

namespace CSBackend
{
	namespace Android
	{
		namespace
		{
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
			bool TryGetProductType(const std::vector<ChilliSource::IAPSystem::ProductRegInfo>& in_productInfos, const std::string& in_productId, ChilliSource::IAPSystem::ProductRegInfo::Type& out_type)
			{
				for(std::vector<ChilliSource::IAPSystem::ProductRegInfo>::const_iterator it = in_productInfos.begin(); it != in_productInfos.end(); ++it)
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
		GooglePlayIAPSystem::GooglePlayIAPSystem(const ChilliSource::ParamDictionary& in_params)
		{
		}
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        bool GooglePlayIAPSystem::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == ChilliSource::IAPSystem::InterfaceID || in_interfaceId == GooglePlayIAPSystem::InterfaceID;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
		void GooglePlayIAPSystem::OnInit()
		{
			m_javaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<GooglePlayIAPJavaInterface>();
			if(m_javaInterface == nullptr)
			{
				m_javaInterface = GooglePlayIAPJavaInterfaceSPtr(new GooglePlayIAPJavaInterface());
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
        void GooglePlayIAPSystem::IsPurchasingEnabled(const PurchasingEnabledDelegate& in_delegate)
        {
            CS_ASSERT(in_delegate, "Cannot have empty delegate");
        	in_delegate(m_javaInterface->IsPurchasingEnabled());
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

        	m_javaInterface->RequestProductDescriptions(in_productIds, [=](const std::vector<ChilliSource::IAPSystem::ProductDesc>& in_products, const std::vector<std::string>& in_currencyCodes, const std::vector<std::string>& in_unformattedPrices)
        	{
        	    CS_ASSERT(in_products.size() == in_currencyCodes.size() && in_products.size() == in_unformattedPrices.size(), "Disparity betewen list sizes.");

                // Construct the extra products information from the supplied lists
            	m_extraProductsInfo.clear();
        	    for (u32 i = 0; i < (u32)in_products.size(); ++i)
        	    {
        	        ExtraProductInfo extraProductInfo;
        	        extraProductInfo.m_productId = in_products[i].m_id;
        	        extraProductInfo.m_currencyCode = in_currencyCodes[i];
        	        extraProductInfo.m_unformattedPrice = in_unformattedPrices[i];
        	        m_extraProductsInfo.push_back(extraProductInfo);
        	    }

        	    if (in_delegate)
        	    {
            	    in_delegate(in_products);
        	    }
        	});
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

#endif
