package routes

import (
	"encoding/json"
	"log"
	"net/http"
	"strings"

	"customgoc2/data"
)

func HandleHeartbeat(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	agentID := strings.TrimPrefix(r.URL.Path, "/heartbeat/")
	if agentID == "" {
		http.Error(w, "Agent ID missing", http.StatusBadRequest)
		return
	}

	var systemInfo map[string]string
	err := json.NewDecoder(r.Body).Decode(&systemInfo)
	if err != nil {
		// If body isn't JSON (like "ping"), default to nil system info
		systemInfo = nil
	}

	data.Store.UpdateAgent(agentID, systemInfo)
	log.Printf("[+] Agent %s checked in\n", agentID)

	w.WriteHeader(http.StatusOK)
	w.Write([]byte("OK"))
}
