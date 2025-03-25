package data

import (
	"sync"
	"time"
)

type AgentInfo struct {
	AgentID    string            `json:"agent_id"`
	SystemInfo map[string]string `json:"system_info"`
	LastSeen   time.Time         `json:"last_seen"`
}

type AgentStore struct {
	sync.RWMutex
	Agents  map[string]*AgentInfo
	Tasks   map[string][]string
	Outputs map[string][]string
}

var Store = &AgentStore{
	Agents:  make(map[string]*AgentInfo),
	Tasks:   make(map[string][]string),
	Outputs: make(map[string][]string),
}

func (s *AgentStore) UpdateAgent(id string, info map[string]string) {
	s.Lock()
	defer s.Unlock()

	agent, exists := s.Agents[id]
	if !exists {
		agent = &AgentInfo{
			AgentID:    id,
			SystemInfo: info,
			LastSeen:   time.Now().UTC(),
		}
		s.Agents[id] = agent
	} else {
		agent.LastSeen = time.Now().UTC()
		if agent.SystemInfo == nil && info != nil {
			agent.SystemInfo = info
		}
	}
}

func (s *AgentStore) QueueTask(id, task string) {
	s.Lock()
	defer s.Unlock()
	s.Tasks[id] = append(s.Tasks[id], task)
}

func (s *AgentStore) GetNextTask(id string) (string, bool) {
	s.Lock()
	defer s.Unlock()
	tasks := s.Tasks[id]
	if len(tasks) == 0 {
		return "", false
	}
	task := tasks[0]
	s.Tasks[id] = tasks[1:]
	return task, true
}

func (s *AgentStore) AddOutput(id, output string) {
	s.Lock()
	defer s.Unlock()
	s.Outputs[id] = append(s.Outputs[id], output)
}

func (s *AgentStore) GetOutputs(id string) []string {
	s.RLock()
	defer s.RUnlock()
	return s.Outputs[id]
}
