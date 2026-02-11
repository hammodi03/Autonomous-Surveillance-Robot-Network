#include "Communication.h"
#include "ActivityPlanning.h"
#include "AgentListController.h"
#include "Position.h"
#include "Person.h"
#include "Radar.h"

std::vector<Message> globalReceivedMessages;
Communication *Communication::instance = nullptr;
bool Communication::systemOn = false;

Communication::Communication(RoomController *rc, AgentListController *alc, ActivityPlanning *ap, EnergyManagement *em, Position *position, PersonList *pl, Navigation *nav, Radar *r)
  : roomController(rc),
    agentList(alc),
    personList(pl),
    activityPlanning(ap),
    energyManagement(em),
    navigation(nav),
    radar(r),
    robotPosition(position) {
  instance = this;
  latestMessage.startUpMessage = true;
}

bool Communication::connectToServerAndSend(
  const char *host,
  uint16_t port,
  int id,
  float x,
  float y,
  float goalX,
  float goalY,
  float energyPercentage,
  ActivityPlanning::states currentState,
  bool alarmActivated) {
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("Kunde inte ansluta till servern.");
    return false;
  }

  std::string state = stateToString(currentState);

  char buffer[128];
  snprintf(buffer, sizeof(buffer),
           "%d,%.2f,%.2f,%.2f,%.2f,%.1f,%s,%s\n",
           id, x, y, goalX, goalY, energyPercentage, state.c_str(), alarmActivated ? "true" : "false");

  client.print(buffer);
  //Serial.println(buffer);

  unsigned long start = millis();
  while (!client.available() && millis() - start < 2000) {
    delay(10);
  }

  if (client.available()) {
    String response = client.readStringUntil('\n');
    response.trim();

    int p = response.indexOf(':');
    String cmd = (p > 0) ? response.substring(0, p) : response;

    // ------------ VALUABLE OBJECT ------------
    if (cmd == "VO_STOLEN" || cmd == "VO_RETURNED") {
      String type;
      int voIndex, objectId, roomId;

      Serial.println("VO stolen or returned");

      if (parseVOMessage(response, type, voIndex, objectId, roomId)) {
        Room *room = roomController->getRoomPointer(roomId);

        ValuableObject *vo = room->getValuableObject(objectId);
        if (vo->getId() == objectId) {
          Serial.print("ID");
          Serial.println(vo->getId());
          Serial.print("Object is stolen: ");
          vo->setStolen(type == "VO_STOLEN");
          Serial.println(vo->isStolen());
        }
      }
    }

    // ------------ BATTERY ------------
    else if (cmd == "BATTERY_SET") {
      int p1 = response.indexOf(':');
      int p2 = response.indexOf(':', p1 + 1);

      int targetId = response.substring(p1 + 1, p2).toInt();
      float newBatt = response.substring(p2 + 1).toFloat();

      if (targetId == id) {
        energyManagement->setPercentage(newBatt);
        Serial.println("Battery updated from server");
      }
    }

    // ------------ ACTIVATE OBSTACLE DETECTORS ------------
    else if (cmd == "OBSTACLE_DETECTORS") {
      int p1 = response.indexOf(':');
      int p2 = response.indexOf(':', p1 + 1);

      int targetId = response.substring(p1 + 1, p2).toInt();
      int enabledInt = response.substring(p2 + 1).toInt();
      bool enabled = (enabledInt == 1);

      if (targetId == id) {
        navigation->setRealDetection(enabled);
        Serial.printf("Obstacle detectors %s by server (device id=%d)\n", enabled ? "ENABLED" : "DISABLED", id);
      }
    }

    // ------------ ACTIVATE RADAR ------------
    else if (cmd == "RADAR") {
      int p1 = response.indexOf(':');
      int p2 = response.indexOf(':', p1 + 1);

      int targetId = response.substring(p1 + 1, p2).toInt();
      int enabledInt = response.substring(p2 + 1).toInt();
      bool enabled = (enabledInt == 1);

      if (targetId == id) {
        Serial.printf("Radar %s by server (device id=%d)\n", enabled ? "ENABLED" : "DISABLED", id);
        radar->setSimulatedStatus(!enabled);
        if (enabled) {
          radar->initRadar(true, 3);
        }
      }
    }

    // ------------ KILL ------------
    else if (cmd == "KILL_DEVICE") {
      int p1 = response.indexOf(':');
      int targetId = response.substring(p1 + 1).toInt();

      if (targetId == id) {
        activityPlanning->currentState = ActivityPlanning::states::Killed;
        Serial.println("Device killed by server");
      }
    }

    // ------------ REVIVE ------------
    else if (cmd == "REVIVE_DEVICE") {
      int p1 = response.indexOf(':');
      int targetId = response.substring(p1 + 1).toInt();

      if (targetId == id) {
        activityPlanning->currentState = ActivityPlanning::states::Planning;
        Serial.println("Device revived by server");
      }
    }

    // ------------ ADD PERSON ------------
    else if (cmd == "ADD_PERSON") {
      //formatet är ADD_PERSON:device_id:pX:pY:pRoom:pID

      // 1. Hitta positionerna för alla kolon
      int colon1 = response.indexOf(':');              // Efter ADD_PERSON
      int colon2 = response.indexOf(':', colon1 + 1);  // Efter device_id
      int colon3 = response.indexOf(':', colon2 + 1);  // Efter pX
      int colon4 = response.indexOf(':', colon3 + 1);  // Efter pY
      int colon5 = response.indexOf(':', colon4 + 1);  // Efter pRoom

      // 2. Extrahera variablerna (vi hoppar över device_id)
      double pX = response.substring(colon2 + 1, colon3).toFloat();
      double pY = response.substring(colon3 + 1, colon4).toFloat();
      int pRoom = response.substring(colon4 + 1, colon5).toInt();
      int pID = response.substring(colon5 + 1).toInt();

      // 3. Använd värdena
      Serial.print("Person Added - ID: ");
      Serial.print(pID);
      Serial.print(" Pos: ");
      Serial.print(pX);
      Serial.print(",");
      Serial.print(pY);
      Serial.print(" Room: ");
      Serial.println(pRoom);
      personList->addPerson(pID, pX, pY, pRoom);
    }

    // ------------ REMOVE PERSON ------------
    else if (cmd == "REMOVE_PERSON") {
      int colon1 = response.indexOf(':');
      int pID = response.substring(colon1 + 1).toInt();
      personList->removePersonById(pID);
      Serial.print("Person removed: ");
      Serial.println(pID);

    }


    // ------------ TELEPORT ------------
    else if (cmd == "TELEPORT") {
      Serial.println(response);
      int p1 = response.indexOf(':');
      int p2 = response.indexOf(':', p1 + 1);
      int p3 = response.indexOf(':', p2 + 1);
      int p4 = response.indexOf(':', p3 + 1);

      if (p1 < 0 || p2 < 0 || p3 < 0 || p4 < 0) {
        Serial.println("Fel teleport format");
      } else {
        int targetId = response.substring(p1 + 1, p2).toInt();
        float tx = response.substring(p2 + 1, p3).toFloat();
        float ty = response.substring(p3 + 1, p4).toFloat();
        int room = response.substring(p4 + 1).toInt();

        if (targetId == id) {
          if (!roomController->isPointInsideRoom(room, tx, ty)) {
            Serial.printf("Teleport misslyckades: (%.2f, %.2f) ligger inte i room %d\n", tx, ty, room);
          } else {
            robotPosition->updateXPosition(tx);
            robotPosition->updateYPosition(ty);
            robotPosition->updateRoom(room);
            Serial.printf("Teleport OK: %.2f, %.2f (room=%d)\n", tx, ty, room);
          }
        } else {
          //Serial.println("Okänt kommando från server");
        }
      }
    }
  }

  client.stop();
  return true;
}

