//
//  ApkExpansionContentProvider.cpp
//  Chilli Source
//  Created by Ian Copland on 24/06/2015.
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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileSystem.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>

#include <jni.h>

extern "C"
{
    //------------------------------------------------------------------------------
    /// Extracts the file at the given source file path inside the Apk Expansion to
    /// the given literal destination file. Called from Java.
    ///
    /// @author Ian Copland
    ///
    /// @param in_environment - The JNI Environment.
    /// @param in_this - The java object that called the method.
    //------------------------------------------------------------------------------
	bool Java_com_chilliworks_chillisource_core_ApkExpansionContentProvider_extractApkExpansionFile(JNIEnv* in_environment, jobject in_this, jstring in_sourceFilePath, jstring in_destFilePath);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool Java_com_chilliworks_chillisource_core_ApkExpansionContentProvider_extractApkExpansionFile(JNIEnv* in_environment, jobject in_this, jstring in_sourceFilePath, jstring in_destFilePath)
{
    auto fileSystem = CSCore::Application::Get()->GetFileSystem();

    auto absSourceFilePath = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(in_sourceFilePath);
    auto absDestFilePath = CSBackend::Android::JavaUtils::CreateSTDStringFromJString(in_destFilePath);
    auto absPackageDirectoryPath = fileSystem->GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_package);
    auto absChilliSourceDirectoryPath = fileSystem->GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_chilliSource);

    CSCore::StorageLocation storageLocation = CSCore::StorageLocation::k_none;
    std::string sourceFilePath;

    if (CSCore::StringUtils::StartsWith(absSourceFilePath, absPackageDirectoryPath, false) == true)
    {
        sourceFilePath = absSourceFilePath.substr(absPackageDirectoryPath.length());
        storageLocation = CSCore::StorageLocation::k_package;
    }
    else if (CSCore::StringUtils::StartsWith(absSourceFilePath, absChilliSourceDirectoryPath, false) == true)
    {
        sourceFilePath = absSourceFilePath.substr(absChilliSourceDirectoryPath.length());
        storageLocation = CSCore::StorageLocation::k_chilliSource;
    }
    else
    {
        return false;
    }

    return fileSystem->CopyFile(storageLocation, sourceFilePath, CSCore::StorageLocation::k_root, absDestFilePath);
}

#endif

#endif
