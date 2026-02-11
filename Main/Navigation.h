#ifndef NAVIGATION_H
#define NAVIGATION_H
#include "Goal.h"
#include "RoomController.h"
#include "Position.h"
#include "DetectionController.h"
#include <queue>
#include <vector>

#include <Arduino.h>

typedef struct{
  Goal roomGoal; 
  int nextRoom; 
} PassageInfo; 

typedef struct{
  double x;
  double y;
  double length; 
  double angleRad; 
  double angleDeg; 
} RoomGoalDirectionInfo;

typedef struct{
  double x;
  double y;
} point;

	typedef struct{
		double x;
		double y;
	} NavigationVector; 

class Navigation {
private:
  bool realDetection = false; 
  Goal mainGoal; 
  Goal roomGoal; 
  Goal lastGoal; 
  double barrierProximityThreshold = 0.4; 
  bool gointThruPassage = false; 
  int passageCounter = 0; 
  RoomController* roomController;
  Position* robotPosition; 
  DetectionController* detectionController; 
  std::queue<Goal> navigationQueue; 
  std::vector<BlockedSpaceBlock> getBlockedSpacesWall(char wallDirection); // w,n,e,s
  

public:
  Navigation(RoomController* roomController, Position* robotPosition, DetectionController* detectionController);
  void setRealDetection(bool realDetection); 
  void setupNavigation();
  void setMainGoal(Goal mainGoal); 
  Goal getMainGoal(); 
  void setBarrierProximityThreshold(double threshold); 
  double getBarrierProximityThreshold(); 
  RoomGoalDirectionInfo getRoomGoalDirectionInfo(double ownX, double ownY, double goalX, double goalY); 
  double getDirection(); 
  double getDirectDirectionToGoal();
  //double getLastDirection();
  void test();
  double getDimensionCoordinateBarrierInFront(Room currentRoom, char direction, double robotX, double robotY, std::vector<BlockedSpaceBlock> blockedSpacesInRoom); 
  double getDimensionCoordinateBlockedSpaceInFront(char direction, double robotX, double robotY, std::vector<BlockedSpaceBlock> blockedSpacesInRoom); 
  int checkIfBlockOrWallInFront(char direction, double robotX, double robotY, std::vector<BlockedSpaceBlock> blockedSpaces); 
  PassageInfo getPassageInfoToNextRoom(int currentRoom, int goalRoom); 
  int getNextRoomNrOnWayToMainGoal(int currentRoom, int goalRoom); 
  bool getSimulatedBarrierDetectedFront(double detectionLimit, double lastDirection, double robotX, double robotY, double northBarrier, double eastBarrier, double southBarrier, double westBarrier); 
  bool getSimulatedBarrierDetectedRight(double detectionLimit, double lastDirection, double robotX, double robotY, double northBarrier, double eastBarrier, double southBarrier, double westBarrier); 
  bool getFrontIRDetectorValue(); 
  bool getRightIRDetectorValue();
  bool checkIfPassageInFront(Room currentRoom, double lastDirection, double ownX, double ownY);
  bool checkIfClearDriveToGoalDiagonal(Room currentRoom, double ownX, double ownY, double goalX, double goalY);
  bool checkIfLinesCross(point firstLineStart, point firstLineEnd, point secondLineStart, point secondLineEnd); 

};


#endif