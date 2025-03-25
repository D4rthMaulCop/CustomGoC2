package routes

import (
	"net/http"
	"strings"

	"customgoc2/data"
)

func HandleCheckin(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	agentID := strings.TrimPrefix(r.URL.Path, "/checkin/")
	if agentID == "" {
		http.Error(w, "Agent ID missing", http.StatusBadRequest)
		return
	}

	task, ok := data.Store.GetNextTask(agentID)
	if !ok {
		w.WriteHeader(http.StatusNoContent) // 204
		return
	}

	w.WriteHeader(http.StatusOK)
	w.Write([]byte(task))
}
