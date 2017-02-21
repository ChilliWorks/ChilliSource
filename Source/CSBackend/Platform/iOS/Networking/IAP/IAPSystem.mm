//
//  IAPSystem.mm
//  ChilliSource
//  Created by Scott Downie on 12/06/2013.
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

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Networking/IAP/IAPSystem.h>

#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>

#import <ChilliSource/Core/Base/Application.h>
#import <ChilliSource/Core/Cryptographic/BaseEncoding.h>
#import <ChilliSource/Core/Delegate/MakeDelegate.h>
#import <ChilliSource/Core/String/StringUtils.h>
#import <ChilliSource/Core/Threading/TaskScheduler.h>

namespace CSBackend
{
    namespace iOS
    {
        namespace
        {
            //---------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param List of product registrations
            /// @param Product Id to find
            ///
            /// @return Whether a product reg info exists in the list
            /// with the given Id.
            //---------------------------------------------------------------
            bool ContainsProductId(const std::vector<ChilliSource::IAPSystem::ProductRegInfo>& in_productInfos, const std::string& in_productId)
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
        
        CS_DEFINE_NAMEDTYPE(IAPSystem);
        
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        bool IAPSystem::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == ChilliSource::IAPSystem::InterfaceID || in_interfaceId == IAPSystem::InterfaceID;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::OnInit()
        {
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& in_taskContext)
            {
                m_storeKitSystem = [[StoreKitIAPSystem alloc] init];
            });
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::RegisterProducts(const std::vector<ProductRegInfo>& in_productInfos)
        {
            CS_ASSERT(in_productInfos.empty() == false, "Must register at least one product");
            m_productRegInfos = in_productInfos;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        std::string IAPSystem::GetProviderID() const
        {
            return "AppleAppStore";
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::IsPurchasingEnabled(const PurchasingEnabledDelegate& in_delegate)
        {
            CS_ASSERT(in_delegate, "Cannot have empty delegate");
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& in_taskContext)
            {
                in_delegate([m_storeKitSystem isPurchasingEnabled]);
            });
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::StartListeningForTransactionUpdates(const TransactionStatusDelegate& in_delegate)
        {
            CS_ASSERT(in_delegate != nullptr, "Cannot have null transaction delegate");
            m_transactionStatusDelegate = in_delegate;
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& in_taskContext)
            {
                [m_storeKitSystem startListeningForTransactions:ChilliSource::MakeDelegate(this, &IAPSystem::OnTransactionUpdate)];
            });
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::OnTransactionUpdate(NSString* in_productID, StoreKitIAP::TransactionResult in_result, SKPaymentTransaction* in_skTransaction)
        {
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& in_taskContext)
            {
                if(m_transactionStatusDelegate == nullptr)
                    return;
                
                bool hasReceipt = false;
                Transaction::Status result = Transaction::Status::k_failed;
                switch(in_result)
                {
                    case StoreKitIAP::TransactionResult::k_succeeded:
                        hasReceipt = true;
                        result = Transaction::Status::k_succeeded;
                        break;
                    case StoreKitIAP::TransactionResult::k_failed:
                        result = Transaction::Status::k_failed;
                        break;
                    case StoreKitIAP::TransactionResult::k_cancelled:
                        result = Transaction::Status::k_cancelled;
                        break;
                    case StoreKitIAP::TransactionResult::k_restored:
                        hasReceipt = true;
                        result = Transaction::Status::k_restored;
                        break;
                    case StoreKitIAP::TransactionResult::k_resumed:
                        hasReceipt = true;
                        result = Transaction::Status::k_resumed;
                        break;
                }
                
                TransactionSPtr transaction(new Transaction());
                transaction->m_productId = [NSStringUtils newUTF8StringWithNSString:in_productID];
                transaction->m_transactionId = [NSStringUtils newUTF8StringWithNSString:in_skTransaction.transactionIdentifier];
                if(hasReceipt)
                {
                    CS_ASSERT([in_skTransaction.transactionReceipt length] < static_cast<NSUInteger>(std::numeric_limits<u32>::max()), "Transaction receipt is too large, cannot exceed "
                              + ChilliSource::ToString(std::numeric_limits<u32>::max()) + " bytes.");
                    u32 length = static_cast<u32>([in_skTransaction.transactionReceipt length]);
                    
                    transaction->m_receipt = ChilliSource::BaseEncoding::Base64Encode((s8*)[in_skTransaction.transactionReceipt bytes], length);
                }
                
                m_transactionStatusDelegate(result, transaction);
            });
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::StopListeningForTransactionUpdates()
        {
            m_transactionStatusDelegate = nullptr;
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& in_taskContext)
            {
                [m_storeKitSystem stopListeningForTransactions];
            });
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::RequestProductDescriptions(const std::vector<std::string>& in_productIds, const ProductDescDelegate& in_delegate)
        {
            CS_ASSERT(in_productIds.empty() == false, "Cannot request no product descriptions");
            CS_ASSERT(in_delegate != nullptr, "Cannot have null product description delegate");
            CS_ASSERT(m_productDescDelegate == nullptr, "Only 1 product description request can be active at a time");
            
            m_productDescDelegate = in_delegate;
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& in_taskContext)
            {
                NSMutableSet* idSet = [[NSMutableSet alloc] initWithCapacity:in_productIds.size()];
                
                for (u32 i=0; i<in_productIds.size(); ++i)
                {
                    NSString* projectId = [NSStringUtils newNSStringWithUTF8String:in_productIds[i]];
                    [idSet addObject:projectId];
                    [projectId release];
                }
                
                [m_storeKitSystem requestProducts:idSet forDelegate:ChilliSource::MakeDelegate(this, &IAPSystem::OnProductDescriptionRequestComplete)];
                
                [idSet release];
            });
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::RequestAllProductDescriptions(const ProductDescDelegate& in_delegate)
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
        void IAPSystem::OnProductDescriptionRequestComplete(NSArray* in_products)
        {
            std::vector<ProductDesc> descriptions;
            
            if(in_products != nil)
            {
                NSNumberFormatter* formatter = [[NSNumberFormatter alloc] init];
                [formatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
                [formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
                
                for(SKProduct* product in in_products)
                {
                    ProductDesc description;
                    description.m_id = [NSStringUtils newUTF8StringWithNSString:product.productIdentifier];
                    description.m_name = [NSStringUtils newUTF8StringWithNSString:product.localizedTitle];
                    description.m_description = [NSStringUtils newUTF8StringWithNSString:product.localizedDescription];
                    
                    [formatter setLocale:product.priceLocale];
                    description.m_formattedPrice = [NSStringUtils newUTF8StringWithNSString:[formatter stringFromNumber:product.price]];
                    
                    NSLocale* storeLocale = product.priceLocale;
                    description.m_countryCode = [NSStringUtils newUTF8StringWithNSString:(NSString*)CFLocaleGetValue((CFLocaleRef)storeLocale, kCFLocaleCountryCode)];
                    
                    descriptions.push_back(description);
                }
                
                [formatter release];
            }
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& in_taskContext)
            {
                if(m_productDescDelegate == nullptr)
                    return;

                m_productDescDelegate(descriptions);
                m_productDescDelegate = nullptr;
            });
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::CancelProductDescriptionsRequest()
        {
            m_productDescDelegate = nullptr;
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& in_taskContext)
            {
                [m_storeKitSystem cancelProductsRequest];
            });
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::RequestProductPurchase(const std::string& in_productId)
        {
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& in_taskContext)
            {
                CS_ASSERT(ContainsProductId(m_productRegInfos, in_productId), "Products must be registered with the IAP system before purchasing");
                NSString* productID = [NSStringUtils newNSStringWithUTF8String:in_productId];
                [m_storeKitSystem requestPurchaseWithProductID:productID andQuantity:1];
                [productID release];
            });
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::CloseTransaction(const TransactionSPtr& in_transaction, const TransactionCloseDelegate& in_delegate)
        {
            CS_ASSERT(in_delegate != nullptr, "Cannot have null transaction close delegate");
            CS_ASSERT(m_transactionCloseDelegate == nullptr, "Only 1 transaction can be closed at a time");
            
            m_transactionCloseDelegate = in_delegate;
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& in_taskContext)
            {
                NSString* transactionId = [NSStringUtils newNSStringWithUTF8String:in_transaction->m_transactionId];
                [m_storeKitSystem closeTransactionWithID:transactionId];
                [transactionId release];
                
                ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& in_taskContext)
                {
                    m_transactionCloseDelegate(in_transaction->m_productId, in_transaction->m_transactionId, true);
                    m_transactionCloseDelegate = nullptr;
                });
            });
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::RestoreManagedPurchases()
        {
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& in_taskContext)
            {
                [m_storeKitSystem restoreNonConsumablePurchases];
            });
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        std::vector<IAPSystem::ExtraProductInfo> IAPSystem::GetExtraProductInfo() const
        {
            CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread() == true, "This can only be called on the main thread.");
            
            ExtraProductInfo extraProductInfo;
            std::vector<ExtraProductInfo> extraProductsInfo;

            NSArray* products = [m_storeKitSystem getNativeStoreData];
            if (products != nil)
            {
                for (SKProduct* product in products)
                {
                    extraProductInfo.m_productId = [NSStringUtils newUTF8StringWithNSString:product.productIdentifier];
                    extraProductInfo.m_unformattedPrice = [NSStringUtils newUTF8StringWithNSString:[product.price stringValue]];

                    NSNumberFormatter* Formatter = [[[NSNumberFormatter alloc] init] autorelease];
                    [Formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
                    [Formatter setLocale:product.priceLocale];
                    extraProductInfo.m_currencyCode = [NSStringUtils newUTF8StringWithNSString:[Formatter currencyCode]];
                    
                    extraProductsInfo.push_back(extraProductInfo);
                }
            }
            
            return extraProductsInfo;
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void IAPSystem::OnDestroy()
        {
            auto storeKitSystem = m_storeKitSystem;
            m_storeKitSystem = nil;
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [storeKitSystem](const ChilliSource::TaskContext& in_taskContext)
            {
                [storeKitSystem release];
            });
            
            m_productDescDelegate = nullptr;
            m_transactionStatusDelegate = nullptr;
            m_transactionCloseDelegate = nullptr;
            m_productRegInfos.clear();
            m_productRegInfos.shrink_to_fit();
        }
    }
}

#endif
