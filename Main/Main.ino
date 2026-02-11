#include "Communication.h"
#include "ActivityPlanning.h"
#include "Navigation.h"
#include "Movement.h"
#include "EnergyManagement.h"
#include "IRSensor.h"
#include "RoomController.h"
#include "Position.h"
#include "Goal.h"
#include "Radar.h"
#include "DetectionController.h"
#include "Room.h"
#include "Agent.h"
#include "AgentListController.h"
#include "PersonList.h"
#include "Person.h"

RoomController roomController;
AgentListController agentList;
PersonList personList;
EnergyManagement energyManagement;
ActivityPlanning activityPlanning(&roomController, &agentList);
Position robotPosition(5, 47.5, 2);
DetectionController detectionController(4, 0, 2, 26, 27);
Radar radar(Serial1);
Navigation navigation(&roomController, &robotPosition, &detectionController);
Communication communication(&roomController, &agentList, &activityPlanning, &energyManagement, &robotPosition, &personList, &navigation, &radar);
Movement movement;
double inputX = 0.0;
double inputY = 0.0;
TaskHandle_t sendTaskHandle = NULL;
TaskHandle_t broadcastTaskHandle = NULL;
Goal newGoal;
Room currentRoom;
int idForEsp;
int newEspCount;
int groupId;
int totalEsps;
int percentage;

unsigned long startTime;
bool timerStarted = false;
bool systemOn = false;
bool chargerOccupied = false;
bool inGroupLowBattery = false;
bool initialStart = true;
bool espCountChanged = false;
Agent agent;

void setup() {
  Serial.begin(9600);
  delay(1000);
  roomController.initRooms();
  communication.setupCommunication();
  //Serial.println("Test1");
  detectionController.initDetection();
  //Serial.println("Test2");
  if (radar.getSimulatedStatus() == false) {
    radar.initRadar(true, 3);
  }

  systemOn = communication.startUpLate();
  auto startupCoords = communication.startUpSeq(systemOn);
  auto [nbrOfEsps, x, y, id] = startupCoords;

  // Serial.print("Number of ESP: ");
  // Serial.println(nbrOfEsps);

  idForEsp = id;

  //FOR TESTING-----------------------
  //id = 2;
  //----------------------------------

  robotPosition.changeId(id);
  robotPosition.updateRoom(11);
  robotPosition.updateXPosition(x);
  robotPosition.updateYPosition(y);
  activityPlanning.currentState = ActivityPlanning::states::Planning;

  agent.setId(id);
  agent.setX(x);
  agent.setY(y);
  agent.setRoom(11);
  //agent.setGroupId(groupId);
  agent.setRoom(11);
  agent.setNextId(totalEsps + 1);
  agent.setLastMsgTime(millis());

  agentList.addOrUpdateAgent(agent);

  xTaskCreate(
    sendMessageTask,  // Task-funktion
    "SendTask",       // Namn
    8192,             // Stack size
    NULL,             // Parameter
    1,                // Prioritet (1 = låg)
    &sendTaskHandle   // Handle
  );

  xTaskCreate(
    sendBroadcastTask,    // Task-funktion
    "SendTask",           // Namn
    12288,                // Stack size
    NULL,                 // Parameter
    1,                    // Prioritet (1 = låg)
    &broadcastTaskHandle  // Handle
  );

  communication.systemOn = true;
  delay(2000);

  if (!systemOn)
    totalEsps = nbrOfEsps;
  else
    totalEsps = activityPlanning.countNbrEsps();
  newEspCount = totalEsps;
}

