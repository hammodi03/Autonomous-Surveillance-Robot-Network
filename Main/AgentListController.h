#ifndef AGENTLISTCONTROLLER_H
#define AGENTLISTCONTROLLER_H

#include <vector>
class Agent;

class AgentListController {
private:

public:
  AgentListController();
  std::vector<Agent> otherAgents;

  void addOrUpdateAgent(const Agent& agent);
  bool removeAgentById(int id);
  Agent getAgentById(int id);
  Agent* getAgentPointerById(int id);
  int getTotalAgents() const;
  Agent* getAgentByIndex(int index);
  std::vector<Agent> getOtherAgents();
  int getHighestId();

  void clear();
};

#endif
