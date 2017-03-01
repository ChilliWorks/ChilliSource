//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#include <ChilliSource/Core/Base/DeviceInfo.h>
#include <ChilliSource/Core/Base/ScreenInfo.h>
#include <ChilliSource/Core/Base/SystemInfo.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <CSBackend/Platform/RPi/Core/Base/Screen.h>
#include <CSBackend/Platform/RPi/Core/Base/SystemInfoFactory.h>
#include <CSBackend/Rendering/OpenGL/Base/RenderInfoFactory.h>

#include <algorithm>
#include <vector>

namespace CSBackend
{
    namespace RPi
    {
        //--------------------------------------------------------------------------------
        ChilliSource::SystemInfoCUPtr SystemInfoFactory::CreateSystemInfo() noexcept
        {
            // Create DeviceInfo.
            ChilliSource::DeviceInfo deviceInfo(k_deviceModel, k_deviceModelType, k_deviceManufacturer, k_deviceUdid, GetLocale(), ParseLanguageFromLocale(GetLocale()), GetOSVersion(), GetNumberOfCPUCores());

            // Create ScreenInfo.
            ChilliSource::ScreenInfo screenInfo(GetScreenResolution(), 1.0f, 1.0f, GetSupportedResolutions());

			//Create RenderInfo
			ChilliSource::RenderInfo renderInfo = OpenGL::RenderInfoFactory::CreateRenderInfo();

            // Create SystemInfo.
            ChilliSource::SystemInfoUPtr systemInfo(new ChilliSource::SystemInfo(deviceInfo, screenInfo, renderInfo, ""));

            return std::move(systemInfo);
        }

    }
}

#endif
