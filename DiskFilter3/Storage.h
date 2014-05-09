#pragma once
#include "common.h"
#include "DiskFilterIoctl.h"

#ifdef USE_DRAM
  #define BLOCK_RESERVE                   (0)
#else
  #define BLOCK_RESERVE                   (16<<20)
  #ifdef BLOCK_STORAGE_WRITE_BUFF
    #define WRITE_BUFFER_SIZE             (2<<20)
  #endif
#endif

#ifndef USE_DRAM
  //TODO: Block Storage Buffer Not Work
  //#define BLOCK_STORAGE_WRITE_BUFF
  #ifdef BLOCK_STORAGE_WRITE_BUFF
    #define WRITE_BUFFER_SIZE             (2<<20)
  #endif
#endif

typedef struct _STORAGE_POOL
{
    ULONG           Size;
    ULONG           Used;
    LONGLONG        TotalSize;
    RTL_BITMAP      Bitmap;
    PULONG          Bitmap_Buffer;
#ifdef USE_DRAM
    PUCHAR          Buffer;
#else
    PDEVICE_OBJECT  BlockDevice;
#endif
    // Opaque
    ULONG           HintIndex;
#ifdef BLOCK_STORAGE_WRITE_BUFF
    PUCHAR          WBuffer;
    LARGE_INTEGER   WBufStartOffset;
    ULONG           WBufLength;
#endif
}STORAGE_POOL, *PSTORAGE_POOL;

BOOLEAN
InitStoragePool (PSTORAGE_POOL StoragePool, ULONG Size
                 #ifndef USE_DRAM
                     ,ULONG DiskNum
                     ,ULONG PartitionNum
                 #endif
                );

VOID
DestroyStoragePool(PSTORAGE_POOL StoragePool);

ULONG
StoragePoolAlloc(PSTORAGE_POOL StoragePool);

VOID
StoragePoolFree(PSTORAGE_POOL StoragePool, ULONG Index);

VOID
StoragePoolWrite(PSTORAGE_POOL StoragePool, ULONG StartIndex, ULONG Offset, PVOID Data, ULONG Len);

VOID
StoragePoolRead(PSTORAGE_POOL StoragePool, PVOID Data, ULONG StartIndex, ULONG Offset, ULONG Len);
