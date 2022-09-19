#include <iostream>
#include <Windows.h>


int main()
{
    const char name[] = {"C:\\Users\\Omer Cohen\\OneDrive\\Documents\\Programming\\Sandbox\\POC-sandbox\\inlineHook\\Debug\\inlineHook.dll"};
    unsigned int len{ sizeof(name) + 1 };
    DWORD result = GetFullPathNameA(name,0,NULL,NULL);
    char* buf = new char[result];
    result = GetFullPathNameA(name, result, buf, NULL);
    
    PVOID addrLoadLibrary =(PVOID)GetProcAddress(GetModuleHandleA("kernel32"), "LoadLibraryA");

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    CreateProcessA(
        "C:\\Users\\Omer Cohen\\OneDrive\\Docuemnts\\Programming\\Sandbox\\POC-sandbox\\virus\\Debug\\virus.exe",
        NULL,
        NULL,
        NULL,
        FALSE,
        NORMAL_PRIORITY_CLASS,
        NULL,
        NULL,
        (LPSTARTUPINFOA)&si,
        &pi
    );
    std::cout << pi.hProcess << '\n';
    PVOID memAddr = (PVOID)VirtualAllocEx(
        pi.hProcess,
        NULL,
        result,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_EXECUTE_READWRITE
    );

    if (memAddr == NULL) {
        std::cout << "here";
        DWORD err = GetLastError();
        std::cout << err;
        return 0;
    }

    if (!WriteProcessMemory(
        pi.hProcess,
        memAddr,
        buf,
        result,
        NULL
    )) {
        DWORD err = GetLastError();
        std::cout << err;
        return 0;
    }

    HANDLE remote_thread = CreateRemoteThread(
        pi.hProcess,
        NULL,
        0,
        (LPTHREAD_START_ROUTINE )addrLoadLibrary,
        memAddr,
        0,
        NULL
    );
    WaitForSingleObject(remote_thread, INFINITE);
    CloseHandle(remote_thread);
    return 0;
}
