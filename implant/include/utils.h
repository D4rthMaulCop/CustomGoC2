#ifndef UTILS_H
#define UTILS_H

// Function declarations
void GenerateUUID(char *uuidStr, size_t maxLen);

// Get system info JSON
void GetSystemInfoForServer(char *jsonOut, size_t len);

// Execute command and send output to server
void ExecuteCommand(const char *cmd);

#endif
