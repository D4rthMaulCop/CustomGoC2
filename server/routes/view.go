package routes

import (
	"html/template"
	"net/http"
	"strings"

	"customgoc2/data"
)

var templates = template.Must(template.ParseGlob("templates/*.html"))

func HandleDashboard(w http.ResponseWriter, r *http.Request) {
	data.Store.RLock()
	agents := make(map[string]*data.AgentInfo)
	for id, agent := range data.Store.Agents {
		agents[id] = agent
	}
	data.Store.RUnlock()

	err := templates.ExecuteTemplate(w, "dashboard.html", agents)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

func HandleAgentView(w http.ResponseWriter, r *http.Request) {
	agentID := strings.TrimPrefix(r.URL.Path, "/agent/")
	if agentID == "" {
		http.Error(w, "Missing agent ID", http.StatusBadRequest)
		return
	}

	agent := data.Store.Agents[agentID]
	if agent == nil {
		http.Error(w, "Agent not found", http.StatusNotFound)
		return
	}

	err := templates.ExecuteTemplate(w, "agent_detail.html", agentID)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}
