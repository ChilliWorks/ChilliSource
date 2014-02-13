//
//  IAPSystem.h
//  MoFlow
//
//  Created by Scott Downie on 10/06/2013.
//  Copyright (c) 2013 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_NETWORKING_IAPSYSTEM_H_
#define _MOFLOW_NETWORKING_IAPSYSTEM_H_

#include <ChilliSource/Core/System.h>
#include <ChilliSource/Core/ParamDictionary.h>

namespace moFlo
{
	namespace Networking
    {
        struct IAPProductDesc
        {
            std::string strID;
            std::string strName;
            std::string strDescription;
            std::string strFormattedPrice;
            std::string strCountryCode;
        };
        
        struct IAPProductRegInfo
        {
            enum Type
            {
                MANAGED,
                UNMANAGED
            };
            
            std::string strID;
            Type eType;
        };
        
        typedef fastdelegate::FastDelegate1<const DYNAMIC_ARRAY<IAPProductDesc>&> IAPProductDescDelegate;
        

        struct IAPTransaction
        {
            std::string strProductID;
            std::string strTransactionID;
            std::string strReceipt;
            
            enum Status
            {
                SUCCEEDED,
                FAILED,
                CANCELLED,
                REFUNDED,
                RESTORED,
                RESUMED
            };
        };
        
        typedef SHARED_PTR<IAPTransaction> IAPTransactionPtr;
        typedef fastdelegate::FastDelegate2<IAPTransaction::Status, const IAPTransactionPtr&> IAPTransactionDelegate;
        typedef fastdelegate::FastDelegate2<const std::string&, const std::string&> IAPTransactionCloseDelegate;
        
        
		class IIAPSystem : public moFlo::Core::ISystem
        {
        public:
            DECLARE_NAMED_INTERFACE(IIAPSystem);
            
            //---------------------------------------------------------------
            /// Create
            ///
            /// Creates a new platform specfic instance of the IAP system.
            ///
            /// @param A param dictionary that contains platform specific
            /// parameters for the IAP system. The possible values are:
            /// 	GooglePlayPublicKey  	The public key used for connecting
            ///								to the Google Play store.
            ///		AmazonPrivateKey		The private key used to encrypt
            ///								the on disk Amazon IAP cache.
            //---------------------------------------------------------------
            static IIAPSystem* Create(const Core::ParamDictionary& inParams);

            //---------------------------------------------------------------
            /// Is A
            ///
            /// @return Whether this implements the passed in interface id.
            //---------------------------------------------------------------
            bool IsA(Core::InterfaceIDType inInterfaceID) const;
            
            //---------------------------------------------------------------
            /// Register Products
            ///
            /// Inform the system of which products are available for
            /// purchase and whether they are managed or unmanaged
            ///
            /// @param List of products
            //---------------------------------------------------------------
            virtual void RegisterProducts(const DYNAMIC_ARRAY<IAPProductRegInfo>& inaProducts) = 0;
            //---------------------------------------------------------------
			/// Get Provider ID
			///
			/// @return The ID off the IAP provider as a string.
            //---------------------------------------------------------------
			virtual std::string GetProviderID() const = 0;
            //---------------------------------------------------------------
			/// Get Provider Name
			///
			/// @return A displayable name for the IAP Provider.
            //---------------------------------------------------------------
			virtual std::string GetProviderName() const = 0;
            //---------------------------------------------------------------
			/// Is Purchasing Enabled
			///
			/// @return Whether the purchasing is allowed by the device/OS
            //---------------------------------------------------------------
            virtual bool IsPurchasingEnabled() = 0;
            
            //---------------------------------------------------------------
			/// Start Listening For Transaction Updates
			///
			/// Calling this function will set the listener to which any
            /// transaction events are directed. This is not necessarily
            /// in response to a user action it may be previously outstanding
            /// transactions.
            ///
            /// @param Delegate
            //---------------------------------------------------------------
            virtual void StartListeningForTransactionUpdates(const IAPTransactionDelegate& inRequestDelegate) = 0;
            //---------------------------------------------------------------
			/// Stop Listening For Transaction Updates
            ///
            /// Prevent any more transaction uppdates from being triggered.
            //---------------------------------------------------------------
            virtual void StopListeningForTransactionUpdates() = 0;
            
            //---------------------------------------------------------------
			/// Request All Product Descriptions
			///
            /// Starts a request to the store for details of all registered
            /// products. These details are name, description and price
            ///
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            virtual void RequestAllProductDescriptions(const IAPProductDescDelegate& inRequestDelegate) = 0;
            //---------------------------------------------------------------
			/// Request Product Descriptions
			///
            /// Starts a request to the store for details of the products.
            /// These details are name, description and price
            ///
			/// @param List of product IDs to request descriptions for
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            virtual void RequestProductDescriptions(const DYNAMIC_ARRAY<std::string>& inaProductIDs, const IAPProductDescDelegate& inRequestDelegate) = 0;
            //---------------------------------------------------------------
			/// Cancel Product Descriptions Request
			///
			/// Prevent the completion delegate being called for
            /// any pending product description requests and attempt to
            /// cancel the request to the store.
            //---------------------------------------------------------------
            virtual void CancelProductDescriptionsRequest() = 0;
            
            //---------------------------------------------------------------
			/// Request Product Purchase
			///
			/// Make a request to the store to purchase the item.
            /// This will trigger a call to the transaction listener delegate
            ///
            /// @param Product ID
            //---------------------------------------------------------------
            virtual void RequestProductPurchase(const std::string& instrProductID) = 0;
            
            //---------------------------------------------------------------
			/// Close Transaction
			///
			/// Tell the store to close the transaction as complete.
            /// NOTE: This should only be called after the goods have been
            /// awarded.
            ///
            /// @param Transaction to close
            /// @param Delegate to call when closed successfully
            //---------------------------------------------------------------
            virtual void CloseTransaction(const IAPTransactionPtr& inpTransaction, const IAPTransactionCloseDelegate& inDelegate) = 0;
            
            //---------------------------------------------------------------
			/// Restore Managed Purchases
			///
            /// Request that the store trigger new purchase requests for
            /// owned non-consumable items
            //---------------------------------------------------------------
            virtual void RestoreManagedPurchases() = 0;
        };
    }
}


#endif
