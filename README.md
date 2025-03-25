# CustomGoC2
Custom command-and-control framework with a C implant and Go server.

## To-Do
- [ ] Encrypt communications (use AES or XOR for task/output bodies)
- [ ] Seperate callback paths from management paths (i.e. mgmt paths listen on 50050 and callbacks on 443)
- [ ] Implement a jitter function
- [ ] Make comms profile completely custom (similar to a malleable c2 profile for Cobalt Strike)
- [ ] Clear out old agents
- [ ] Save cmd output history in a file and in the web UI
- [ ] Cmd timestamps
- [ ] Save cmd output history in a file and in the web UI
- [ ] Use POST requests for everything?
- [ ] Live agent count and/or dashboard stats?
- [ ] Dark mode/theme support
- [ ] Auto refresh
- [ ] Web UI authentication
- [ ] General server logging
- [ ] Agent heartbeat alerting