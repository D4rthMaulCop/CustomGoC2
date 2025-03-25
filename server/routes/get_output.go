package routes

import (
	"encoding/json"
	"net/http"
	"strings"

	"customgoc2/data"
)

func HandleGetOutput(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	agentID := strings.TrimPrefix(r.URL.Path, "/get_output/")
	if agentID == "" {
		http.Error(w, "Agent ID missing", http.StatusBadRequest)
		return
	}

	output := data.Store.GetOutputs(agentID)
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(output)
}
