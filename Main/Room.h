#ifndef ROOM_H
#define ROOM_H
#include "Space.h"
#include <vector>
#include <string>
#include "ValuableObject.h"
#include "BlockedSpaceBlock.h"
#include "Passage.h"


class Room : public Space {
private:
  std::vector<ValuableObject> valuableObjectsToNotRemove;
  std::vector<ValuableObject> valuableObjects;
  std::vector<BlockedSpaceBlock> blockedSpaceBlocks;
  std::vector<Passage> passages;

public:
  Room();
  Room(double xLow, double xHigh, double yLow, double yHigh, int roomNr);

  std::vector<BlockedSpaceBlock> getBlockedSpaceBlocks() const;
  std::vector<ValuableObject> getValuableObjects() const;
  std::vector<Passage> getPassages() const;

  void addValuableObject(ValuableObject valuableObject);
  bool checkPresenceValuableObject(int id);
  bool checkPresenceValuableObject(std::string name);
  ValuableObject removeValuableObject(std::string name);
  ValuableObject removeValuableObject(int id);
  int getRoomNbr() const;
  

  ValuableObject* getValuableObject(int id);

  void addBlockedSpaceBlock(BlockedSpaceBlock blockedSpaceBlock);
  bool removeBlockedSpaceBlock(double x, double y);

  void addPassage(Passage passage);
  bool removePassage(int room1, int room2, double passageCoordLow);
};

#endif
