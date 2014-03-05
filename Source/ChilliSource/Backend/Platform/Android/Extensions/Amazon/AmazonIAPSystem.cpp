//
//  AmazonIAPSystem.cpp
//  MoFlow
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
			const std::string kstrAmazonPrivateKeyKey = "AmazonPrivateKey";

	        //---------------------------------------------------------------
	        /// Is Product ID Registered
			///
			/// @param The registered product infos.
			/// @param The product id.
			/// @return Whether or not the given product ID exists within the
			/// given product infos.
	        //---------------------------------------------------------------
	        bool IsProductIDRegistered(const std::vector<Networking::IAPProductRegInfo>& inProductRegInfos, const std::string& instrProductID)
	        {
	            for(u32 i=0; i<inProductRegInfos.size(); ++i)
	            {
	                if(inProductRegInfos[i].strID == instrProductID)
	                {
	                    return true;
	                }
	            }

	            return false;
	        }
		}
    	//---------------------------------------------------------------
    	/// Constructor
    	//---------------------------------------------------------------
		AmazonIAPSystem::AmazonIAPSystem(const Core::ParamDictionary& inParams)
		{
			mpJavaInterface = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<AmazonIAPJavaInterface>();
			if (mpJavaInterface == NULL)
			{
				std::string strPrivateKey = "";
				if (inParams.HasValue(kstrAmazonPrivateKeyKey) == true)
				{
					strPrivateKey = inParams.ValueForKey(kstrAmazonPrivateKeyKey);
				}

				mpJavaInterface = AmazonIAPJavaInterfaceSPtr(new AmazonIAPJavaInterface(strPrivateKey, Core::Device::GetUDID()));
	        	ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpJavaInterface);
			}
		}
        //---------------------------------------------------------------
        /// Register Products
        //---------------------------------------------------------------
        void AmazonIAPSystem::RegisterProducts(const std::vector<Networking::IAPProductRegInfo>& inaProducts)
        {
            maProductRegInfos = inaProducts;
        }
        //---------------------------------------------------------------
		/// Get Provider ID
        //---------------------------------------------------------------
		std::string AmazonIAPSystem::GetProviderID() const
		{
			return "Amazon";
		}
        //---------------------------------------------------------------
		/// Get Provider Name
        //---------------------------------------------------------------
		std::string AmazonIAPSystem::GetProviderName() const
		{
			return "Amazon";
		}
        //---------------------------------------------------------------
		/// Is Purchasing Enabled
        //---------------------------------------------------------------
        bool AmazonIAPSystem::IsPurchasingEnabled()
        {
        	if (mpJavaInterface != NULL)
        	{
        		return mpJavaInterface->IsPurchasingEnabled();
        	}
        	return false;
        }
        //---------------------------------------------------------------
		/// Start Listening For Transaction Updates
        //---------------------------------------------------------------
        void AmazonIAPSystem::StartListeningForTransactionUpdates(const Networking::IAPTransactionDelegate& inRequestDelegate)
        {
        	if (mpJavaInterface != NULL)
        	{
        		mpJavaInterface->StartListeningForTransactionUpdates(inRequestDelegate);
        	}
        }
        //---------------------------------------------------------------
		/// Stop Listening For Transaction Updates
        //---------------------------------------------------------------
        void AmazonIAPSystem::StopListeningForTransactionUpdates()
        {
        	if (mpJavaInterface != NULL)
        	{
        		mpJavaInterface->StopListeningForTransactionUpdates();
        	}
        }
        //---------------------------------------------------------------
		/// Request Product Descriptions
        //---------------------------------------------------------------
        void AmazonIAPSystem::RequestProductDescriptions(const std::vector<std::string>& inaProductIDs, const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
        	if (mpJavaInterface != NULL)
        	{
        		mpJavaInterface->RequestProductDescriptions(inaProductIDs, inRequestDelegate);
        	}
        }
        //---------------------------------------------------------------
        /// Request All Product Descriptions
        //---------------------------------------------------------------
        void AmazonIAPSystem::RequestAllProductDescriptions(const Networking::IAPProductDescDelegate& inRequestDelegate)
        {
            std::vector<std::string> aIDs;
            aIDs.reserve(maProductRegInfos.size());

            for(u32 i=0; i<maProductRegInfos.size(); ++i)
            {
                aIDs.push_back(maProductRegInfos[i].strID);
            }

            RequestProductDescriptions(aIDs, inRequestDelegate);
        }
        //---------------------------------------------------------------
		/// Cancel Product Descriptions Request
        //---------------------------------------------------------------
        void AmazonIAPSystem::CancelProductDescriptionsRequest()
        {
        	if (mpJavaInterface != NULL)
        	{
        		mpJavaInterface->CancelProductDescriptionsRequest();
        	}
        }
        //---------------------------------------------------------------
		/// Request Product Purchase
        //---------------------------------------------------------------
        void AmazonIAPSystem::RequestProductPurchase(const std::string& instrProductID)
        {
        	if (mpJavaInterface != NULL)
        	{
        		CS_ASSERT(IsProductIDRegistered(maProductRegInfos, instrProductID), "Products must be registered with the IAP system before purchasing.");
        		mpJavaInterface->RequestProductPurchase(instrProductID);
        	}
        }
        //---------------------------------------------------------------
		/// Close Transactione
        //---------------------------------------------------------------
        void AmazonIAPSystem::CloseTransaction(const Networking::IAPTransactionPtr& inpTransaction, const Networking::IAPTransactionCloseDelegate& inDelegate)
        {
        	if (mpJavaInterface != NULL)
        	{
        		mpJavaInterface->CloseTransaction(inpTransaction->strProductID, inpTransaction->strTransactionID, inDelegate);
        	}
        }
        //---------------------------------------------------------------
		/// Restore Managed Purchases
        //---------------------------------------------------------------
        void AmazonIAPSystem::RestoreManagedPurchases()
        {
        	if (mpJavaInterface != NULL)
        	{
        		mpJavaInterface->RestoreManagedPurchases();
        	}
        }
	}
}
