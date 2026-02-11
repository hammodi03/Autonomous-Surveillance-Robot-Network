#ifndef ACTIVITYPLANNING_H
#define ACTIVITYPLANNING_H

#include <Arduino.h>
#include "Goal.h"
#include <vector>
#include "RoomController.h"
#include "AgentListController.h"
#include "Agent.h"

class Navigation;
class Movement;
class Position;
class RoomController;
class AgentListController;

class ActivityPlanning {
private:
  int currentValuableObjectID = -1;

public:
  enum class states {
    Moving,
    Scanning,
    Planning,
    Charging,
    Alarming,
    Killed,
    Waiting
  };

  ActivityPlanning(RoomController *rc, AgentListController *alc);
  std::vector<Goal> Alarming(Room currentRoom);
  std::vector<Goal> getPassageGoal(std::vector<Passage> passages);
  std::vector<Goal> answeringGoalRequest(GoalMessage goal);
  int decideArea(uint32_t id, int nbrOfEsps);
  Room getNextRoomForAgent(int agentId);
  Goal getNextValObject(Room room);
  Goal getChargerGoal(uint32_t id);
  void RemoveObj();
  void setGoalToGuard(Goal goal);
  Goal getGoalToGuard();
  bool isValObjectStolen(int roomNr);
  void setAlarmActivated(bool alarm);
  bool getAlarmActivated();
  bool getChargerOccupied(Position robotPosition);
  bool isGroupLowBattery(Position robotPosition);
  int checkAgentStatus();
  bool checkAgentStatusNew();
  std::tuple<int, bool> deleteInactiveEspsNew();
  int countNbrEsps();
  Goal getChargerGoalByIndex(int id);

  bool chargeGoalSet = false;
  volatile states currentState;
  RoomController *roomController;
  AgentListController *agentList;
  Navigation *navigation;
  std::vector<ValuableObject> valuableObjectsInRoom;
  std::vector<Goal> goalsToSendAlarm;
  Goal goalToGuardAlarm;
  bool alarmActivated = false;
  bool goalGuardSet = false;
  std::vector<Room> roomsToGuard;
};


#endif