//
//  IAPSystem.h
//  Chilli Source
//  Created by Scott Downie on 10/06/2013.
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

#ifndef _CHILLISOURCE_NETWORKING_IAP_IAPSYSTEM_H_
#define _CHILLISOURCE_NETWORKING_IAP_IAPSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <functional>

namespace ChilliSource
{
	namespace Networking
    {
        //----------------------------------------------------------------------------------
        /// System that allows purchasing of IAPs from
        /// the backend App Store. System allows product info
        /// to be requested in ordrer to build displays and then
        /// for products to be purchased. NOTE: Must StartListeningForTransactionUpdates()
        /// before any purchases are made as this can be called with previously incomplete
        /// transactions.
        ///
        /// @author S Downie
        //----------------------------------------------------------------------------------
		class IAPSystem : public Core::AppSystem
        {
        public:
            
            //------------------------------------------------------
            /// Holds the information relating to a single
            /// transaction.
            ///
            /// @author S Downie
            //------------------------------------------------------
            struct Transaction
            {
                std::string m_productId;
                std::string m_transactionId;
                std::string m_receipt;
                
                enum class Status
                {
                    k_succeeded,
                    k_failed,
                    k_cancelled,
                    k_refunded,
                    k_restored,
                    k_resumed
                };
            };
            using TransactionSPtr = std::shared_ptr<Transaction>;
            //------------------------------------------------------
            /// Holds the description of a product as obtained
            /// from the store
            ///
            /// @author S Downie
            //------------------------------------------------------
            struct ProductDesc
            {
                std::string m_id;
                std::string m_name;
                std::string m_description;
                std::string m_formattedPrice;
                std::string m_countryCode;
            };
            //------------------------------------------------------
            /// Used to describe a product when registering it.
            /// Products can be either managed or unmanaged
            ///
            /// @author S Downie
            //------------------------------------------------------
            struct ProductRegInfo
            {
                enum class Type
                {
                    k_managed,
                    k_unmanaged
                };
                
                std::string m_id;
                Type m_type;
            };
            //------------------------------------------------------
            /// Delegate that is triggered when a request for product
            /// informations completes.
            ///
            /// @author S Downie
            ///
            /// @param List of product descriptions
            //------------------------------------------------------
            using ProductDescDelegate = std::function<void(const std::vector<ProductDesc>&)>;
            //------------------------------------------------------
            /// Delegate triggered when the status of a transaction
            /// changes i.e. from pending to success.
            ///
            /// @author S Downie
            ///
            /// @param Transaction status
            /// @param Transaction
            //------------------------------------------------------
            using TransactionStatusDelegate = std::function<void(Transaction::Status, const TransactionSPtr&)>;
            //------------------------------------------------------
            /// Delegate triggered when the transaction is closed.
            ///
            /// @author S Downie
            ///
            /// @param Product ID
            /// @param Transaction ID
            /// @param Success or failure
            //------------------------------------------------------
            using TransactionCloseDelegate = std::function<void(const std::string&, const std::string&, bool)>;
            
            CS_DECLARE_NAMEDTYPE(IAPSystem);
            
            //---------------------------------------------------------------
            /// Creates a new platform specfic instance of the IAP system.
            ///
            /// @author S Downie
            ///
            /// @param A param dictionary that contains platform specific
            /// parameters for the IAP system. The possible values are:
            /// 	GooglePlayPublicKey  	The public key used for connecting
            ///								to the Google Play store.
            ///		AmazonPrivateKey		The private key used to encrypt
            ///								the on disk Amazon IAP cache.
            //---------------------------------------------------------------
            static IAPSystemUPtr Create(const Core::ParamDictionary& in_params);
            //---------------------------------------------------------------
            /// Inform the system of which products are available for
            /// purchase and whether they are managed or unmanaged
            ///
            /// @author S Downie
            ///
            /// @param List of products
            //---------------------------------------------------------------
            virtual void RegisterProducts(const std::vector<ProductRegInfo>& in_productInfos) = 0;
            //---------------------------------------------------------------
			/// @author S Downie
			///
			/// @return The ID off the IAP provider as a string.
            //---------------------------------------------------------------
			virtual std::string GetProviderID() const = 0;
            //---------------------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether the purchasing is allowed by the device/OS
            //---------------------------------------------------------------
            virtual bool IsPurchasingEnabled() = 0;
            //---------------------------------------------------------------
			/// Calling this function will set the listener to which any
            /// transaction events are directed. This is not necessarily
            /// in response to a user action it may be previously outstanding
            /// transactions.
            ///
            /// @author S Downie
            ///
            /// @param Transaction status changed delegate
            //---------------------------------------------------------------
            virtual void StartListeningForTransactionUpdates(const TransactionStatusDelegate& in_delegate) = 0;
            //---------------------------------------------------------------
            /// Prevent any more transaction uppdates from being triggered.
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            virtual void StopListeningForTransactionUpdates() = 0;
            //---------------------------------------------------------------
            /// Starts a request to the store for details of all registered
            /// products. These details are name, description and price
            ///
            /// @author S Downie
            ///
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            virtual void RequestAllProductDescriptions(const ProductDescDelegate& in_delegate) = 0;
            //---------------------------------------------------------------
            /// Starts a request to the store for details of the products.
            /// These details are name, description and price
            ///
            /// @author S Downie
            ///
			/// @param List of product IDs to request descriptions for
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            virtual void RequestProductDescriptions(const std::vector<std::string>& in_productIds, const ProductDescDelegate& in_delegate) = 0;
            //---------------------------------------------------------------
			/// Prevent the completion delegate being called for
            /// any pending product description requests and attempt to
            /// cancel the request to the store.
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            virtual void CancelProductDescriptionsRequest() = 0;
            //---------------------------------------------------------------
			/// Make a request to the store to purchase the item.
            /// This will trigger a call to the transaction listener delegate
            ///
            /// @author S Downie
            ///
            /// @param Product ID
            //---------------------------------------------------------------
            virtual void RequestProductPurchase(const std::string& in_productId) = 0;
            //---------------------------------------------------------------
			/// Tell the store to close the transaction as complete.
            /// NOTE: This should only be called after the goods have been
            /// awarded.
            ///
            /// @author S Downie
            ///
            /// @param Transaction to close
            /// @param Delegate to call when closed (either with failure or success)
            //---------------------------------------------------------------
            virtual void CloseTransaction(const TransactionSPtr& in_transaction, const TransactionCloseDelegate& in_delegate) = 0;
            //---------------------------------------------------------------
            /// Request that the store trigger new purchase requests for
            /// owned non-consumable items
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            virtual void RestoreManagedPurchases() = 0;
            //---------------------------------------------------------------
            /// Virtual destructor
            ///
            /// @author S Downie
            //---------------------------------------------------------------
            virtual ~IAPSystem(){}
        };
    }
}


#endif
