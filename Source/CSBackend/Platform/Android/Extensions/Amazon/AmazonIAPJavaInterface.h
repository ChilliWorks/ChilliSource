//
//  AmazonIAPJavaInterface.h
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

#ifndef _CSBACKEND_PLATFORM_ANDROID__EXTENSIONS_AMAZON_AMAZONIAPJAVAINTERFACE_H_
#define _CSBACKEND_PLATFORM_ANDROID__EXTENSIONS_AMAZON_AMAZONIAPJAVAINTERFACE_H_

#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>
#include <ChilliSource/Networking/IAP/IAPSystem.h>

namespace CSBackend
{
	namespace Android
	{
		//--------------------------------------------------------------
		/// Interface class that allows communication between the
		/// Amazon IAP system and the Java Amazon billing SDK
		///
		/// @author Ian Copland
		//--------------------------------------------------------------
		class AmazonIAPJavaInterface final : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE(AmazonIAPJavaInterface);
			//--------------------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			///
			/// @param The private key.
			/// @param The UDID
			//--------------------------------------------------------------
			AmazonIAPJavaInterface(const std::string& in_privateKey, const std::string& in_udid);
			//--------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Interface ID
			///
			/// @return whether or not this object implements the given interface.
			//--------------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return Whether the purchasing is allowed by the device/OS
            //---------------------------------------------------------------
            bool IsPurchasingEnabled();
            //---------------------------------------------------------------
			/// Calling this function will set the listener to which any
            /// transaction events are directed. This is not necessarily
            /// in response to a user action it may be previously outstanding
            /// transactions.
            ///
            /// @author Ian Copland
            ///
            /// @param Delegate
            //---------------------------------------------------------------
            void StartListeningForTransactionUpdates(const CSNetworking::IAPSystem::TransactionStatusDelegate& in_delegate);
            //---------------------------------------------------------------
            /// Prevent any more transaction updates from being triggered.
            ///
            /// @author Ian Copland
            //---------------------------------------------------------------
            void StopListeningForTransactionUpdates();
            //---------------------------------------------------------------
            /// Starts a request to the store for details of the products.
            /// These details are name, description and price
            ///
            /// @author Ian Copland
            ///
			/// @param List of product IDs to request descriptions for
            /// @param Delegate to invoke when the request completes
            //---------------------------------------------------------------
            void RequestProductDescriptions(const std::vector<std::string>& in_productIds, const CSNetworking::IAPSystem::ProductDescDelegate& in_delegate);
            //---------------------------------------------------------------
			/// Prevent the completion delegate being called for
            /// any pending product description requests and attempt to
            /// cancel the request to the store.
            ///
            /// @author Ian Copland
            //---------------------------------------------------------------
            void CancelProductDescriptionsRequest();
            //---------------------------------------------------------------
			/// Make a request to the store to purchase the item.
            /// This will trigger a call to the transaction listener delegate
            ///
            /// @author Ian Copland
            ///
            /// @param Product ID
            //---------------------------------------------------------------
            void RequestProductPurchase(const std::string& in_productId);
            //---------------------------------------------------------------
			/// Tell the store to close the transaction as complete.
            /// NOTE: This should only be called after the goods have been
            /// awarded.
            ///
            /// @author Ian Copland
            ///
            /// @param Product ID
            /// @param Transaction ID
            /// @param Delegate to call when closed (either with success or failure)
            //---------------------------------------------------------------
            void CloseTransaction(const std::string& in_productId, const std::string& in_transactionId, const CSNetworking::IAPSystem::TransactionCloseDelegate& in_delegate);
            //---------------------------------------------------------------
            /// Request that the store trigger new purchase requests for
            /// owned non-consumable items
            ///
            /// @author Ian Copland
            //---------------------------------------------------------------
            void RestoreManagedPurchases();

            //---Only called by JNI - Otherwise would be private
            //---------------------------------------------------------------
    		/// Called when product descriptions request completes
            ///
            /// @author Ian Copland
            ///
            /// @param List of product descriptions
            //---------------------------------------------------------------
            void OnProductDescriptionsRequestComplete(const std::vector<CSNetworking::IAPSystem::ProductDesc>& in_products);
            //---------------------------------------------------------------
    		/// Called when transaction status changes
            ///
            /// @author Ian Copland
            ///
            /// @param Status ID
            /// @param Transaction description
            //---------------------------------------------------------------
            void OnTransactionStatusUpdated(u32 in_statusId, const CSNetworking::IAPSystem::Transaction& in_transaction);
            //---------------------------------------------------------------
    		/// Called when transaction close request completes
            ///
            /// @author Ian Copland
            ///
            /// @param Product ID
            /// @param Transaction ID
            //---------------------------------------------------------------
            void OnTransactionClosed(const std::string& in_productId, const std::string& in_transactionId);

		private:

            CSNetworking::IAPSystem::ProductDescDelegate m_productsRequestDelegate;
            CSNetworking::IAPSystem::TransactionStatusDelegate m_transactionStatusDelegate;
            CSNetworking::IAPSystem::TransactionCloseDelegate m_transactionCloseDelegate;
		};
	}
}

#endif

#endif

#endif