bool Communication::parseVOMessage(const String &msg,
                                   String &type,
                                   int &voIndex,
                                   int &objectId,
                                   int &roomId) {
  int p1 = msg.indexOf(':');
  int p2 = msg.indexOf(':', p1 + 1);
  int p3 = msg.indexOf(':', p2 + 1);

  if (p1 < 0 || p2 < 0 || p3 < 0) return false;

  type = msg.substring(0, p1);
  voIndex = msg.substring(p1 + 1, p2).toInt();
  objectId = msg.substring(p2 + 1, p3).toInt();
  roomId = msg.substring(p3 + 1).toInt();

  return true;
}


//FLYTTA DENNA METOD TILL ANNAN KLASS OM DEN FUNGERAR.
std::string Communication::stateToString(ActivityPlanning::states s) {
  switch (s) {
    case ActivityPlanning::states::Moving: return "Moving";
    case ActivityPlanning::states::Scanning: return "Scanning";
    case ActivityPlanning::states::Planning: return "Planning";
    case ActivityPlanning::states::Charging: return "Charging";
    case ActivityPlanning::states::Killed: return "Killed";
    case ActivityPlanning::states::Waiting: return "Waiting & scanning";
    // Lägg till alla möjliga states här
    default: return "UNKNOWN";
  }
}


