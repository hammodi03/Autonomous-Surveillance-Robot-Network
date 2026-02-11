#include "AgentListController.h"
#include "Agent.h"

AgentListController::AgentListController() {
}

void AgentListController::addOrUpdateAgent(const Agent& agent) {
  for (auto& a : otherAgents) {
    if (a.getId() == agent.getId()) {
      a = agent;
      return;
    }
  }

  otherAgents.push_back(agent);
}

bool AgentListController::removeAgentById(int id) {
  for (auto it = otherAgents.begin(); it != otherAgents.end(); ++it) {
    if (it->getId() == id) {
      otherAgents.erase(it);
      return true;
    }
  }
  return false;
}

Agent AgentListController::getAgentById(int id) {
  Agent agent;

  for (int i = 0; i < otherAgents.size(); i++) {
    if (id == otherAgents[i].getId()) {
      agent = otherAgents[i];
      return agent;
    }
  }

  return agent;
}

Agent* AgentListController::getAgentPointerById(int id) {
  for (int i = 0; i < otherAgents.size(); i++) {
    if (id == otherAgents[i].getId()) {
      return &otherAgents[i];
    }
  }

  return nullptr;
}

Agent* AgentListController::getAgentByIndex(int index) {
  return &otherAgents[index];
}


int AgentListController::getTotalAgents() const {
  return otherAgents.size();
}

void AgentListController::clear() {
  otherAgents.clear();
}

std::vector<Agent> AgentListController::getOtherAgents() {
  return otherAgents;
}

int AgentListController::getHighestId() {
  if (otherAgents.empty()) return -1;

  int maxId = otherAgents[0].getId();

  for (int i = 1; i < otherAgents.size(); i++) {
    if (otherAgents[i].getId() > maxId) {
      maxId = otherAgents[i].getId();
    }
  }
  return maxId;
}
