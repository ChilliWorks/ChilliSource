//
//  ETC1ImageProvider.cpp
//  moFlow
//
//  Created by Ian Copland on 09/11/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Image/ETC1ImageProvider.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>

const std::string ETC1Extension("pkm");

namespace ChilliSource
{
	//----------------------------------------------------------------
	/// Is A
	//----------------------------------------------------------------
	bool CETC1ImageProvider::IsA(Core::InterfaceIDType inInterfaceID) const
	{
		return inInterfaceID == moFlo::Core::IResourceProvider::InterfaceID;
	}
	//----------------------------------------------------------------
	/// Can Create Resource Of Kind
	//----------------------------------------------------------------
	bool CETC1ImageProvider::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
	{
		return inInterfaceID == Core::CImage::InterfaceID;
	}
	//----------------------------------------------------------------
	/// Can Create Resource From File With Extension
	//----------------------------------------------------------------
	bool CETC1ImageProvider::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
	{
		return (inExtension == ETC1Extension);
	}
	//----------------------------------------------------------------
	/// Create Resource From File
	//----------------------------------------------------------------
	bool CETC1ImageProvider::CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)
	{
		return CreateImageFromFile(ineStorageLocation, inFilePath, Core::CImage::Format::k_default, outpResource);
	}
	//----------------------------------------------------------------
	/// Create Image From File
	//----------------------------------------------------------------
	bool CETC1ImageProvider::CreateImageFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::CImage::Format ineFormat, Core::ResourcePtr& outpResource)
	{
		//ensure the extension is correct.
		if (moFlo::Core::CStringUtils::EndsWith(inFilePath, ETC1Extension, true) == false)
			return false;

		Core::FileStreamPtr pImageFile = Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, inFilePath, Core::FileMode::k_readBinary);

		if (pImageFile != NULL && pImageFile->IsOpen() == true && pImageFile->IsBad() == false)
		{
			//ETC1 Format is in big endian format. As all the platforms we support are little endian we will have to convert the data to little endian.
			//read the header.
			ETC1Header sHeader;
			pImageFile->Read((s8*)sHeader.abyPKMTag, sizeof(u8) * 6);

			pImageFile->Read((s8*)&sHeader.wNumberOfMipmaps, sizeof(u16));
			sHeader.wNumberOfMipmaps = moFlo::Core::CUtils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.wNumberOfMipmaps));

			pImageFile->Read((s8*)&sHeader.wTextureWidth, sizeof(u16));
			sHeader.wTextureWidth = moFlo::Core::CUtils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.wTextureWidth));

			pImageFile->Read((s8*)&sHeader.wTextureHeight, sizeof(u16));
			sHeader.wTextureHeight = moFlo::Core::CUtils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.wTextureHeight));

			pImageFile->Read((s8*)&sHeader.wOriginalWidth, sizeof(u16));
			sHeader.wOriginalWidth = moFlo::Core::CUtils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.wOriginalWidth));

			pImageFile->Read((s8*)&sHeader.wOriginalHeight, sizeof(u16));
			sHeader.wOriginalHeight = moFlo::Core::CUtils::Endian2ByteSwap(reinterpret_cast<u16*>(&sHeader.wOriginalHeight));

			//get the size of the rest of the data
			const u32 kstrHeaderSize = 16;
			pImageFile->SeekG(0, Core::SeekDir::k_end);
			u32 dwDataSize = pImageFile->TellG() - kstrHeaderSize;
			pImageFile->SeekG(kstrHeaderSize, Core::SeekDir::k_beginning);

			//read the rest of the data
			u8* pData = new u8[dwDataSize];
			pImageFile->Read((s8*)pData, dwDataSize);
			pImageFile->Close();

			//setup the output image
			Core::CImage* outpImage = (Core::CImage*)outpResource.get();
			outpImage->SetFormat(Core::CImage::Format::k_RGBA8888);
			outpImage->SetData(pData);
			outpImage->SetDataLength(dwDataSize);
			outpImage->SetWidth(sHeader.wTextureWidth);
			outpImage->SetHeight(sHeader.wTextureHeight);
			outpImage->SetCompression(Core::ImageCompression::k_ETC1);

			return true;
		}

		return false;
	}
}
