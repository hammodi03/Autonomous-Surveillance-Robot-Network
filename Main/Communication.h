#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include "ActivityPlanning.h"
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <esp_err.h>
#include <vector>
#include <algorithm>
#include <tuple>
#include "AgentListController.h"
#include "EnergyManagement.h"
#include "Position.h"
#include "PersonList.h"
#include "Navigation.h"
#include "Radar.h"

class RoomController;
class AgentListController;
class PersonList;
class Radar;

const uint8_t BROADCAST_ADDR[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };


struct Message {
  uint32_t senderId;
  uint32_t receiverId;
  float x;
  float y;
  uint8_t currentRoom;
  int groupId;
  ActivityPlanning::states currentState;
  char msgContent[32];
  uint8_t macAddr[6];
  bool startUpMessage;
  GoalMessage goals[4];
  GoalMessage goal;
  int nextId = 0;
  bool chargerGoal;
  double percentage;
};


class Communication {
private:
  static Communication *instance;
  bool startupDone = false;
  static void onDataRecv(const esp_now_recv_info *recv_info, const uint8_t *incomingData, int len);
  static void onDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status);
  bool parseVOMessage(const String &msg, String &type, int &voIndex, int &objectId, int &roomId);
  std::string stateToString(ActivityPlanning::states s);

public:
  Message onDataRecv_fake(const Message &msg);
  Communication(RoomController *rc, AgentListController *alc, ActivityPlanning *ap, EnergyManagement *em, Position *position, PersonList *pl, Navigation *nav, Radar *r);
  void setupCommunication();
  bool startUpLate();
  std::tuple<int, float, float, int> startUpSeq(bool systemOn);
  bool connectToServerAndSend(const char *host, uint16_t port, int id, float x, float y, float goalX, float goalY, float energyPercentage, ActivityPlanning::states currentState, bool alarmActivated);
  bool sendMessage(const uint8_t *receiverMac, const Message &msg);

  RoomController *roomController;
  AgentListController *agentList;
  ActivityPlanning *activityPlanning;
  EnergyManagement *energyManagement;
  Position *robotPosition;
  Message latestMessage;
  static bool systemOn;
  PersonList *personList;
  Navigation *navigation;
  Radar *radar;
};

#endif
