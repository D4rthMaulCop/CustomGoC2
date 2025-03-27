#ifndef COMMANDS_H
#define COMMANDS_H

// cleanly exit implant process
void ExitImplantProcess();

// execute shell command and capture output
void ExecuteWindowsShellCommand(const char *cmd);

#endif