void Communication::setupCommunication() {
  WiFi.mode(WIFI_STA);
  WiFi.begin("ComHem<F580A8>", "4673a335");
  // WiFi.begin("FJRL_NET", "Tyre1313");
  //WiFi.begin("TN-RR8102", "abusneineh");
  // WiFi.begin("iPhone","ui2rwa8n3mfub");

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("\nWiFi anslutet!");

  if (esp_now_init() != ESP_OK) {
    Serial.println("Fel: kunde inte initiera ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(Communication::onDataRecv);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, BROADCAST_ADDR, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Kunde inte lägga till broadcast peer!");
  } else {
    Serial.println("Broadcast peer registrerad!");
  }

  Serial.println("ESP-NOW aktivt tillsammans med WiFi.");
}



bool Communication::sendMessage(const uint8_t *receiverMac, const Message &msg) {
  esp_err_t result = esp_now_send(receiverMac, (uint8_t *)&msg, sizeof(msg));
  if (result == ESP_OK) {
    return true;
  } else {
    Serial.printf("Skickfel: %d\n", result);
    return false;
  }
}

bool Communication::startUpLate() {
  unsigned long startTime = millis();
  while (millis() - startTime < 3000) {}

  if (latestMessage.startUpMessage) {
    return false;
  } else {
    return true;
  }
}


std::tuple<int, float, float, int> Communication::startUpSeq(bool systemOn) {
  randomSeed(esp_random());
  float x = 4.0 + (esp_random() / (float)UINT32_MAX) * (48.0 - 4.0);
  float y = 15.0 + (esp_random() / (float)UINT32_MAX) * (18.0 - 15.0);

  Message myMsg = {};
  myMsg.senderId = 0;
  myMsg.x = x;
  myMsg.y = y;
  myMsg.currentRoom = 0;
  //myMsg.currentState = 0;
  myMsg.startUpMessage = true;
  snprintf(myMsg.msgContent, sizeof(myMsg.msgContent), "Startup coordinate");

  //connectToServerAndSend("192.168.1.179", 5000, 1, x, y);
  //Serial.println("INNAN !SYSTEMON");
  if (!systemOn) {
    //Serial.println("EFTER !SYSTEMON1");
    globalReceivedMessages.push_back(myMsg);

    unsigned long startTime = millis();
    unsigned long lastSend = 0;

    int myId;

    while (millis() - startTime < 15000) {
      if (millis() - lastSend >= 1000) {
        sendMessage(BROADCAST_ADDR, myMsg);
        lastSend = millis();
        Serial.printf("Broadcast: (%.2f, %.2f)\n", x, y);
      }
      delay(50);
    }

    std::sort(globalReceivedMessages.begin(), globalReceivedMessages.end(), [](Message &a, Message &b) {
      if (a.x != b.x) return a.x < b.x;
      return a.y < b.y;
    });

    myId = 1;
    for (size_t i = 0; i < globalReceivedMessages.size(); i++) {
      Serial.print("X: ");
      Serial.println(globalReceivedMessages[i].x);
      Serial.print("Y: ");
      Serial.println(globalReceivedMessages[i].y);
      if (globalReceivedMessages[i].x == x && globalReceivedMessages[i].y == y) {
        myId = i + 1;
        break;
      }
    }

    myMsg.senderId = myId;
    Serial.printf("Tilldelat ID: %lu\n", myId);

    return std::tuple(globalReceivedMessages.size(), x, y, myId);
  } else {
    //Serial.println("EFTER !SYSTEMON2");
    return std::tuple(latestMessage.nextId, x, y, latestMessage.nextId);
  }
}

