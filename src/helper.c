#include <windows.h>
#include <stdio.h>

#define FORMAT_FLAGS ( FORMAT_MESSAGE_ALLOCATE_BUFFER |\
                       FORMAT_MESSAGE_FROM_SYSTEM     |\
                       FORMAT_MESSAGE_IGNORE_INSERTS  )

void WINAPI Log(HRESULT Result, PWSTR Function)
{
    if (Result == 0)
        return;

    PWSTR MsgBuffer = NULL;

    FormatMessageW(FORMAT_FLAGS, NULL, Result, 0,
                   (PWSTR)&MsgBuffer, 0, NULL);
    wprintf(L"%ls Error: %ls\n", Function, MsgBuffer);
    LocalFree(MsgBuffer);
}

void WINAPI Usage(PCWSTR Program)
{
    wprintf(
        L"\n"
        L"hvtool -- Experiments with Hyper-V VM and container managing APIs.\n"
        L"Copyright (C) 2019 Biswapriyo Nath\n"
        L"This program comes with ABSOLUTELY NO WARRANTY.\n"
        L"Licensed under GNU General Public License version 3 or higher\n\n"
        L"Usage: %ls [options] [agruments]\n\n"
        L"Options:\n"
        L"  endp       Get list of network endpoints.\n"
        L"  help       Show this usage information.\n"
        L"  list       Get list of containers.\n"
        L"  net        Get list of networks.\n"
        L"  kill       Container ID\n"
        L"             Kill the specified container.\n"
        L"  pause      Container ID\n"
        L"             Pause the specified container.\n"
        L"  resume     Container ID\n"
        L"             Resume the specified container.\n"
        L"  shutdown   Container ID\n"
        L"             Shutdown the specified container (for VM only).\n"
        L"  wsl        Get Windows side IPv4 address connected with WSL2 VM.\n\n"
        L"The Container argument can be found in \"list\" option output.\n",
        Program);
}
