#include <iostream>
#include <Windows.h>
#include <process.h>

#include "MhyDrvIO.h"

char drv_path[MAX_PATH] = { 0 };

void TestDrv(MhyDrvIO* pdrvCtl);

int main()
{
    char tempPath[MAX_PATH] = { 0 };
    char curDirPath[MAX_PATH] = { 0 };
    if (!GetTempPathA(MAX_PATH, tempPath))
    {
        printf("Can not get tmp path...\n");
        return 0;
    }
    if (!GetCurrentDirectoryA(MAX_PATH, curDirPath))
    {
        printf("Can not get cur path...\n");
        return 0;
    }
    printf("tmp path: %s\n", tempPath);
    printf("cur dir path: %s\n", curDirPath);
    char raw_drv_path[MAX_PATH] = { 0 };
    sprintf_s(drv_path, "%s\\%s", tempPath, "mhyprot2.Sys");
    sprintf_s(raw_drv_path, "%s\\%s", curDirPath, "mhyprot2.Sys");

    printf("drv path: %s\n", drv_path);
    printf("raw path: %s\n", raw_drv_path);
    CopyFileA(raw_drv_path, drv_path, true);
    MhyDrvIO* pdrvCtl = new MhyDrvIO(drv_path);
    TestDrv(pdrvCtl);
    delete pdrvCtl;
}

void TestDrv(MhyDrvIO* pdrvCtl)
{
    char buff1[8] = { 0 };
    char buff2[sizeof(buff1)] = { 0 };
    char buff3[sizeof(buff1)] = { 0 };
    memset(buff1, 0x22, sizeof(buff1));
    pdrvCtl->ReadProcMem(_getpid(), (DWORD64)buff1, sizeof(buff1), buff2);
    printf("Buff1 is : //( if all 0x22 is success.\n");
    for (int i = 0; i < sizeof(buff1); i++)
    {
        printf("%02x ", buff2[i]);
    }
    printf("\n\n");

    pdrvCtl->WriteProcMem(_getpid(), (DWORD64)buff3, sizeof(buff1), buff1);
    printf("Buff3 is : //( if all 0x22 is success.\n");
    for (int i = 0; i < sizeof(buff1); i++)
    {
        printf("%02x ", buff3[i]);
    }
    printf("\n\n");
}