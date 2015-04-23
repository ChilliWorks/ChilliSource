//
//  ApkExpansionDownloader.cpp
//  Chilli Source
//  Created by Ian Copland on 22/04/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#include <CSBackend/Platform/Android/GooglePlay/JNI/Networking/ApkExpansion/ApkExpansionDownloader.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaClassDef.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaVirtualMachine.h>

namespace CSBackend
{
	namespace Android
	{
        CS_DEFINE_NAMEDTYPE(ApkExpansionDownloader);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        ApkExpansionDownloaderUPtr ApkExpansionDownloader::Create()
        {
            return ApkExpansionDownloaderUPtr(new ApkExpansionDownloader());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ApkExpansionDownloader::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (ApkExpansionDownloader::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ApkExpansionDownloader::OnInit()
        {
            //define and create the java class.
            JavaClassDef classDef("com/chilliworks/chillisource/googleplay/networking/ApkExpansionDownloader");
            m_javaClass = JavaClassUPtr(new JavaClass(classDef));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ApkExpansionDownloader::OnDestroy()
        {
            m_javaClass.reset();
        }
	}
}

#endif

#endif
