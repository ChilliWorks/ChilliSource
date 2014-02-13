//
//  BulletCollisionMeshLoader.cpp
//  moFlow
//
//  Created by Ian Copland on 29/05/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#include <moFlo/Physics3D/BulletPhysics/BulletCollisionMeshLoader.h>
#include <moFlo/Physics3D/BulletPhysics/BulletCollisionMesh.h>
#include <moFlo/Core/TaskScheduler.h>
#include <moFlo/Core/Application.h>
#include <moFlo/Core/FileIO/FileSystem.h>
#include <moFlo/Core/FileIO/FileStream.h>


namespace moFlo
{
    namespace Physics3D
    {
        const std::string kstrMoCollisionExtension("mocollision");
        
        //----------------------------------------------------------
        /// Constructor
        //----------------------------------------------------------
        CBulletCollisionMeshLoader::CBulletCollisionMeshLoader()
        {
        }
        //----------------------------------------------------------
        /// Is A
        //----------------------------------------------------------
        bool CBulletCollisionMeshLoader::IsA(Core::InterfaceIDType inInterfaceID) const
        {
            return (inInterfaceID == IResourceProvider::InterfaceID);
        }
        //----------------------------------------------------------
        /// Can Create Resource of Kind
        //----------------------------------------------------------
        bool CBulletCollisionMeshLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
        {
            return (inInterfaceID == CBulletCollisionMesh::InterfaceID);
        }
        //----------------------------------------------------------
        /// Can Create Resource From File With Extension
        //----------------------------------------------------------
        bool CBulletCollisionMeshLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
        {
            return (inExtension == kstrMoCollisionExtension);
        }
        //----------------------------------------------------------
        /// Destructor
        //----------------------------------------------------------
        CBulletCollisionMeshLoader::~CBulletCollisionMeshLoader()
        {
        }
        //----------------------------------------------------------
        /// Create Resource From File
        //----------------------------------------------------------
        bool CBulletCollisionMeshLoader::CreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilePath, Core::ResourcePtr& outpResource)
        {
            BulletCollisionMeshPtr pMesh = SHARED_PTR_CAST<CBulletCollisionMesh>(outpResource);
			return CreateCollisionMeshFromFile(ineStorageLocation, instrFilePath, pMesh);
        }
        //----------------------------------------------------------
        /// Async Create Resource From File
        //----------------------------------------------------------
        bool CBulletCollisionMeshLoader::AsyncCreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilePath, Core::ResourcePtr& outpResource)
        {
            BulletCollisionMeshPtr pMesh = SHARED_PTR_CAST<CBulletCollisionMesh>(outpResource);
			return AsyncCreateCollisionMeshFromFile(ineStorageLocation, instrFilePath, pMesh);
        }
        //----------------------------------------------------------
        /// Create Collision Mesh From File
        //----------------------------------------------------------
        bool CBulletCollisionMeshLoader::CreateCollisionMeshFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath, BulletCollisionMeshPtr& outpResource)
        {
            return ReadFile(ineStorageLocation, instrFilePath, outpResource);
        }
        //----------------------------------------------------------
        /// Async Create Collision Mesh From File
        //----------------------------------------------------------
        bool CBulletCollisionMeshLoader::AsyncCreateCollisionMeshFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath, BulletCollisionMeshPtr& outpResource)
        {
            Task3<Core::STORAGE_LOCATION, const std::string&, BulletCollisionMeshPtr&> ReadFileTask(this, &CBulletCollisionMeshLoader::ReadFileTask, ineStorageLocation, instrFilePath, outpResource);
            CTaskScheduler::ScheduleTask(ReadFileTask);
            return true;
        }
        //----------------------------------------------------------
        /// Read File Task
        //----------------------------------------------------------
        void CBulletCollisionMeshLoader::ReadFileTask(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath, BulletCollisionMeshPtr& outpResource)
        {
            bool bSuccess = ReadFile(ineStorageLocation, instrFilePath, outpResource);
            if (bSuccess == true)
            {
                outpResource->SetLoaded(true);
            }
        }
        //----------------------------------------------------------
        /// Read File
        //----------------------------------------------------------
        bool CBulletCollisionMeshLoader::ReadFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string &instrFilePath, BulletCollisionMeshPtr& outpResource)
        {
            //open the file
            Core::FileStreamPtr pStream = Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, instrFilePath, Core::FM_READ_BINARY);
            if (pStream->IsOpen() == false || pStream->IsBad() == true)
            {
                ERROR_LOG("Could not open file '" + instrFilePath + "'");
                pStream->Close();
                return false;
            }
            
            //check the endianess check value.
            u32 udwEndianessCheckValue = ReadValue<u32>(pStream);
            if (udwEndianessCheckValue != 8888)
            {
                ERROR_LOG("File '" + instrFilePath + "' is not a valid mocollision file.");
            }
            
            //check the version
            u32 udwVersionNumber = ReadValue<u32>(pStream);
            if (udwVersionNumber != 2)
            {
                ERROR_LOG("The mocollision format version in '" + instrFilePath + "' is not supported.");
            }
            
            //get the data sizes
            u32 udwIndexSize = ReadValue<u32>(pStream);
            u32 udwVertexSize = ReadValue<u32>(pStream);
            
            //get the counts
            u32 udwIndexCount = ReadValue<u32>(pStream);
            u32 udwVertexCount = ReadValue<u32>(pStream);

            
            //check the sizes
            if (udwIndexSize == 0 || udwVertexSize == 0 || udwIndexCount == 0 || udwVertexCount == 0)
            {
                pStream->Close();
                return false;
            }
            
            //read index data
            u8* pIndexData = new u8[udwIndexSize * udwIndexCount];
            pStream->Read((s8*)pIndexData, udwIndexSize * udwIndexCount);
            
            //read vertex data
            u8* pVertexData = new u8[udwVertexSize * udwVertexCount];
            pStream->Read((s8*)pVertexData, udwVertexSize * udwVertexCount);
            
            //add this data to the resource
            outpResource->Prepare(pIndexData, udwIndexSize * udwIndexCount, pVertexData, udwVertexSize * udwVertexCount);
            
            //cleanup
            pStream->Close();
            
            return true;
        }
    }
}