void Communication::onDataRecv(const esp_now_recv_info *recv_info, const uint8_t *incomingData, int len) {
  if (len != sizeof(Message)) return;

  Message msg;
  memcpy(&msg, incomingData, sizeof(Message));

  Message myMsg = {};

  if (msg.startUpMessage) {
    bool exists = false;
    for (const auto &m : globalReceivedMessages) {
      const uint8_t *mac = recv_info->src_addr;
      // Jämför MAC och koordinater
      if (memcmp(mac, m.macAddr, 6) == 0 && m.x == msg.x && m.y == msg.y) {
        exists = true;
        break;
      }
    }
    if (exists) return;

    // Lägg till MAC-adress i msg för kontroll (lägg till i Message struct)
    memcpy(msg.macAddr, recv_info->src_addr, 6);

    globalReceivedMessages.push_back(msg);

    Serial.println("\nMottaget ESP-NOW-meddelande:");
    Serial.printf("Från MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  recv_info->src_addr[0], recv_info->src_addr[1], recv_info->src_addr[2],
                  recv_info->src_addr[3], recv_info->src_addr[4], recv_info->src_addr[5]);
    Serial.printf("Sender ID: %lu\n", msg.senderId);
    Serial.printf("Position: (%.2f, %.2f)\n", msg.x, msg.y);
    Serial.printf("Room: %u\n", msg.currentRoom);
    Serial.printf("Meddelande: %s\n", msg.msgContent);

  } else if ((strcmp(msg.msgContent, "AlarmOrders") == 0) && Communication::systemOn) {
    Agent senderAgent = instance->agentList->getAgentById(msg.senderId);
    int senderGroupId = senderAgent.getGroupId();
    int myId = Communication::instance->robotPosition->getId();
    Agent me = instance->agentList->getAgentById(myId);
    int myGroupId = me.getGroupId();

    if ((msg.senderId != myId) && (myGroupId == senderGroupId)) {
      myMsg.senderId = myId;
      myMsg.startUpMessage = false;
      myMsg.goal = msg.goals[0];
      myMsg.receiverId = msg.senderId;
      snprintf(myMsg.msgContent, sizeof(myMsg.msgContent), "Ansgoal");
      instance->activityPlanning->setAlarmActivated(true);

      instance->sendMessage(BROADCAST_ADDR, myMsg);
    }
  } else if ((strcmp(msg.msgContent, "Ansgoal") == 0) && Communication::systemOn) {
    // Om första i goalsToSend är lika med msg.goal, är det OK, annars nej.
    if (msg.receiverId == instance->robotPosition->getId()) {
      std::vector<Goal> goalsApproved = instance->activityPlanning->answeringGoalRequest(msg.goal);
      myMsg.receiverId = msg.senderId;

      // Om empty godkänns förfrågan om goal
      if (goalsApproved.empty()) {
        myMsg.senderId = instance->robotPosition->getId();
        myMsg.startUpMessage = false;
        myMsg.goal = msg.goal;
        snprintf(myMsg.msgContent, sizeof(myMsg.msgContent), "GoalRequestPositive");
      } else {

        for (int i = 0; i < 4; i++) {
          myMsg.goals[i].targetX = goalsApproved[i].getGoalX();
          myMsg.goals[i].targetY = goalsApproved[i].getGoalY();
          myMsg.goals[i].targetRoom = goalsApproved[i].getRoom();
        }

        myMsg.senderId = instance->robotPosition->getId();
        myMsg.startUpMessage = false;
        snprintf(myMsg.msgContent, sizeof(myMsg.msgContent), "AlarmOrders");
      }

      instance->sendMessage(BROADCAST_ADDR, myMsg);
    }

  } else if ((strcmp(msg.msgContent, "GoalRequestPositive") == 0) && Communication::systemOn) {
    if (msg.receiverId == instance->robotPosition->getId()) {
      Goal goal(msg.goal.targetX, msg.goal.targetY, msg.goal.targetRoom, true);
      instance->activityPlanning->setAlarmActivated(true);
      instance->activityPlanning->setGoalToGuard(goal);
    }

  } else if (strcmp(msg.msgContent, "UpdateMessage") == 0 && Communication::systemOn) {
    Agent senderAgent = instance->agentList->getAgentById(msg.senderId);
    int senderGroupId = senderAgent.getGroupId();
    int myId = Communication::instance->robotPosition->getId();
    Agent me = instance->agentList->getAgentById(myId);
    int myGroupId = me.getGroupId();
    // Serial.println("Got update message1");
    // Serial.print("MY GROUP ID: ");
    // Serial.println(myGroupId);
    // Serial.print("SENDER GROUP ID: ");
    // Serial.println(senderGroupId);

    //Serial.println("Got update message2");
    Agent newAgent;
    newAgent.setId(msg.senderId);
    newAgent.setNextId(msg.nextId);
    newAgent.setX(msg.x);
    newAgent.setY(msg.y);
    newAgent.setRoom(msg.currentRoom);
    newAgent.setNextId(msg.nextId);
    newAgent.setLastMsgTime(millis());

    instance->agentList->addOrUpdateAgent(newAgent);

    if ((msg.senderId != myId) && (myGroupId == senderGroupId)) {
      // Serial.println("Got update message2");
      // Agent newAgent;
      // newAgent.setId(msg.senderId);
      // newAgent.setX(msg.x);
      // newAgent.setY(msg.y);
      // newAgent.setRoom(msg.currentRoom);
      // newAgent.setNextId(msg.nextId);

      // instance->agentList->addOrUpdateAgent(newAgent);
    }
  } else {

    if (msg.nextId > Communication::instance->latestMessage.nextId)
      Communication::instance->latestMessage = msg;

    Serial.print("Next ID: ");
    Serial.println(msg.nextId);
    Agent newAgent;
    newAgent.setId(msg.senderId);
    newAgent.setX(msg.x);
    newAgent.setY(msg.y);
    newAgent.setRoom(msg.currentRoom);
    newAgent.setNextId(msg.nextId);
    newAgent.setGoalSetToCharger(msg.chargerGoal);
    newAgent.setPercentage(msg.percentage);
    // Serial.println("TOG EMOT: ");
    // Serial.printf("Position: (%.2f, %.2f)\n", msg.x, msg.y);
    // Serial.printf("Room: %u", msg.currentRoom);
    // Serial.println("");
    //newAgent.setState(static_cast<ActivityPlanning::states>(msg.currentState));

    instance->agentList->addOrUpdateAgent(newAgent);

    // Serial.println("Agent tillagd/uppdaterad i AgentListController.");

    // Serial.println("==============================");
  }
}

void Communication::onDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Sändstatus: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Lyckades." : "Misslyckades.");
}
