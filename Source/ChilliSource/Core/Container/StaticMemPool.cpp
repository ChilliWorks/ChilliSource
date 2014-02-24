#include <ChilliSource/Core/Container/StaticMemPool.h>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------------------------
        /// Constructor
        ///
        /// Allocate a fixed size pool from the heap
        ///
        /// @param Size of memory pool in bytes
        /// @param Alignment in bytes
        //----------------------------------------------------------------------
        CStaticMemPool::CStaticMemPool(u32 inudwHeapSize, u32 inudwAlignmentSize)
            : mudwAlignmentSize(inudwAlignmentSize), mudwFreeMemory(inudwHeapSize), mudwUsedMemory(0)
        {
            //Allocate the static memory size
            mpMemPool = (s8*)malloc(inudwHeapSize);

            if(mpMemPool)
            {
                //Zero the memory to aid debugging
                memset(mpMemPool, 0, inudwHeapSize * sizeof(s8));
            }
            else
            {
                ERROR_LOG("Not enough memory to allocate static mem pool of size " + STRING_CAST(inudwHeapSize));
            }

            //Let's allocate the chunks that manage the memory pool
            u32 udwNumChunksRequired = mudwFreeMemory/mudwAlignmentSize;
            mChunks.reserve(udwNumChunksRequired);

            s8* pBlock = mpMemPool;

            for(u32 i=0; i<udwNumChunksRequired; ++i, pBlock+=mudwAlignmentSize)
            {
                CStaticMemPool::MemChunk Chunk;
                //Assign this chunk a section of the pool to manage
                Chunk.pbyData = pBlock;

                mChunks.push_back(Chunk);
            }
        }
        //-----------------------------------------------------------------------
        /// Release
        ///
        /// Give the memory held by the object back to the memory pool
        ///
        /// @param Pointer to memory held by pool
        //-----------------------------------------------------------------------
        void CStaticMemPool::Release(void* inpData)
        {
            for(DYNAMIC_ARRAY<MemChunk>::iterator it = mChunks.begin(); it != mChunks.end(); ++it)
            {
                if(it->pbyData == inpData)
                {
                    mudwFreeMemory += it->udwDataSize;
                    mudwUsedMemory -= it->udwDataSize;

                    u32 udwNumChunks = (it->udwDataSize/mudwAlignmentSize + 1);

                    for(u32 i=0; i<udwNumChunks; ++i)
                    {
                        (it+i)->udwDataSize = 0;
                        (it+i)->bIsAllocated = false;
                    }
                    return;
                }
            }
        }
        //----------------------------------------------------------------------
        /// Destructor
        //----------------------------------------------------------------------
        CStaticMemPool::~CStaticMemPool()
        {
            free(mpMemPool);
        }
    }
}