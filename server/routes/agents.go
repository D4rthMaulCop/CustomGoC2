package routes

import (
	"encoding/json"
	"net/http"

	"customgoc2/data"
)

func HandleAgents(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	agents := make(map[string]data.AgentInfo)

	data.Store.RLock()
	defer data.Store.RUnlock()

	for id, agent := range data.Store.Agents {
		agents[id] = *agent
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(agents)
}
