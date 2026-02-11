#include "ActivityPlanning.h"
#include "Room.h"
#include "RoomController.h"
#include "Movement.h"
#include "Position.h"
#include "AgentListController.h"

ActivityPlanning::ActivityPlanning(RoomController* rc, AgentListController* alc)
  : roomController(rc),
    agentList(alc),
    currentState(states::Moving) {}

void ActivityPlanning::setGoalToGuard(Goal goal) {
  goalToGuardAlarm = goal;
  goalGuardSet = true;
}

Goal ActivityPlanning::getGoalToGuard() {
  if (goalGuardSet) {
    return goalToGuardAlarm;
  } else {
    return Goal(0, 0, 0);
  }
}

void ActivityPlanning::setAlarmActivated(bool alarm) {
  alarmActivated = alarm;
}

bool ActivityPlanning::getAlarmActivated() {
  return alarmActivated;
}

Goal ActivityPlanning::getChargerGoalByIndex(int id) {
  std::vector<Agent> allEsps = agentList->getOtherAgents();
  Goal newGoal;

  int myIndex = -1;
  for (int i = 0; i < allEsps.size(); i++) {
    if (allEsps[i].getId() == id) {
      myIndex = i;
      break;
    }
  }

  if (myIndex == -1) {
    Serial.println("ERROR: My ESP ID not found");
  }

  if (roomController->chargeStations.empty())
    return newGoal;

  int chargerIndex = myIndex % roomController->chargeStations.size();
  ChargeStation& cs = roomController->chargeStations[chargerIndex];

  newGoal.setRoom(cs.getRoomNr());
  newGoal.setGoalX(cs.getXHigh());
  newGoal.setGoalY(cs.getYHigh());

  Serial.print("Assigned charger ID: ");
  Serial.println(cs.getId());

  return newGoal;
}


bool ActivityPlanning::getChargerOccupied(Position robotPosition) {

  for (int i = 0; i < agentList->getTotalAgents(); i++) {
    Agent* agent = agentList->getAgentByIndex(i);
    if (!agent) continue;
    if ((agent->getGroupId() == agentList->getAgentById(robotPosition.getId()).getGroupId()) && (agent->getGoalSetToCharger())) {
      //Serial.println("Its true");
      return true;
    }
  }

  return false;
}

bool ActivityPlanning::isGroupLowBattery(Position robotPosition) {
  bool deviceLowBattery = false;

  for (int i = 0; i < agentList->getTotalAgents(); i++) {
    Agent* agent = agentList->getAgentByIndex(i);
    if (!agent) continue;
    if ((agent->getGroupId() == agentList->getAgentById(robotPosition.getId()).getGroupId()) && (agent->getPercentage() < 20) && (agent->getId() != robotPosition.getId())) {
      deviceLowBattery = true;
    }
  }

  return deviceLowBattery;
}

int ActivityPlanning::decideArea(uint32_t id, int nbrOfEsps) {
  std::vector<Agent> allEsps = agentList->getOtherAgents();

  if (agentList->getTotalAgents() < nbrOfEsps) {
    Serial.println("Waiting for full agent list before decideArea()");
    return -1;
  }

  if (allEsps.empty()) {
    Serial.println("ERROR: No ESP agents available");
    return -1;
  }

  roomController->setupGroups(nbrOfEsps);

  for (int i = 0; i < allEsps.size() - 1; i++) {
    for (int j = 0; j < allEsps.size() - i - 1; j++) {
      if (allEsps[j].getId() > allEsps[j + 1].getId()) {
        Agent tmp = allEsps[j];
        allEsps[j] = allEsps[j + 1];
        allEsps[j + 1] = tmp;
      }
    }
  }

  int myIndex = -1;
  for (int i = 0; i < allEsps.size(); i++) {
    if (allEsps[i].getId() == id) {
      myIndex = i;
      break;
    }
  }

  if (myIndex == -1) {
    Serial.println("ERROR: My ESP ID not found");
    return -1;
  }

  int numberOfGroups = (nbrOfEsps < 4) ? nbrOfEsps : 4;

  int groupId = (myIndex % numberOfGroups) + 1;

  roomsToGuard.clear();

  std::vector<int> groupRooms = roomController->getRoomsInGroup(groupId);

  for (int i = 0; i < groupRooms.size(); i++) {
    roomsToGuard.push_back(roomController->getRoom(groupRooms[i]));
  }

  Serial.print("ESP: ");
  Serial.print(id);
  Serial.print(", assigned to group: ");
  Serial.println(groupId);

  return groupId;
}

bool ActivityPlanning::isValObjectStolen(int roomNr) {
  Room* room = roomController->getRoomPointer(roomNr);
  ValuableObject* vo = room->getValuableObject(currentValuableObjectID);

  // Serial.print("ID vo: ");
  // Serial.println(vo->getId());

  // Serial.print("ID currentValID");
  // Serial.println(currentValuableObjectID);

  if (vo->getId() == currentValuableObjectID) {
    // Serial.print("Is obj stolen: ");
    // Serial.println(vo->isStolen());
    return vo->isStolen();
  }
  return false;
}

Goal ActivityPlanning::getNextValObject(Room room) {
  Goal obj;
  Goal newGoal;

  obj.setZone(false);

  if (valuableObjectsInRoom.empty())
    valuableObjectsInRoom = room.getValuableObjects();

  if (!valuableObjectsInRoom.empty()) {
    ValuableObject obj = valuableObjectsInRoom.front();
    currentValuableObjectID = obj.getId();
    //Serial.println(currentValuableObjectID);

    newGoal.setRoom(obj.getRoomNr());
    newGoal.setGoalX(obj.getXHigh());
    newGoal.setGoalY(obj.getYHigh());
    newGoal.setZone(obj.getIsZone());
    newGoal.setIsComZone(obj.getIsComZone());
    // Serial.print("Room nr: ");
    // Serial.println(newGoal.getRoom());
    // Serial.print("X: ");
    // Serial.println(newGoal.getGoalX());
    // Serial.print("Y: ");
    // Serial.println(newGoal.getGoalY());
    // Serial.print("Is zone: ");
    // Serial.println(obj.getIsZone());
  }

  return newGoal;
}