void sendMessageTask(void* parameter) {

  const char* ip = "192.168.0.11";
  // const char *ip = "10.161.240.116";
  //const char* ip = "192.168.1.179";
  // const char *ip = "10.196.80.116";
  // const char *ip = "10.161.240.116";
  // const char *ip = "172.20.10.9"; //BEHÖVER ÄNDRA IP TILL SERVERNS ADRESS. ANVÄND "IPCONFIG" I TERMINAL DÄR SERVERN KÖRS OCH ANGE IPV4 ADRESS HÄR!
  const uint16_t port = 5000;

  for (;;) {
    if (activityPlanning.currentState == ActivityPlanning::states::Killed) {
      energyManagement.turnOffDraining();
      // Heartbeat 1 Hz så servern inte sätter Disconnected
      communication.connectToServerAndSend(ip, port,
                                           robotPosition.getId(),
                                           robotPosition.getXPosition(),
                                           robotPosition.getYPosition(),
                                           newGoal.getGoalX(),
                                           newGoal.getGoalY(),
                                           energyManagement.getPercentage(),
                                           activityPlanning.currentState,
                                           activityPlanning.getAlarmActivated());

      vTaskDelay(1000 / portTICK_PERIOD_MS);
      continue;
    }
    if (activityPlanning.currentState != ActivityPlanning::states::Charging) {
      energyManagement.addToCommunicationDrain();
    }

    communication.connectToServerAndSend(ip, port, robotPosition.getId(),
                                         robotPosition.getXPosition(),
                                         robotPosition.getYPosition(),
                                         newGoal.getGoalX(),
                                         newGoal.getGoalY(),
                                         energyManagement.getPercentage(),
                                         activityPlanning.currentState,
                                         activityPlanning.getAlarmActivated());

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void sendBroadcastTask(void* parameter) {
  for (;;) {
    if (activityPlanning.currentState != ActivityPlanning::states::Charging) {
      energyManagement.addToCommunicationDrain();
    }

    if (activityPlanning.currentState != ActivityPlanning::states::Killed) {

      Message myMsg = {};
      myMsg.senderId = robotPosition.getId();
      myMsg.x = newGoal.getGoalX();
      myMsg.y = newGoal.getGoalY();
      myMsg.currentRoom = newGoal.getRoom();
      myMsg.groupId = agent.getGroupId();
      //myMsg.currentState = activityPlanning.currentState;
      myMsg.percentage = energyManagement.getPercentage();
      myMsg.startUpMessage = false;
      myMsg.nextId = agentList.getHighestId() + 1;
      snprintf(myMsg.msgContent, sizeof(myMsg.msgContent), "UpdateMessage");
      agent.setLastMsgTime(millis());
      agentList.addOrUpdateAgent(agent);

      communication.sendMessage(BROADCAST_ADDR, myMsg);
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void loop() {
  //andreasTestMovement();
  if (activityPlanning.currentState != ActivityPlanning::states::Killed) {
    switch (activityPlanning.currentState) {
        //============================================================  MOVING  ===========================================================================
      case ActivityPlanning::states::Moving:
        if (energyManagement.getPercentage() == 0) {
          activityPlanning.currentState = ActivityPlanning::states::Killed;

        } else if ((activityPlanning.valuableObjectsInRoom.empty() || energyManagement.getPercentage() < 10) && !activityPlanning.chargeGoalSet) {
          activityPlanning.currentState = ActivityPlanning::states::Planning;

        } else if (!(fabs(newGoal.getGoalX() - robotPosition.getXPosition()) < 0.3) || !(fabs(newGoal.getGoalY() - robotPosition.getYPosition()) < 0.3)) {
          double direction = navigation.getDirection();
          movement.moveDirection(direction, &robotPosition);
          energyManagement.energyDrain(true, false, false);

        } else {
          if (activityPlanning.chargeGoalSet) {
            activityPlanning.currentState = ActivityPlanning::states::Charging;

          } else if (newGoal.getIsComZone()) {
            activityPlanning.currentState = ActivityPlanning::states::Planning;

          } else {
            activityPlanning.currentState = ActivityPlanning::states::Scanning;
          }

          robotPosition.updateRoom(newGoal.getRoom());
          movement.stopMoving();
          energyManagement.turnOffDraining();
        }
        break;

        //=============================================================== SCANNING  ======================================================================
      case ActivityPlanning::states::Scanning:
        movement.stopMoving();
        energyManagement.turnOffDraining();
        //Start energi drain
        energyManagement.energyDrain(false, false, true);

        if (newGoal.getIsZone()) {  //----------------------------Scanning empty zone-----------------------------------------------------------------
          Serial.println("Reached goal. Scanning room zone...");

          //Face north
          //Check if other agents are in range of radar
          int listSize = agentList.getTotalAgents();
          bool waitScan = false;
          for (int i = 0; i < (int)listSize; i++) {
            Agent* agent = agentList.getAgentByIndex(i);
            bool inRange = radar.isUnitInCone(agent->getX(), agent->getY(),
                                              robotPosition.getXPosition(), robotPosition.getYPosition(),
                                              robotPosition.getLastDirection(), 90,
                                              6);  //180 degree cone, 6 m away(90 + 90 = 180)
            // if (inRange && (agent->getRoom() == robotPosition.getRoom())) {
            //   waitScan = true;
            // }
          }
          // if (waitScan) {
          //   break;  //Wait if agents are nearby
          // }


          //Scan for people
          //Loop through 360 degrees / 3
          for (int d = 0; d < 3; d++) {
            bool alarmTriggered = false;
            if (radar.getSimulatedStatus()) {
              delay(1000);
              //////////////////////Simulated - scan area
              //Check if simulated people would be within scan radius
              for (int i = 0; i < (int)personList.getTotalPeople(); i++) {
                Person* person = personList.getPersonByIndex(i);
                bool tangoFound = radar.isUnitInCone(person->getX(), person->getY(),
                                                     robotPosition.getXPosition(), robotPosition.getYPosition(),
                                                     robotPosition.getLastDirection(), 60,
                                                     4);
                if (tangoFound && (person->getRoom() == robotPosition.getRoom())) {
                  alarmTriggered = true;
                  break;
                }
              }
              //////////////////////Non-Simulated - scan area
            } else {
              while (radar.getStationarySignal() <= 0) {
                radar.readValue();
                delay(100);
              }
              for (int d = 0; d < 3; d++) {
                radar.readValue();
                int numberOfTries = 0;
                int ammMovingDetected = 0;
                Serial.println("MovingDetected:");
                Serial.println(radar.isMovingDetected());
                while (numberOfTries <= 10) {
                  delay(100);
                  radar.readValue();
                  numberOfTries++;
                  if (radar.isMovingDetected()) {
                    ammMovingDetected++;
                    if (ammMovingDetected == 5) {
                      alarmTriggered = true;
                      break;
                    }
                  }
                }
                robotPosition.setLastDirection(robotPosition.getLastDirection() + 120);
              }
            }
            if (alarmTriggered) {
              std::vector<Goal> goalsToSend = activityPlanning.Alarming(currentRoom);

              Message myMsg = {};
              for (int i = 0; i < 4; i++) {
                myMsg.goals[i].targetX = goalsToSend[i].getGoalX();
                myMsg.goals[i].targetY = goalsToSend[i].getGoalY();
                myMsg.goals[i].targetRoom = goalsToSend[i].getRoom();
              }

              myMsg.senderId = robotPosition.getId();
              myMsg.x = newGoal.getGoalX();
              myMsg.y = newGoal.getGoalY();
              myMsg.startUpMessage = false;
              snprintf(myMsg.msgContent, sizeof(myMsg.msgContent), "AlarmOrders");

              communication.sendMessage(BROADCAST_ADDR, myMsg);
              activityPlanning.setAlarmActivated(true);

              Serial.println("Sent message with goals");
              break;
            } else {
              if (activityPlanning.getAlarmActivated()) {
                activityPlanning.setAlarmActivated(false);
              }
            }
            robotPosition.setLastDirection(robotPosition.getLastDirection() + 120);
          }
        } else {  //---------------------------------------------- Scanning valuable objects ---------------------------------------------------------------------

          Serial.println("Reached goal. Scanning valuable object...");
          //face goal
          float dx = newGoal.getGoalX() - robotPosition.getXPosition();
          float dy = newGoal.getGoalY() - robotPosition.getYPosition();
          float angleRad = atan2(dy, dx);
          float angleDeg = angleRad * 180.0 / 3.14159265;
          //if (angleDeg < 0) angleDeg += 360.0;
          robotPosition.setLastDirection(angleDeg);
          //Check if other agents are in range of radar
          int listSize = agentList.getTotalAgents();
          bool waitScan = false;
          for (int i = 0; i < (int)listSize; i++) {
            Agent* agent = agentList.getAgentByIndex(i);
            bool inRange = radar.isUnitInCone(agent->getX(), agent->getY(),
                                              robotPosition.getXPosition(), robotPosition.getYPosition(),
                                              robotPosition.getLastDirection(), 90,
                                              6);  //180 degree cone, 6 m away(90 + 90 = 180)
            // if (inRange) {
            //   waitScan = true;
            // }
          }
          // if (waitScan) {
          //   break;  //Wait if agents are nearby
          // }

          bool alarmTriggered = false;
          /////////////////////////////Simulated - valuable object
          if (radar.getSimulatedStatus()) {
            delay(3000);
            if (activityPlanning.isValObjectStolen(currentRoom.getRoomNbr())) {
              alarmTriggered = true;
            } else {
              alarmTriggered = !(radar.isUnitInCone(newGoal.getGoalX(), newGoal.getGoalY(),
                                                    robotPosition.getXPosition(), robotPosition.getYPosition(),
                                                    robotPosition.getLastDirection(), 60,
                                                    4));

              // Serial.print("X: ");
              // Serial.print(newGoal.getGoalX());
              // Serial.print(" Y: ");
              // Serial.println(newGoal.getGoalY());

              // Serial.print("RX: ");
              // Serial.print(robotPosition.getXPosition());
              // Serial.print(" RY: ");
              // Serial.println(robotPosition.getYPosition());

              // Serial.print("Direction: ");
              // Serial.println(robotPosition.getLastDirection());
            }


          }
          /////////////////////////////Non-Simulated - valuable object
          else {
            float dx = robotPosition.getXPosition() - newGoal.getGoalX();
            float dy = robotPosition.getYPosition() - newGoal.getGoalY();
            float distance = sqrtf(dx * dx + dy * dy);  //<- pythagoras
            radar.readValue();
            while (radar.getStationarySignal() <= 0) {
              radar.readValue();
              delay(100);
            }
            Serial.println("Stationary:");
            Serial.println(radar.isStationaryDetected());
            Serial.println("Distance:");
            Serial.println(radar.getStationaryDistance());
            Serial.println(radar.isStationaryDetected());
            if (radar.isStationaryDetected()) {
              int numberOfTries = 0;
              int errorCount = 0;
              while (numberOfTries < 20) {
                radar.readValue();
                delay(100);
                numberOfTries++;
                Serial.println("Stationary distance numtries:");
                Serial.println(radar.getStationaryDistance());
                if ((radar.getStationaryDistance() > (distance + 100)) || (radar.getStationaryDistance() < (distance - 100))) { errorCount++; }

                if (errorCount > 5) {
                  Serial.println("Distance triggered:");
                  Serial.println(radar.getStationaryDistance());
                  alarmTriggered = true;
                  break;
                }
              }
            } else {
              alarmTriggered = true;
            }
          }


          if (!alarmTriggered) {
            Serial.println("Object found.");
            if (activityPlanning.getAlarmActivated()) {
              activityPlanning.setAlarmActivated(false);
            }
          } else {
            Serial.println("Object not found!");
            std::vector<Goal> goalsToSend = activityPlanning.Alarming(currentRoom);

            Message myMsg = {};
            for (int i = 0; i < 4; i++) {
              myMsg.goals[i].targetX = goalsToSend[i].getGoalX();
              myMsg.goals[i].targetY = goalsToSend[i].getGoalY();
              myMsg.goals[i].targetRoom = goalsToSend[i].getRoom();
            }

            myMsg.senderId = robotPosition.getId();
            myMsg.x = newGoal.getGoalX();
            myMsg.y = newGoal.getGoalY();
            myMsg.startUpMessage = false;
            snprintf(myMsg.msgContent, sizeof(myMsg.msgContent), "AlarmOrders");

            communication.sendMessage(BROADCAST_ADDR, myMsg);
            activityPlanning.setAlarmActivated(true);

            Serial.println("Sent message with goals");
          }
        }

        activityPlanning.RemoveObj();
        activityPlanning.currentState = ActivityPlanning::states::Planning;
        energyManagement.turnOffDraining();
        radar.resetValues();
        break;

        //================================================================= PLANNING =================================================================
      case ActivityPlanning::states::Planning:
        movement.stopMoving();
        energyManagement.turnOffDraining();

        delay(2000);
        chargerOccupied = activityPlanning.getChargerOccupied(robotPosition);

        if (initialStart || agent.getGroupId() == -1) {
          Serial.print("totalEsps: ");
          Serial.println(totalEsps);
          Serial.print("newEspCount: ");
          Serial.println(newEspCount);

          activityPlanning.valuableObjectsInRoom.clear();
          groupId = activityPlanning.decideArea(idForEsp, totalEsps);
          agent.setGroupId(groupId);
          agent.setNextId(agentList.getHighestId() + 1);

          //initialStart = false;

        } else if (energyManagement.getPercentage() < 50 && !chargerOccupied) {
          Serial.println("Getting charger goal...");
          //newGoal = activityPlanning.getChargerGoal(robotPosition.getId());
          newGoal = activityPlanning.getChargerGoalByIndex(idForEsp);
          Agent* agent = agentList.getAgentPointerById(robotPosition.getId());
          agent->setGoalSetToCharger(true);
          activityPlanning.chargeGoalSet = true;
          navigation.setMainGoal(newGoal);
          activityPlanning.currentState = ActivityPlanning::states::Moving;

        } else if (activityPlanning.getAlarmActivated() && activityPlanning.getGoalToGuard().getRoom() != 0) {
          newGoal = activityPlanning.getGoalToGuard();
          navigation.setMainGoal(newGoal);
          activityPlanning.currentState = ActivityPlanning::states::Moving;

        } else if (espCountChanged && activityPlanning.valuableObjectsInRoom.empty()) {
          if (totalEsps > newEspCount) {
            Serial.println("TotalEsps decreased!");
          } else if (totalEsps < newEspCount) {
            Serial.println("TotalEsps increased!");
          }
          totalEsps = newEspCount;
          Serial.print("totalEsps: ");
          Serial.println(totalEsps);
          Serial.print("newEspCount: ");
          Serial.println(newEspCount);

          activityPlanning.valuableObjectsInRoom.clear();
          groupId = activityPlanning.decideArea(idForEsp, totalEsps);
          agent.setGroupId(groupId);
          agent.setNextId(agentList.getHighestId() + 1);
          espCountChanged = false;
          //initialStart = false;

        } else if (activityPlanning.valuableObjectsInRoom.empty()) {
          currentRoom = activityPlanning.getNextRoomForAgent(robotPosition.getId());
          std::rotate(activityPlanning.roomsToGuard.begin(), activityPlanning.roomsToGuard.begin() + 1, activityPlanning.roomsToGuard.end());
          newGoal = activityPlanning.getNextValObject(currentRoom);
          navigation.setMainGoal(newGoal);
          activityPlanning.currentState = ActivityPlanning::states::Moving;

        } else {
          newGoal = activityPlanning.getNextValObject(currentRoom);
          navigation.setMainGoal(newGoal);
          activityPlanning.currentState = ActivityPlanning::states::Moving;
        }

        break;
        //============================================================  CHARGING  ========================================================================

      case ActivityPlanning::states::Charging:
        movement.stopMoving();
        energyManagement.turnOffDraining();
        percentage = energyManagement.charging();

        inGroupLowBattery = activityPlanning.isGroupLowBattery(robotPosition);

        if ((percentage == 100) || (inGroupLowBattery && percentage > 50)) {
          Agent agent = agentList.getAgentById(robotPosition.getId());
          agent.setGoalSetToCharger(false);
          energyManagement.setPercentage(percentage);
          activityPlanning.chargeGoalSet = false;
          activityPlanning.currentState = ActivityPlanning::states::Planning;
        }

        break;

      case ActivityPlanning::states::Killed:
        movement.stopMoving();
        energyManagement.turnOffDraining();
        delay(200);
        break;

      case ActivityPlanning::states::Waiting:
        movement.stopMoving();
        energyManagement.turnOffDraining();

        if (millis() - startTime >= 10000 && newGoal.getIsComZone()) {
          //if (millis() - startTime >= 310000 && newGoal.getIsComZone()) {
          Serial.println("Timern är slut");
          timerStarted = false;
          newGoal.setIsComZone(false);
          activityPlanning.RemoveObj();
          activityPlanning.currentState = ActivityPlanning::states::Planning;
        }

        break;
    }

    //totalEsps = activityPlanning.countNbrEsps();
    // if (energyManagement.getPercentage() > 10 && activityPlanning.chargeGoalSet == true) {
    //   activityPlanning.chargeGoalSet = false;
    //   activityPlanning.currentState = ActivityPlanning::states::Planning;
    // }

    if (!initialStart) {
      newEspCount = activityPlanning.checkAgentStatus();
      if (totalEsps != newEspCount) {
        espCountChanged = true;
      }
    }
  }

  initialStart = false;
  delay(20);

  //========================================================= COMMUNICATION =======================================================================
  //TODO: Hantera meddelanden som har kommit in (Och eventuellt ändra state/beliefs beroende på vad meddelandet innehåller?)
}

void andreasTestMovement() {
  Serial.println("skriv rummsnr för robot");
  while (Serial.available())
    Serial.read();
  while (Serial.available() == 0) {
  }
  String input = Serial.readStringUntil('\n');
  input.trim();
  int newRoomR = input.toInt();
  robotPosition.updateRoom(newRoomR);
  Serial.print("rum robot: ");
  Serial.println(newRoomR);

  Serial.println("skriv robotX");
  while (Serial.available() == 0) {
  }
  input = Serial.readStringUntil('\n');
  input.trim();
  inputX = input.toDouble();
  robotPosition.updateXPosition(inputX);
  Serial.print("X: ");
  Serial.println(inputX);

  Serial.println("skriv robotY");
  while (Serial.available() == 0) {
  }
  input = Serial.readStringUntil('\n');
  input.trim();
  inputY = input.toDouble();
  robotPosition.updateYPosition(inputY);
  Serial.print("Y: ");
  Serial.println(inputY);

  Goal newGoal;
  Serial.println("skriv rummsnr för mål");
  while (Serial.available()) Serial.read();
  while (Serial.available() == 0) {
  }
  input = Serial.readStringUntil('\n');
  input.trim();
  int newRoom = input.toInt();
  newGoal.setRoom(newRoom);
  Serial.print("rum mål: ");
  Serial.println(newRoom);

  Serial.println("skriv goalX");
  while (Serial.available()) Serial.read();
  while (Serial.available() == 0) {
  }
  input = Serial.readStringUntil('\n');
  input.trim();
  inputX = input.toDouble();
  newGoal.setGoalX(inputX);

  Serial.print("X: ");
  Serial.println(inputX);

  Serial.println("skriv goalY");
  while (Serial.available()) Serial.read();
  while (Serial.available() == 0) {
  }
  input = Serial.readStringUntil('\n');
  input.trim();
  inputY = input.toDouble();
  newGoal.setGoalY(inputY);
  Serial.print("Y: ");
  Serial.println(inputY);
  Serial.println();
  navigation.setMainGoal(newGoal);
  while (!(fabs(newGoal.getGoalX() - robotPosition.getXPosition()) < 0.3) || !(fabs(newGoal.getGoalY() - robotPosition.getYPosition()) < 0.3)) {
    double direction = navigation.getDirection();
    /*  Serial.print("Direction from navigation: ");
     Serial.println(direction); */
    movement.moveDirection(direction, &robotPosition);
    /* Serial.print("New x-position ");
    Serial.println(robotPosition.getXPosition());
    Serial.print("New y-position ");
    Serial.println(robotPosition.getYPosition()); */
    delay(20);
  }
  movement.stopMoving();
  //movement.clearforNewMovement();
}
