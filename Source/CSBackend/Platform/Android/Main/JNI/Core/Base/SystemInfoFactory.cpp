//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#include <ChilliSource/Core/Base/DeviceInfo.h>
#include <ChilliSource/Core/Base/ScreenInfo.h>
#include <ChilliSource/Core/Base/SystemInfo.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Base/CoreJavaInterface.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Base/DeviceJavaInterface.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Base/SystemInfoFactory.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterfaceManager.h>
#include <CSBackend/Rendering/OpenGL/Base/RenderInfoFactory.h>

#include <vector>

namespace CSBackend
{
    namespace Android
    {
		namespace
		{
			const std::string k_defaultLocale = "en_US";
			const std::string k_defaultLanguage = "en";

			/// Returns the language portion of a locale code.
			///
			/// @param in_locale
			///         The locale code.
			///
			/// @return The language code.
			///
			std::string ParseLanguageFromLocale(const std::string& in_locale) noexcept
			{
				std::vector<std::string> strLocaleBrokenUp = ChilliSource::StringUtils::Split(in_locale, "_", 0);

				if (strLocaleBrokenUp.size() > 0)
				{
					return strLocaleBrokenUp[0];
				}
				else
				{
					return k_defaultLanguage;
				}
			}

		}

		//---------------------------------------------------------
		ChilliSource::SystemInfoCUPtr SystemInfoFactory::CreateSystemInfo() noexcept
		{
		    DeviceJavaInterfaceSPtr deviceJavaInterface(new DeviceJavaInterface());
            JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(deviceJavaInterface);

            CoreJavaInterfaceSPtr coreJavaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>();
            CS_ASSERT(coreJavaInterface != nullptr, "Cannot get CoreJavaInterface!");

            // Create DeviceInfo.
            ChilliSource::DeviceInfo deviceInfo(deviceJavaInterface->GetDeviceModel(), deviceJavaInterface->GetDeviceModelType(), deviceJavaInterface->GetDeviceManufacturer(), deviceJavaInterface->GetUniqueId(), deviceJavaInterface->GetDefaultLocaleCode(), ParseLanguageFromLocale(deviceJavaInterface->GetDefaultLocaleCode()), ChilliSource::ToString(deviceJavaInterface->GetOSVersionCode()), deviceJavaInterface->GetNumberOfCores());

            // Create ScreenInfo.
            ChilliSource::Vector2 currentResolution((f32)coreJavaInterface->GetScreenWidth(), (f32)coreJavaInterface->GetScreenHeight());
            f32 screenDensity = coreJavaInterface->GetScreenDensity();
            std::vector<ChilliSource::Integer2> supportedResolutions;
            supportedResolutions.push_back(ChilliSource::Integer2((s32)currentResolution.x, (s32)currentResolution.y));
            supportedResolutions.push_back(ChilliSource::Integer2((s32)currentResolution.y, (s32)currentResolution.x));

            ChilliSource::RenderInfo renderInfo = OpenGL::RenderInfoFactory::CreateRenderInfo();
            ChilliSource::ScreenInfo screenInfo(currentResolution, screenDensity, 1.0f / screenDensity, supportedResolutions);

		    // Create SystemInfo.
		    ChilliSource::SystemInfoUPtr systemInfo(new ChilliSource::SystemInfo(deviceInfo, screenInfo, renderInfo, coreJavaInterface->GetApplicationVersionName()));

		    return std::move(systemInfo);
		}
    }
}

#endif