int ActivityPlanning::checkAgentStatus() {
  auto agents = agentList->getOtherAgents();

  for (int i = agents.size() - 1; i >= 0; i--) {
    if (millis() - agents[i].getLastMsgTime() >= 40000) {
      agentList->removeAgentById(agents[i].getId());
      Serial.print("Deleted ESP: ");
      Serial.println(agents[i].getId());
    }
  }

  return agentList->getOtherAgents().size();
}

int ActivityPlanning::countNbrEsps() {
  int agents = agentList->getTotalAgents();

  return agents;
}

bool ActivityPlanning::checkAgentStatusNew() {

  auto agents = agentList->getOtherAgents();

  unsigned long currentMillis = millis();

  for (int i = agents.size() - 1; i >= 0; i--) {

    // auto timeSince = millis() - agents[i].getLastMsgTime();
    // Serial.print("ID: ");
    // Serial.print(agents[i].getId());
    // Serial.print(", time since: ");
    // Serial.println(timeSince);
    if (currentMillis - agents[i].getLastMsgTime() >= 30000) {
      return true;
    }
  }

  return false;
}

std::tuple<int, bool> ActivityPlanning::deleteInactiveEspsNew() {
  auto agents = agentList->getOtherAgents();
  unsigned long currentMillis = millis();

  for (int i = agents.size() - 1; i >= 0; i--) {
    // auto timeSince = millis() - agents[i].getLastMsgTime();
    // Serial.print("ID: ");
    // Serial.print(agents[i].getId());
    // Serial.print(", time since: ");
    // Serial.println(timeSince);

    if (currentMillis - agents[i].getLastMsgTime() >= 30000) {
      agentList->removeAgentById(agents[i].getId());
      Serial.print("Deleted ESP: ");
      Serial.println(agents[i].getId());
      return std::tuple(agentList->getTotalAgents(), true);
    }
  }

  return std::tuple(agentList->getTotalAgents(), true);
}


std::vector<Goal> ActivityPlanning::getPassageGoal(std::vector<Passage> passages) {
  Goal newGoal;
  std::vector<Goal> goalsToSend;

  for (int i = 0; i < passages.size(); i++) {
    newGoal.setRoom(passages[i].getRoom1());
    newGoal.setGoalX(passages[i].getPassageCoordHigh());
    newGoal.setGoalY(passages[i].getCrossCoord());

    goalsToSend.push_back(newGoal);
  }

  return goalsToSend;
}

Goal ActivityPlanning::getChargerGoal(uint32_t groupId) {
  Goal newGoal;

  std::vector<int> groupRooms = roomController->getRoomsInGroup(groupId);

  for (int i = 0; i < groupRooms.size(); i++) {
    int roomNbr = groupRooms[i];
    //Serial.println(roomNbr);

    for (int j = 0; j < roomController->chargeStations.size(); j++) {
      ChargeStation& cs = roomController->chargeStations[j];

      if (cs.getRoomNr() == roomNbr) {
        Serial.print("Found charger in room: ");
        Serial.println(roomNbr);
        newGoal.setRoom(cs.getRoomNr());
        newGoal.setGoalX(cs.getXHigh());
        newGoal.setGoalY(cs.getYHigh());
        // Serial.print("Room nr: ");
        // Serial.println(newGoal.getRoom());
        // Serial.print("X: ");
        // Serial.println(newGoal.getGoalX());
        // Serial.print("Y: ");
        // Serial.println(newGoal.getGoalY());

        chargeGoalSet = true;
        return newGoal;
      }
    }
  }

  return newGoal;
}

Room ActivityPlanning::getNextRoomForAgent(int agentId) {
  Room selectedRoom;
  int attempts = 0;
  const int MAX_ATTEMPTS = 5;

  while (attempts < MAX_ATTEMPTS) {
    selectedRoom = roomsToGuard.front();

    bool roomFree = true;

    for (int i = 0; i < agentList->getTotalAgents(); i++) {
      Agent otherAgent = agentList->otherAgents[i];

      if (otherAgent.getId() != agentId && otherAgent.getRoom() == selectedRoom.getRoomNr()) {
        roomFree = false;
        break;
      }
    }

    if (roomFree) {
      return selectedRoom;
    }

    std::rotate(roomsToGuard.begin(),
                roomsToGuard.begin() + 1,
                roomsToGuard.end());

    attempts++;
  }

  return selectedRoom;
}


void ActivityPlanning::RemoveObj() {
  valuableObjectsInRoom.erase(valuableObjectsInRoom.begin());
}

std::vector<Goal> ActivityPlanning::Alarming(Room currentRoom) {
  std::vector<Passage> passages = currentRoom.getPassages();
  goalsToSendAlarm = getPassageGoal(passages);
  return goalsToSendAlarm;
}

std::vector<Goal> ActivityPlanning::answeringGoalRequest(GoalMessage goal) {
  std::vector<Goal> goalsApproved;

  if (goal.targetX == goalsToSendAlarm.front().getGoalX() && goal.targetY == goalsToSendAlarm.front().getGoalY()) {
    goalsToSendAlarm.erase(goalsToSendAlarm.begin());
  } else {
    goalsApproved = goalsToSendAlarm;
  }

  return goalsApproved;
}
