#ifndef ROOM_CONTROLLER_H
#define ROOM_CONTROLLER_H
#include <vector>
#include "Room.h"
#include "ValuableObject.h"
#include "ChargeStation.h"
#include <map>


class RoomController {
private:
  std::vector<Room> rooms;
  std::map<int, std::vector<int>> roomGroups;
  void addRoom(Room room);
  void addRoomToGroup(int groupID, int roomNr);

public:
  RoomController();
  void initRooms();
  bool addValuableObjectToRoom(ValuableObject valuableObject);
  ValuableObject getValuableObject(int valuableObjectId);
  int checkValuableObjectRoom(int valuableObjectId);
  Room getRoom(int roomNr);
  void printInformationAboutRoom(int roomNr);
  ValuableObject* getValuableObjectPointer(int valuableObjectId);
  Room* getRoomPointer(int roomNr);
  //--------Laddstationerna-----------
  void addChargeStation(const ChargeStation& station);
  bool updateChargingStatus(double robotX, double robotY);
  bool isRoomInGroup(int groupID, int roomNr);
  std::vector<int> getRoomsInGroup(int groupId);
  std::vector<ChargeStation> chargeStations;
  bool isPointInsideRoom(int roomNr, double x, double y);
  void setupGroups(int nbr);

  //void setupGroups(int nbr);
};


#endif
