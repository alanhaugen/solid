#include "cafefilesystem.h"

CafeFileSystem::CafeFileSystem()
{
}

CafeFileSystem::~CafeFileSystem()
{
    Shutdown();
}

bool CafeFileSystem::Init()
{
    FSInit();

    client = (FSClient*) MEMAllocFromDefaultHeap(sizeof(FSClient));
    if (!client)
    {
        OSHalt("Error: cannot allocate memory for fs client.\n");
    }

    cmd = (FSCmdBlock*) MEMAllocFromDefaultHeap(sizeof(FSCmdBlock));

    if (!cmd)
    {
        OSHalt("Error: cannot allocate memory for command block.\n");
    }
     
    FSStatus status = FSAddClient(client, FS_RET_NO_ERROR);

    if (status == FS_STATUS_OK)
    {
        FSInitCmdBlock(cmd);

        return true;
    }

    return false;
}

bool CafeFileSystem::Shutdown()
{
    FSDelClient(client, FS_RET_NO_ERROR);
    FSShutdown();

    MEMFreeToDefaultHeap(client);
    MEMFreeToDefaultHeap(cmd);

    return true;
}

CafeFile *CafeFileSystem::Open(URL path, const unsigned int fileType)
{
    CafeFile *cafefile = new CafeFile(client, cmd, path, fileType);

    return cafefile;
}

