#include "Navigation.h"
#include <cmath>
#include <random>

Navigation::Navigation(RoomController *roomController, Position *robotPosition, DetectionController *detectionController)
  : roomController(roomController), robotPosition(robotPosition), detectionController(detectionController) {}

void Navigation::setupNavigation() {
	test();
}

bool Navigation::getSimulatedBarrierDetectedFront(double detectionLimit, double lastDirection, double robotX, double robotY, double northBarrier, double eastBarrier, double southBarrier, double westBarrier) {
	//Randomization of simulated detection with +- 2 cm.
	bool returnSimulatedDetectionValue = false;
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<double> errorDetection(0, 0.08);
	double errorDetectionRandomized = errorDetection(gen);

	double detectionLimitWithError = (detectionLimit - 0.04) + errorDetectionRandomized;

	//detectionLimitWithError = detectionLimit;

	if (lastDirection < 20 || lastDirection > 340) {
		if (northBarrier - robotY < detectionLimitWithError) {
			returnSimulatedDetectionValue = true;
			//Serial.println("SimulatedIRFrontDetecting north barrier < 0.4");
		}
	} else if (lastDirection > 70 && lastDirection < 110) {
		if (eastBarrier - robotX < detectionLimitWithError) {
			returnSimulatedDetectionValue = true;
			//Serial.println("SimulatedIRFrontDetecting east barrier < 0.4");
		}
	} else if (lastDirection > 160 && lastDirection < 200) {
		if (robotY - southBarrier < detectionLimitWithError) {
			returnSimulatedDetectionValue = true;
			//Serial.println("SimulatedIRFrontDetecting south barrier < 0.4");
		}
	} else if (lastDirection > 250 && lastDirection < 290) {
		if (robotX - westBarrier < detectionLimitWithError) {
			returnSimulatedDetectionValue = true;
			//Serial.println("SimulatedIRFrontDetecting west barrier < 0.4");
		}
	}

	return returnSimulatedDetectionValue;
}

bool Navigation::getSimulatedBarrierDetectedRight(double detectionLimit, double lastDirection, double robotX, double robotY, double northBarrier, double eastBarrier, double southBarrier, double westBarrier) {
	bool returnSimulatedDetectionValue = false;
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<double> errorDetection(0, 0.08);
	double errorDetectionRandomized = errorDetection(gen);

	double detectionLimitWithError = (detectionLimit - 0.04) + errorDetectionRandomized;

	if (lastDirection < 20 || lastDirection > 340) {
		if (eastBarrier - robotX < detectionLimitWithError) {
			returnSimulatedDetectionValue = true;
		}
	} else if (lastDirection > 70 && lastDirection < 110) {
		if (robotY - southBarrier < detectionLimitWithError) {
			returnSimulatedDetectionValue = true;
		}
	} else if (lastDirection > 160 && lastDirection < 200) {
		if (robotX - westBarrier < detectionLimitWithError) {
			returnSimulatedDetectionValue = true;
		}
	} else if (lastDirection > 250 && lastDirection < 290) {
		if (northBarrier - robotY < detectionLimitWithError) {
			returnSimulatedDetectionValue = true;
		}
	}

	return returnSimulatedDetectionValue;
}

bool Navigation::getFrontIRDetectorValue() {
	bool returnFrontIRDetectionValue = false;

	if (detectionController->getFrontIRValue()) {
		returnFrontIRDetectionValue = true;
		//Serial.println("Real IR detection front indicating true");
		//Serial.println("SimulatedIRFrontDetecting north barrier < 0.4");
	}
	return returnFrontIRDetectionValue;
}

bool Navigation::getRightIRDetectorValue() {
	bool rightIRDetectionValue = false;

	if (detectionController->getRightIRValue()) {
		rightIRDetectionValue = true;
		//Serial.println("SimulatedIRFrontDetecting north barrier < 0.4");
	}

	return rightIRDetectionValue;
}

void Navigation::test() {
	//Serial.println("Test from Positioning.cpp.");
}

void Navigation::setRealDetection(bool realDetectionBool) {
	realDetection = realDetectionBool;
}

void Navigation::setMainGoal(Goal goal) {
	this->mainGoal = goal;
}

Goal Navigation::getMainGoal() {
	return this->mainGoal;
}

void Navigation::setBarrierProximityThreshold(double threshold) {
	this->barrierProximityThreshold = threshold;
}
double Navigation::getBarrierProximityThreshold() {
	return this->barrierProximityThreshold;
}

PassageInfo Navigation::getPassageInfoToNextRoom(int currentRoom, int goalRoom) {
	PassageInfo passageInfo;
	Goal returnGoal;
	returnGoal.setRoom(currentRoom);
	switch (currentRoom) {
		case 1:
			returnGoal.setGoalX(7.5);
			returnGoal.setGoalY(47);
			passageInfo.nextRoom = 2;
			break;
		case 2:
			if (goalRoom == 1) {
				returnGoal.setGoalX(7.8);
				returnGoal.setGoalY(47);
				passageInfo.nextRoom = 1;
			} else {
				returnGoal.setGoalX(7.5);
				returnGoal.setGoalY(42);
				passageInfo.nextRoom = 3;
			}
			break;
		case 3:
			if (goalRoom == 2 || goalRoom == 1) {
				returnGoal.setGoalX(7.8);
				returnGoal.setGoalY(42);
				passageInfo.nextRoom = 2;
			} else if (goalRoom >= 4 && goalRoom <= 10) {
				returnGoal.setGoalX(17);
				returnGoal.setGoalY(27);
				passageInfo.nextRoom = 4;
			} else {
				returnGoal.setGoalX(6.65);
				returnGoal.setGoalY(19);
				passageInfo.nextRoom = 11;
			}
			break;
		case 4:
			switch (goalRoom) {
				case 5:
					returnGoal.setGoalX(23.2);
					returnGoal.setGoalY(23.5);
					passageInfo.nextRoom = 5;
					break;
				case 6:
					returnGoal.setGoalX(25.7);
					returnGoal.setGoalY(23.5);
					passageInfo.nextRoom = 6;
					break;
				case 7:
					returnGoal.setGoalX(31.6);
					returnGoal.setGoalY(23.5);
					passageInfo.nextRoom = 7;
					break;
				case 8:
					returnGoal.setGoalX(36.2);
					returnGoal.setGoalY(23.5);
					passageInfo.nextRoom = 8;
					break;
				case 9:
					returnGoal.setGoalX(39.1);
					returnGoal.setGoalY(23.5);
					passageInfo.nextRoom = 9;
					break;
				case 10:
					returnGoal.setGoalX(42.5);
					returnGoal.setGoalY(22.6);
					passageInfo.nextRoom = 10;
					break;
				default:
					returnGoal.setGoalX(17);
					returnGoal.setGoalY(27.7);
					passageInfo.nextRoom = 3;
					break;
			}
			break;
		case 5:
			returnGoal.setGoalX(21.5);
			returnGoal.setGoalY(23.5);
			passageInfo.nextRoom = 4;
			break;
		case 6:
			returnGoal.setGoalX(24.9);
			returnGoal.setGoalY(23.5);
			passageInfo.nextRoom = 4;
			break;
		case 7:
			returnGoal.setGoalX(30.7);
			returnGoal.setGoalY(23.5);
			passageInfo.nextRoom = 4;
			break;
		case 8:
			returnGoal.setGoalX(35.8);
			returnGoal.setGoalY(23.5);
			passageInfo.nextRoom = 4;
			break;
		case 9:
			returnGoal.setGoalX(38.2);
			returnGoal.setGoalY(23.5);
			passageInfo.nextRoom = 4;
			break;
		case 10:
			returnGoal.setGoalX(42.5);
			returnGoal.setGoalY(23.3);
			passageInfo.nextRoom = 4;
			break;
		case 11:
			if (goalRoom <= 10) {
				returnGoal.setGoalX(7.4);
				returnGoal.setGoalY(19);
				passageInfo.nextRoom = 3;
			} else {
				returnGoal.setGoalX(50.8);
				returnGoal.setGoalY(15.3);
				passageInfo.nextRoom = 12;
			}
			break;
		case 12:
			if (goalRoom <= 11) {
				returnGoal.setGoalX(50.8);
				returnGoal.setGoalY(16);
				passageInfo.nextRoom = 11;
			} else if (goalRoom == 13 || goalRoom == 14) {
				returnGoal.setGoalX(56.2);
				returnGoal.setGoalY(19.8);
				passageInfo.nextRoom = 13;
			} else {
				returnGoal.setGoalX(55.2);
				returnGoal.setGoalY(9.8);
				passageInfo.nextRoom = 16;
			}
			break;
		case 13:
			if (goalRoom == 14) {
				returnGoal.setGoalX(51.2);
				returnGoal.setGoalY(30.2);
				passageInfo.nextRoom = 14;
			} else {
				returnGoal.setGoalX(55.4);
				returnGoal.setGoalY(19.8);
				passageInfo.nextRoom = 12;
			}
			break;
		case 14:
			returnGoal.setGoalX(47.5);
			returnGoal.setGoalY(30.2);
			passageInfo.nextRoom = 13;
			break;
		case 15:
			returnGoal.setGoalX(49.8);
			returnGoal.setGoalY(1.9);
			passageInfo.nextRoom = 16;
			break;
		case 16:
			if (goalRoom == 15) {
				returnGoal.setGoalX(49.8);
				returnGoal.setGoalY(8.5);
				passageInfo.nextRoom = 15;
			} else {
				returnGoal.setGoalX(55.7);
				returnGoal.setGoalY(9.8);
				passageInfo.nextRoom = 12;
			}
			break;
	}
	passageInfo.roomGoal = returnGoal;
	return passageInfo;
}

RoomGoalDirectionInfo Navigation::getRoomGoalDirectionInfo(double ownX, double ownY, double goalX, double goalY) {
	RoomGoalDirectionInfo roomGoalDirectionInfo;

	roomGoalDirectionInfo.x = goalX - ownX;
	roomGoalDirectionInfo.y = goalY - ownY;

	roomGoalDirectionInfo.length = std::sqrt(roomGoalDirectionInfo.x * roomGoalDirectionInfo.x + roomGoalDirectionInfo.y * roomGoalDirectionInfo.y);

	roomGoalDirectionInfo.angleRad = std::atan2(roomGoalDirectionInfo.x, roomGoalDirectionInfo.y);
	roomGoalDirectionInfo.angleDeg = roomGoalDirectionInfo.angleRad * 180.0 / M_PI;

	if (roomGoalDirectionInfo.angleDeg < 0) {
		roomGoalDirectionInfo.angleDeg = roomGoalDirectionInfo.angleDeg += 360;
	}

	return roomGoalDirectionInfo;
}

bool Navigation::checkIfPassageInFront(Room currentRoom, double lastDirection, double ownX, double ownY) {
	bool passageInFront = false;
	std::vector<Passage> passages;
	passages = currentRoom.getPassages();

	for (int i = 0; i < passages.size(); i++) {
		int passageDirection = passages[i].getPassageDirection();
		double passageCrossCoord = passages[i].getCrossCoord();
		double passageCoordLow = passages[i].getPassageCoordLow();
		double passageCoordHigh = passages[i].getPassageCoordHigh();

		if (lastDirection < 20 || lastDirection > 340)  //North
		{
			if (passageDirection == 0 && passageCrossCoord > ownY && passageCoordLow < ownX && passageCoordHigh > ownX) {
				//Serial.println("Passage north in front, do not validate");
				passageInFront = true;
				return passageInFront;
			}
		} else if (lastDirection > 70 && lastDirection < 110)  //East
		{
			if (passageDirection == 1 && passageCrossCoord > ownX && passageCoordLow < ownY && passageCoordHigh > ownY) {
				//Serial.println("Passage east in front, do not validate");
				passageInFront = true;
				return passageInFront;
			}
		} else if (lastDirection > 160 && lastDirection < 200)  //South
		{
			if (passageDirection == 0 && passageCrossCoord < ownY && passageCoordLow < ownX && passageCoordHigh > ownX) {
				//Serial.println("Passage south in front, do not validate");
				passageInFront = true;
				return passageInFront;
			}
		} else if (lastDirection > 250 && lastDirection < 290)  //West
		{
			if (passageDirection == 1 && passageCrossCoord < ownX && passageCoordLow < ownY && passageCoordHigh > ownY) {
				//Serial.println("Passage west in front, do not validate");
				passageInFront = true;
				return passageInFront;
			}
		}
	}
	return passageInFront;
}

double Navigation::getDirection() {
	detectionController->updateIRSensors(); 
	detectionController->updateButtonValues(); 
	Room currentRoom = roomController->getRoom(robotPosition->getRoom());

	double robotX = robotPosition->getXPosition();
	double robotY = robotPosition->getYPosition();
	double lastDirection = robotPosition->getLastDirection();
	double diagonalRunningLimit = 1.3;
	bool northWardDirection = false;
	bool eastWardDirection = false;
	bool southWardDirection = false;
	bool westWardDirection = false;

	if (lastDirection > 350 || lastDirection < 10) {
		northWardDirection = true;
	} else if (lastDirection > 80 && lastDirection < 100) {
		eastWardDirection = true;
	} else if (lastDirection > 170 && lastDirection < 190) {
		southWardDirection = true;
	} else if (lastDirection > 260 && lastDirection < 280) {
		westWardDirection = true;
	}

	std::vector<BlockedSpaceBlock> blockedSpacesInRoom = currentRoom.getBlockedSpaceBlocks();
	// Gets all barriers in four directions from robot
	double northBarrier = getDimensionCoordinateBarrierInFront(currentRoom, 'n', robotX, robotY, blockedSpacesInRoom);
	double southBarrier = getDimensionCoordinateBarrierInFront(currentRoom, 's', robotX, robotY, blockedSpacesInRoom);
	double westBarrier = getDimensionCoordinateBarrierInFront(currentRoom, 'w', robotX, robotY, blockedSpacesInRoom);
	double eastBarrier = getDimensionCoordinateBarrierInFront(currentRoom, 'e', robotX, robotY, blockedSpacesInRoom);

	// Get all barriers one step from robotposition. Used to get around corners.
	double oneStepNorthY = robotY + 0.2;
	double westBarrierOneStepNorth = getDimensionCoordinateBarrierInFront(currentRoom, 'w', robotX, oneStepNorthY, blockedSpacesInRoom);
	// double eastBarrierOneStepNorth = getDimensionCoordinateBarrierInFront(currentRoom,'e', robotX, oneStepNorthY, blockedSpacesInRoom);
	double oneStepSouthY = robotY - 0.2;
	// double westBarrierOneStepSouth = getDimensionCoordinateBarrierInFront(currentRoom,'w', robotX, oneStepSouthY, blockedSpacesInRoom);
	double eastBarrierOneStepSouth = getDimensionCoordinateBarrierInFront(currentRoom, 'e', robotX, oneStepSouthY, blockedSpacesInRoom);
	double oneStepWestX = robotX - 0.2;
	// double northBarrierOneStepWest = getDimensionCoordinateBarrierInFront(currentRoom, 'n', oneStepWestX, robotY, blockedSpacesInRoom);
	double southBarrierOneStepWest = getDimensionCoordinateBarrierInFront(currentRoom, 's', oneStepWestX, robotY, blockedSpacesInRoom);
	double oneStepEastX = robotX + 0.2;
	double northBarrierOneStepEast = getDimensionCoordinateBarrierInFront(currentRoom, 'n', oneStepEastX, robotY, blockedSpacesInRoom);
	// double southBarrierOneStepEast = getDimensionCoordinateBarrierInFront(currentRoom, 's', oneStepEastX, robotY, blockedSpacesInRoom);

	bool frontIRDetectingNorth = false;
	bool frontIRDetectingEast = false;
	bool frontIRDetectingSouth = false;
	bool frontIRDetectingWest = false;

	if (!realDetection) {
		double simulatedIRDetectionLimit = 0.3;

		if (lastDirection < 10 || lastDirection > 350) {
			frontIRDetectingNorth = getSimulatedBarrierDetectedFront(simulatedIRDetectionLimit, lastDirection, robotX, robotY, northBarrier, eastBarrier, southBarrier, westBarrier);
		} else if (lastDirection > 70 && lastDirection < 100) {
			frontIRDetectingEast = getSimulatedBarrierDetectedFront(simulatedIRDetectionLimit, lastDirection, robotX, robotY, northBarrier, eastBarrier, southBarrier, westBarrier);
		} else if (lastDirection > 170 && lastDirection < 190) {
			frontIRDetectingSouth = getSimulatedBarrierDetectedFront(simulatedIRDetectionLimit, lastDirection, robotX, robotY, northBarrier, eastBarrier, southBarrier, westBarrier);
		} else if (lastDirection > 260 && lastDirection < 280) {
			frontIRDetectingWest = getSimulatedBarrierDetectedFront(simulatedIRDetectionLimit, lastDirection, robotX, robotY, northBarrier, eastBarrier, southBarrier, westBarrier);
		}
	} else {
		//Serial.println("In real detection else");
		double realIRDetectionBarrierLimit = 0.3;
		if (lastDirection < 20 || lastDirection > 340) {
			if (northBarrier - robotY < realIRDetectionBarrierLimit) {
				frontIRDetectingNorth = getFrontIRDetectorValue();
				//Serial.println("realDetection frontIR detection north"); 
				//Serial.println(frontIRDetectingNorth); 
			}
		} else if (lastDirection > 70 && lastDirection < 110) {
			if (eastBarrier - robotX < realIRDetectionBarrierLimit) {
				frontIRDetectingEast = getFrontIRDetectorValue();
				//Serial.println("realDetection frontIR detection east"); 
				//Serial.println(frontIRDetectingEast); 
			}
		} else if (lastDirection > 160 && lastDirection < 200) {
			if (robotY - southBarrier < realIRDetectionBarrierLimit) {
				frontIRDetectingSouth = getFrontIRDetectorValue();
				//Serial.println("realDetection frontIR detection soutn"); 
				//Serial.println(frontIRDetectingSouth); 
			}
		} else if (lastDirection > 250 && lastDirection < 290) {
			if (robotX - westBarrier < realIRDetectionBarrierLimit) {
				frontIRDetectingWest = getFrontIRDetectorValue();
				//Serial.println("realDetection frontIR detection west"); 
				//Serial.println(frontIRDetectingWest); 
			}
		}
	}

	bool rightIRDetectingNorth = false;
	bool rightIRDetectingEast = false;
	bool rightIRDetectingSouth = false;
	bool rightIRDetectingWest = false;

	if (!realDetection) {
		double simulatedIRDetectionLimit = 0.3;

		if (lastDirection < 10 || lastDirection > 350) {
			rightIRDetectingEast = getSimulatedBarrierDetectedRight(simulatedIRDetectionLimit, lastDirection, robotX, robotY, northBarrier, eastBarrier, southBarrier, westBarrier);
		} else if (lastDirection > 70 && lastDirection < 100) {
			rightIRDetectingSouth = getSimulatedBarrierDetectedRight(simulatedIRDetectionLimit, lastDirection, robotX, robotY, northBarrier, eastBarrier, southBarrier, westBarrier);
		} else if (lastDirection > 170 && lastDirection < 190) {
			rightIRDetectingWest = getSimulatedBarrierDetectedRight(simulatedIRDetectionLimit, lastDirection, robotX, robotY, northBarrier, eastBarrier, southBarrier, westBarrier);
		} else if (lastDirection > 260 && lastDirection < 280) {
			rightIRDetectingNorth = getSimulatedBarrierDetectedRight(simulatedIRDetectionLimit, lastDirection, robotX, robotY, northBarrier, eastBarrier, southBarrier, westBarrier);
		}
	} else {
		double realIRDetectionBarrierLimit = 0.3;
		if ((lastDirection < 20 || lastDirection > 340) && eastBarrier - robotX <= realIRDetectionBarrierLimit) {
			rightIRDetectingEast = getRightIRDetectorValue();
		} else if ((lastDirection > 70 && lastDirection < 110) && robotY - southBarrier <= realIRDetectionBarrierLimit) {
			rightIRDetectingSouth = getRightIRDetectorValue();
		} else if ((lastDirection > 170 && lastDirection < 190) && westBarrier - robotX <= realIRDetectionBarrierLimit) {
			rightIRDetectingWest = getRightIRDetectorValue();
		} else if ((lastDirection > 260 && lastDirection < 280) && northBarrier - robotY <= realIRDetectionBarrierLimit) {
			rightIRDetectingNorth = getRightIRDetectorValue();
		}
	}

	bool closeToSomeKnownBarrier = false;
	if (fabs(robotX - westBarrier) < barrierProximityThreshold || fabs(robotX - eastBarrier) < barrierProximityThreshold || fabs(robotY - northBarrier) < barrierProximityThreshold || fabs(robotY - southBarrier) < barrierProximityThreshold) {
		closeToSomeKnownBarrier = true;
	}
	//Serial.println(robotY);
	//Serial.println(robotX);
	double returnDirection = -1;

	/* if(!closeToSomeKnownBarrier)
	{
		//Serial.println("Not close to known barrier"); 
	} */
	if (!closeToSomeKnownBarrier && getFrontIRDetectorValue()) {
		//Serial.println("Front IR detection, but not close to known barrier");
		returnDirection = -1;
		return returnDirection;
	}

	if (detectionController->checkStoppedAfterButtonPushed()) {
		//Serial.println("Collision button pushed and not moving until 10 seconds button free");
		returnDirection = -1;
		return returnDirection;
	}

	if (gointThruPassage) {
		if (passageCounter >= 25) {
			gointThruPassage = false;
			passageCounter = 0;
			//Serial.println("PassageCounter reset");
		} else {
			passageCounter++;
			//Serial.println("Passagecounter: ");
			//Serial.println(passageCounter);
			returnDirection = lastDirection;
			return returnDirection;
		}
	}
	PassageInfo passageInfo;

	if (mainGoal.getRoom() == robotPosition->getRoom()) {
		roomGoal = mainGoal;
	} else {
		passageInfo = getPassageInfoToNextRoom(currentRoom.getRoomNr(), mainGoal.getRoom());
		roomGoal = passageInfo.roomGoal;
	}
	bool blockedSpaceInFrontOfGoal = checkIfClearDriveToGoalDiagonal(currentRoom, robotX, robotY, roomGoal.getGoalX(), roomGoal.getGoalY());
	//Serial.println(blockedSpaceInFrontOfGoal);

	RoomGoalDirectionInfo roomGoalDirectionInfo = getRoomGoalDirectionInfo(robotX, robotY, roomGoal.getGoalX(), roomGoal.getGoalY());
	//Serial.println(roomGoalDirectionInfo.angleDeg);

	if ((fabs(robotY - roomGoal.getGoalY()) < 0.36) && (fabs(robotX - roomGoal.getGoalX()) < 0.36)) {
		//Serial.println("CLOSE TO GOAL!!");
		delay(500);

		if ((fabs(robotY - roomGoal.getGoalY()) < 0.2) && (fabs(robotX - roomGoal.getGoalX()) < 0.2)) {
			if (roomGoal.getRoom() != mainGoal.getRoom()) {
				//Serial.println("Passage to next room");

				if (fabs(roomGoal.getGoalY() - currentRoom.getYLow()) < 0.2) {
					//Serial.println("Passage south");
					robotPosition->updateRoom(passageInfo.nextRoom);
					returnDirection = 180;
					robotPosition->setLastDirection(returnDirection);
					//lastDirection = returnDirection;
					gointThruPassage = true;
					return returnDirection;
				} else if (fabs(roomGoal.getGoalY() - currentRoom.getYHigh()) < 0.2) {
					//Serial.println("Passage north");
					robotPosition->updateRoom(passageInfo.nextRoom);
					returnDirection = 0;
					robotPosition->setLastDirection(returnDirection);
					//lastDirection = returnDirection;
					gointThruPassage = true;
					return returnDirection;
				} else if (fabs(roomGoal.getGoalX() - currentRoom.getXLow()) < 0.2) {
					//Serial.println("Passage west");
					robotPosition->updateRoom(passageInfo.nextRoom);
					returnDirection = 270;
					robotPosition->setLastDirection(returnDirection);
					//lastDirection = returnDirection;
					gointThruPassage = true;
					return returnDirection;
				} else if (fabs(roomGoal.getGoalX() - currentRoom.getXHigh()) < 0.2) {
					//Serial.println("Passage east");
					robotPosition->updateRoom(passageInfo.nextRoom);
					returnDirection = 90;
					robotPosition->setLastDirection(returnDirection);
					//lastDirection = returnDirection;
					gointThruPassage = true;
					return returnDirection;
				} else if (currentRoom.getRoomNr() == 4) {
					if (roomGoal.getGoalX() > 19 && roomGoal.getGoalX() < 40.5) {
						//Serial.println("Passage north in room 4");
						robotPosition->updateRoom(passageInfo.nextRoom);
						returnDirection = 0;
						robotPosition->setLastDirection(returnDirection);
						gointThruPassage = true;
						return returnDirection;
					}
				}
			} else {
				//Serial.println("REACHED GOAL!!");
				delay(1000);
				return returnDirection;
			}
		}
		//Drive straight to goal
		if (!blockedSpaceInFrontOfGoal) {
			returnDirection = roomGoalDirectionInfo.angleDeg;
			robotPosition->setLastDirection(returnDirection);
			return returnDirection;
		}
		//Reduntand
		if (fabs(robotY - roomGoal.getGoalY()) > fabs(robotX - roomGoal.getGoalX())) {
			if (robotY - roomGoal.getGoalY() < 0) {
				//Serial.println("Goal north!!");
				returnDirection = 0;
				robotPosition->setLastDirection(returnDirection);
				return returnDirection;
			} else {
				//Serial.println("Goal south!!");
				returnDirection = 180;
				robotPosition->setLastDirection(returnDirection);
				return returnDirection;
			}
		} else {
			if (robotX - roomGoal.getGoalX() < 0) {
				//Serial.println("Goal east!!");
				returnDirection = 90;
				robotPosition->setLastDirection(returnDirection);
				return returnDirection;
			} else {
				//Serial.println("Goal west!!");
				returnDirection = 270;
				robotPosition->setLastDirection(returnDirection);
				return returnDirection;
			}
		}
		return returnDirection;
	}
	if (fabs(currentRoom.getXLow() - currentRoom.getXHigh()) <= fabs(currentRoom.getYLow() - currentRoom.getYHigh())) {
		//Serial.println("Room standing rectangle: ");

		if (fabs(robotY - roomGoal.getGoalY()) < diagonalRunningLimit) {
			if (!blockedSpaceInFrontOfGoal) {
				returnDirection = roomGoalDirectionInfo.angleDeg;
				robotPosition->setLastDirection(returnDirection);
				//Serial.println("In if");
				return returnDirection;
			} else {
				if (robotY - southBarrier > 1.4 && northBarrier - robotY > 1.4) {
					returnDirection = lastDirection;
					return returnDirection;
				}
			}
		}
		//Redundant
		if (fabs(robotY - roomGoal.getGoalY()) < 0.05) {
			//Serial.println("Less than 0.3 m difference between robotY and goalY");
			if (robotX - roomGoal.getGoalX() > 0) {  // goal is west
				//Serial.println("Less than 0.05 m difference in y and goal west ");
				returnDirection = 270;
				robotPosition->setLastDirection(returnDirection);
				//lastDirection = returnDirection;
				return returnDirection;
			} else {  // goal is east
				//Serial.println("Less than 0.3 m difference in y and goal east ");
				returnDirection = 90;
				robotPosition->setLastDirection(returnDirection);
				//lastDirection = returnDirection;
				return returnDirection;
			}
		}
		if (robotY < roomGoal.getGoalY()) {  // goal is north
			//Serial.println("More than 0.3 m difference in y and goal North");
			/* 	if (lastDirection < 20 || lastDirection > 340)
			{
				if (frontIRDetectingNorth)
				{
					//Serial.println("SimulatedIRFrontDetecting north barrier < 0.3");
				}
			}
			if (lastDirection > 70 && lastDirection < 110)
			{
				if (frontIRDetectingEast)
				{
					//Serial.println("SimulatedIRFrontDetecting east barrier < 0.3");
				}
			} */
			//if (eastBarrier - robotX < 0.3)

			if (eastBarrier - robotX < barrierProximityThreshold)
			//if (frontIRDetectingEast)
			{
				//Serial.println("Close to east barrier");
				if (northBarrier - robotY < barrierProximityThreshold)
				//Close to north barrier
				{
					// Här bör jag ha koll på om det finns en passage
					if (checkIfPassageInFront(currentRoom, lastDirection, robotX, robotY)) {
						if (northBarrier - robotY <= 0.35 && northWardDirection) {
							//Serial.println("Heading north passage, continue deadwalk");
							returnDirection = 270;
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}
					if (frontIRDetectingNorth) {
						robotPosition->updateYPosition(northBarrier - 0.3);
						//Serial.println("Update position Y");
						returnDirection = 270;
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						returnDirection = lastDirection;
						return returnDirection;
					}
				} else {
					//Serial.println("Not close to north barrier");
					if (checkIfPassageInFront(currentRoom, lastDirection, robotX, robotY)) {
						if (eastBarrier - robotX <= 0.35 && eastWardDirection) {
							//Serial.println("Heading east passage, continue deadwalk");
							returnDirection = 0;
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}
					if (frontIRDetectingEast) {
						robotPosition->updateXPosition(eastBarrier - 0.3);
						//Serial.println("Update position X");
						returnDirection = 0;
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						if (rightIRDetectingEast) {
							returnDirection = 358;
							//Serial.print("rightIRDetectingEast. Direction: ");
							//Serial.println(returnDirection);
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						}
						if (northWardDirection) {
							returnDirection = 2;
							//Serial.print("rightIR not DetectingEast. Direction: ");
							//Serial.println(returnDirection);
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}
				}
			} else {
				//Serial.println("Not close to east barrier");
				if (northBarrier - robotY < barrierProximityThreshold) {
					//Serial.println("Close to north barrier");
					if (checkIfPassageInFront(currentRoom, lastDirection, robotX, robotY)) {
						if (northBarrier - robotY <= 0.35 && northWardDirection) {
							//Serial.println("Heading north passage, continue deadwalk");
							returnDirection = 270;
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}

					if (frontIRDetectingNorth) {
						robotPosition->updateYPosition(northBarrier - 0.3);
						//Serial.println("Update position Y");
						returnDirection = 270;
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						//Serial.println("Close to north barrier but not frontIRDetecting north");
						returnDirection = lastDirection;
						return returnDirection;
					}
				} else {
					//Serial.println("Not close to north barrier");
					if (northBarrierOneStepEast - robotY < barrierProximityThreshold) {
						//Serial.println("Last step from east was close to north barrier");
						returnDirection = 358;
						//Serial.println("North barrier one step east, direction: ");
						//Serial.println(returnDirection);
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						returnDirection = 90;
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					}
				}
			}
		} else {  // goal is south
			//Serial.println("More than 0.3 m difference in y and goal South");
			//Serial.println(lastDirection);
			/* 	if (lastDirection > 220 && lastDirection < 300)
			{
				if (frontIRDetectingWest)
				{
					// simulatedIRFrontDetectingWest = true;
					//Serial.println("SimulatedIRFrontDetecting west barrier < 0.3");
				}
			}
			if (lastDirection > 100 && lastDirection < 190)
			{
				if (frontIRDetectingSouth)
				{
					// simulatedIRFrontDetectingSouth = true;
					//Serial.println("SimulatedIRFrontDetecting south barrier < 0.3");
				}
			}	 */
			if (robotX - westBarrier < barrierProximityThreshold) {
				//Serial.println("Close to west barrier");
				if (robotY - southBarrier < barrierProximityThreshold) {
					if (checkIfPassageInFront(currentRoom, lastDirection, robotX, robotY)) {
						if (robotY - southBarrier <= 0.35 && southWardDirection) {
							//Serial.println("Heading south passage, continue deadwalk");
							returnDirection = 90;
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}
					//Close to south barrier
					if (frontIRDetectingSouth) {
						robotPosition->updateYPosition(southBarrier + 0.3);
						//Serial.println("Update position Y");
						returnDirection = 90;
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						returnDirection = lastDirection;
						return returnDirection;
					}
				} else {
					if (checkIfPassageInFront(currentRoom, lastDirection, robotX, robotY)) {
						if (robotX - westBarrier <= 0.35 && westWardDirection) {
							//Serial.println("Heading passage, continue deadwalk");
							returnDirection = 180;
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}
					if (frontIRDetectingWest) {
						robotPosition->updateXPosition(westBarrier + 0.3);
						//Serial.println("Update position X");
						returnDirection = 180;
						robotPosition->setLastDirection(returnDirection);
						// lastDirection = returnDirection;
						return returnDirection;
					} else {
						if (rightIRDetectingWest) {
							returnDirection = 178;
							//Serial.print("rightIRDetectingWest. Direction: ");
							//Serial.println(returnDirection);
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						}
						if (southWardDirection) {
							returnDirection = 182;
							//Serial.print("rightIR not Detecting west. Direction: ");
							//Serial.println(returnDirection);
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}
				}
			} else {
				//Serial.println("Not close to west barrier");
				if (robotY - southBarrier < barrierProximityThreshold) {
					//Close to south barrier but not close to west

					if (checkIfPassageInFront(currentRoom, lastDirection, robotX, robotY)) {
						if (robotY - southBarrier <= 0.35 && southWardDirection) {
							//Serial.println("Heading south passage, continue deadwalk");
							returnDirection = 90;
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}
					if (frontIRDetectingSouth) {
						robotPosition->updateYPosition(southBarrier + 0.3);
						//Serial.println("Update position Y");
						returnDirection = 90;
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						returnDirection = lastDirection;
						return returnDirection;
					}
					//Serial.println("Close to South barrier");
				} else {
					//Serial.println("Not close to south barrier");
					if (robotY - southBarrierOneStepWest < barrierProximityThreshold) {
						//Serial.println("Last step from west was close to south barrier");
						returnDirection = 178;
						//Serial.println("Southe barrier one step west, direction: ");
						//Serial.println(returnDirection);
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						returnDirection = 270;
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					}
				}
			}
		}
	} else  // Room laying rectangle
	{
		if (fabs(robotX - roomGoal.getGoalX()) < diagonalRunningLimit) {
			if (!blockedSpaceInFrontOfGoal) {
				returnDirection = roomGoalDirectionInfo.angleDeg;
				robotPosition->setLastDirection(returnDirection);
				//Serial.println("In if laying");
				return returnDirection;

			} else {
				if (robotX - westBarrier > 1.4 && eastBarrier - robotX > 1.4) {
					returnDirection = lastDirection;
					return returnDirection;
				}
			}
		}

		//Serial.println("Room laying rectangle: ");
		if (fabs(robotX - roomGoal.getGoalX()) < 0.05)  // Changed
		{
			//Serial.println("Less than 0.05 m difference between robotX and goalX");
			if (robotY - roomGoal.getGoalY() > 0) {
				//Serial.println("Goal is south");
				returnDirection = 180;
				robotPosition->setLastDirection(returnDirection);
				//lastDirection = returnDirection;
				return returnDirection;
			} else {
				//Serial.println("Goal is north ");
				returnDirection = 0;
				robotPosition->setLastDirection(returnDirection);
				//lastDirection = returnDirection;
				return returnDirection;
			}
		}
		if (robotX < roomGoal.getGoalX()) {
			// Goal is east
			/* 	if (lastDirection > 100 && lastDirection < 190)
			{
				if (frontIRDetectingSouth)
				{
					// simulatedIRFrontDetectingSouth = true;
					//Serial.println("SimulatedIRFrontDetecting south barrier < 0.4");
				}
			}
			if (lastDirection > 80 && lastDirection < 100)
			{
				if (frontIRDetectingEast)
				{
					//Serial.println("SimulatedIRFrontDetecting east barrier < 0.4");
				}
			} */
			//Serial.println("More than 0.05 m difference in X and goal is east");
			if (robotY - southBarrier < barrierProximityThreshold)
			//if (frontIRDetectingSouth)
			{

				//Serial.println("Close to south barrier");
				if (eastBarrier - robotX < barrierProximityThreshold)
				//if (frontIRDetectingEast)
				{
					if (checkIfPassageInFront(currentRoom, lastDirection, robotX, robotY)) {
						if (eastBarrier - robotX <= 0.35 && eastWardDirection) {
							//Serial.println("Heading east passage, continue deadwalk");
							returnDirection = 0;
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}
					//Close to east barrier
					if (frontIRDetectingEast) {
						robotPosition->updateXPosition(eastBarrier - 0.3);
						//Serial.println("Update position X");
						returnDirection = 0;
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						returnDirection = lastDirection;
						return returnDirection;
					}
					//Serial.println("Close to east barrier");
				} else {
					if (checkIfPassageInFront(currentRoom, lastDirection, robotX, robotY)) {
						if (robotY - southBarrier <= 0.35 && southWardDirection) {
							//Serial.println("Heading south passage, continue deadwalk");
							returnDirection = 90;
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}
					if (frontIRDetectingSouth) {
						robotPosition->updateYPosition(southBarrier + 0.3);
						//Serial.println("Update position Y");
						returnDirection = 90;
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						if (rightIRDetectingSouth) {
							returnDirection = 88;
							//Serial.print("rightIRDetecting south. Direction: ");
							//Serial.println(returnDirection);
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						}
						if (eastWardDirection) {
							returnDirection = 92;
							//Serial.print("rightIR not Detecting south. Direction: ");
							//Serial.println(returnDirection);
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
						returnDirection = lastDirection;
						return returnDirection;
					}
					//Serial.println("Not close to east barrier");
				}
			} else {
				//Serial.println("Not close to south barrier");
				if (eastBarrier - robotX < barrierProximityThreshold) {
					if (checkIfPassageInFront(currentRoom, lastDirection, robotX, robotY)) {
						if (eastBarrier - robotX <= 0.35 && eastWardDirection) {
							//Serial.println("Heading east passage, continue deadwalk");
							returnDirection = 0;
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}
					//Close to east barrier
					if (frontIRDetectingEast) {
						robotPosition->updateXPosition(eastBarrier - 0.3);
						//Serial.println("Update position X");
						returnDirection = 0;
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						returnDirection = lastDirection;
						return returnDirection;
					}
					//Serial.println("Close to east barrier");
				} else {
					//Serial.println("Not close to east barrier");
					if (eastBarrierOneStepSouth - robotX < barrierProximityThreshold) {
						//Serial.println("Last step from south was close to east barrier");
						returnDirection = 88;
						//Serial.print("East barrier one step south, direction: ");
						//Serial.println(returnDirection);
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						returnDirection = 180;
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					}
				}
			}
		} else {
			//Goal is west
			/* 	if (lastDirection < 10)
			{
				if (frontIRDetectingNorth)
				{
					//Serial.println("SimulatedIRFrontDetecting north barrier < 0.4");
				}
			}

			if (lastDirection > 220 && lastDirection < 300)
			{
				if (frontIRDetectingWest)
				{
					// simulatedIRFrontDetectingWest = true;
					//Serial.println("SimulatedIRFrontDetecting west barrier < 0.4");
				}
			} */
			//Serial.println("More than 0.3 m difference in X and goal is west");
			if (northBarrier - robotY < barrierProximityThreshold) {
				//Serial.println("Close to north barrier");
				if (robotX - westBarrier < barrierProximityThreshold) {
					//Serial.println("Close to west barrier");
					if (checkIfPassageInFront(currentRoom, lastDirection, robotX, robotY)) {
						if (robotX - westBarrier <= 0.35 && westWardDirection) {
							//Serial.println("Heading west passage, continue deadwalk");
							returnDirection = 180;
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}

					if (frontIRDetectingWest) {
						robotPosition->updateXPosition(westBarrier + 0.3);
						//Serial.println("Update position X");
						returnDirection = 180;
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						returnDirection = lastDirection;
						return returnDirection;
					}
				} else {
					//Serial.println("Not close to west barrier");
					//Serial.println("Laying rectangle, goal is west, not close to west barrier, within barrier threshold north");
					if (checkIfPassageInFront(currentRoom, lastDirection, robotX, robotY)) {
						if (northBarrier - robotY <= 0.35 && northWardDirection) {
							//Serial.println("Heading north passage, continue deadwalk");
							returnDirection = 270;
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}

					if (frontIRDetectingNorth) {
						robotPosition->updateYPosition(northBarrier - 0.3);
						//Serial.println("Update position Y");
						returnDirection = 270;
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						if (rightIRDetectingNorth) {
							returnDirection = 268;
							//Serial.print("rightIRDetecting North. Direction: ");
							//Serial.println(returnDirection);
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						}
						if (westWardDirection) {
							returnDirection = 272;
							//Serial.print("rightIR not Detectig north. Direction: ");
							//Serial.println(returnDirection);
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							//Serial.println(returnDirection);
							return returnDirection;
						}
					}
				}
			} else {
				//Serial.println("Not close to north barrier");
				if (robotX - westBarrier < barrierProximityThreshold)
				//if (frontIRDetectingWest)
				{
					if (checkIfPassageInFront(currentRoom, lastDirection, robotX, robotY)) {
						if (robotX - westBarrier <= 0.35 && westWardDirection) {
							//Serial.println("Heading west passage, continue deadwalk");
							returnDirection = 180;
							robotPosition->setLastDirection(returnDirection);
							return returnDirection;
						} else {
							returnDirection = lastDirection;
							return returnDirection;
						}
					}
					if (frontIRDetectingWest) {
						robotPosition->updateXPosition(westBarrier + 0.3);
						//Serial.println("Update position X");
						returnDirection = 180;
						robotPosition->setLastDirection(returnDirection);
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					} else {
						returnDirection = lastDirection;
						return returnDirection;
					}
				} else {
					//Serial.println("Not close to west barrier");
					if (robotX - westBarrierOneStepNorth < barrierProximityThreshold) {
						//Serial.println("Last step from north was close to west barrier");
						returnDirection = 268;
						//Serial.print("West barrier one step north, direction: ");
						//Serial.println(returnDirection);
						robotPosition->setLastDirection(returnDirection);
						//lastDirection = returnDirection;
						return returnDirection;
					}
					returnDirection = 0;
					robotPosition->setLastDirection(returnDirection);
					//lastDirection = returnDirection;
					return returnDirection;
				}
			}
		}
	}
	return returnDirection;
}

double Navigation::getDimensionCoordinateBarrierInFront(Room currentRoom, char direction, double robotX, double robotY, std::vector<BlockedSpaceBlock> blockedSpacesInRoom) {

	double barrierCoord = -1;
	int blockOrWallInFront = checkIfBlockOrWallInFront(direction, robotX, robotY, blockedSpacesInRoom);
	//Serial.println("If following 1 then blocke else wall ");
	//Serial.println(blockOrWallInFront);

	if (blockOrWallInFront == 1) {
		barrierCoord = getDimensionCoordinateBlockedSpaceInFront(direction, robotX, robotY, blockedSpacesInRoom);
	} else {
		switch (direction) {
			case 'n':
				barrierCoord = currentRoom.getYHigh();
				break;
			case 's':
				barrierCoord = currentRoom.getYLow();
				break;
			case 'w':
				barrierCoord = currentRoom.getXLow();
				break;
			case 'e':
				barrierCoord = currentRoom.getXHigh();
				break;
		}
	}
	return barrierCoord;
}

double Navigation::getDimensionCoordinateBlockedSpaceInFront(char direction, double robotX, double robotY, std::vector<BlockedSpaceBlock> blockedSpacesInRoom) {
	std::vector<BlockedSpaceBlock> blockedSpacesInDirectionFound;

	if (direction == 'n') {
		for (int i = 0; i < blockedSpacesInRoom.size(); i++) {
			if ((blockedSpacesInRoom[i].getXLow() <= robotX) && blockedSpacesInRoom[i].getXHigh() >= robotX && blockedSpacesInRoom[i].getYLow() > robotY) {
				blockedSpacesInDirectionFound.push_back(blockedSpacesInRoom[i]);
			}
		}
	}
	if (direction == 's') {
		for (int i = 0; i < blockedSpacesInRoom.size(); i++) {
			if ((blockedSpacesInRoom[i].getXLow() <= robotX) && blockedSpacesInRoom[i].getXHigh() >= robotX && blockedSpacesInRoom[i].getYHigh() < robotY) {
				blockedSpacesInDirectionFound.push_back(blockedSpacesInRoom[i]);
			}
		}
	}
	if (direction == 'w') {
		for (int i = 0; i < blockedSpacesInRoom.size(); i++) {
			if ((blockedSpacesInRoom[i].getYLow() <= robotY) && blockedSpacesInRoom[i].getYHigh() >= robotY && blockedSpacesInRoom[i].getXHigh() < robotX) {
				blockedSpacesInDirectionFound.push_back(blockedSpacesInRoom[i]);
			}
		}
	}
	if (direction == 'e') {
		for (int i = 0; i < blockedSpacesInRoom.size(); i++) {
			if ((blockedSpacesInRoom[i].getYLow() <= robotY) && blockedSpacesInRoom[i].getYHigh() >= robotY && blockedSpacesInRoom[i].getXLow() > robotX) {
				blockedSpacesInDirectionFound.push_back(blockedSpacesInRoom[i]);
			}
		}
	}

	double closestCoordinate = 1000;
	switch (direction) {

		case 'n':
			for (int i = 0; i < blockedSpacesInDirectionFound.size(); i++) {
				if ((blockedSpacesInDirectionFound[i].getYLow() < closestCoordinate) || (closestCoordinate == 1000)) {
					closestCoordinate = blockedSpacesInDirectionFound[i].getYLow();
				}
			}
			break;
		case 's':
			for (int i = 0; i < blockedSpacesInDirectionFound.size(); i++) {
				if ((blockedSpacesInDirectionFound[i].getYHigh() > closestCoordinate) || (closestCoordinate == 1000)) {
					closestCoordinate = blockedSpacesInDirectionFound[i].getYHigh();
				}
			}
			break;
		case 'w':
			for (int i = 0; i < blockedSpacesInDirectionFound.size(); i++) {
				if ((blockedSpacesInDirectionFound[i].getXHigh() > closestCoordinate) || (closestCoordinate == 1000)) {
					closestCoordinate = blockedSpacesInDirectionFound[i].getXHigh();
				}
			}
			break;
		case 'e':
			for (int i = 0; i < blockedSpacesInDirectionFound.size(); i++) {
				if ((blockedSpacesInDirectionFound[i].getXLow() < closestCoordinate) || (closestCoordinate == 1000)) {
					closestCoordinate = blockedSpacesInDirectionFound[i].getXLow();
				}
			}
			break;
	}
	return closestCoordinate;
}

int Navigation::checkIfBlockOrWallInFront(char direction, double robotX, double robotY, std::vector<BlockedSpaceBlock> blockedSpaces) {
	if (blockedSpaces.empty()) {
		return 0;  // wall in direction
	}
	if (direction == 'n') {
		for (int i = 0; i < blockedSpaces.size(); i++) {
			if ((blockedSpaces[i].getXLow() <= robotX) && blockedSpaces[i].getXHigh() >= robotX && blockedSpaces[i].getYLow() > robotY) {
				return 1;
			}
		}
	}
	if (direction == 's') {
		for (int i = 0; i < blockedSpaces.size(); i++) {
			if ((blockedSpaces[i].getXLow() <= robotX) && blockedSpaces[i].getXHigh() >= robotX && blockedSpaces[i].getYHigh() < robotY) {
				return 1;
			}
		}
	}
	if (direction == 'w') {
		for (int i = 0; i < blockedSpaces.size(); i++) {
			if ((blockedSpaces[i].getYLow() <= robotY) && blockedSpaces[i].getYHigh() >= robotY && blockedSpaces[i].getXHigh() < robotX) {
				return 1;
			}
		}
	}
	if (direction == 'e') {
		for (int i = 0; i < blockedSpaces.size(); i++) {
			if ((blockedSpaces[i].getYLow() <= robotY) && blockedSpaces[i].getYHigh() >= robotY && blockedSpaces[i].getXLow() > robotX) {
				return 1;
			}
		}
	}
	return 0;
}

std::vector<BlockedSpaceBlock> Navigation::getBlockedSpacesWall(char wallDirection) {
	Room room = roomController->getRoom(robotPosition->getRoom());
	std::vector<BlockedSpaceBlock> blockeSpaces = room.getBlockedSpaceBlocks();
	std::vector<BlockedSpaceBlock> result;
	switch (wallDirection) {
		case 'n':
			for (int i = 0; i < blockeSpaces.size(); i++) {
				if (fabs(blockeSpaces[i].getYHigh() - room.getYHigh()) < 0.5) {
					result.push_back(blockeSpaces[i]);
				}
			}
			break;
		case 's':
			for (int i = 0; i < blockeSpaces.size(); i++) {
				if (fabs(blockeSpaces[i].getYLow() - room.getYLow()) < 0.5) {
					result.push_back(blockeSpaces[i]);
				}
			}
			break;
		case 'w':
			for (int i = 0; i < blockeSpaces.size(); i++) {
				if (fabs(blockeSpaces[i].getXLow() - room.getXLow()) < 0.5) {
					result.push_back(blockeSpaces[i]);
				}
			}
			break;
		case 'e':
			for (int i = 0; i < blockeSpaces.size(); i++) {
				if (fabs(blockeSpaces[i].getXHigh() - room.getXHigh()) < 0.5) {
					result.push_back(blockeSpaces[i]);
				}
			}
	}
	return result;
}

double Navigation::getDirectDirectionToGoal() {
	int direction = 0;
	if (fabs(mainGoal.getGoalX() - robotPosition->getXPosition()) <= fabs(mainGoal.getGoalY() - robotPosition->getYPosition()))  // Larger or same difference in Y
	{
		if (robotPosition->getYPosition() - mainGoal.getGoalY() > 0)  // Goal is south
		{
			direction = 180;
		} else  // Goal is north
		{
			direction = 0;
		}
	} else  // Larger difference in X
	{
		if (robotPosition->getXPosition() - mainGoal.getGoalX() > 0)  // Goal is west
		{
			direction = 270;
		} else {
			direction = 90;
		}
	}
	return direction;
}

bool Navigation::checkIfClearDriveToGoalDiagonal(Room currentRoom, double ownX, double ownY, double goalX, double goalY) {
	bool blockedSpaceInPath = false;
	std::vector<BlockedSpaceBlock> blockedSpacesInRoom = currentRoom.getBlockedSpaceBlocks();

	for (int i = 0; i < blockedSpacesInRoom.size(); i++) {
		double blockedSpaceXLow = blockedSpacesInRoom[i].getXLow();
		double blockedSpaceXHigh = blockedSpacesInRoom[i].getXHigh();
		double blockedSpaceYLow = blockedSpacesInRoom[i].getYLow();
		double blockedSpaceYHigh = blockedSpacesInRoom[i].getYHigh();

		point robotLocation;
		robotLocation.x = ownX;
		robotLocation.y = ownY;

		point goalLocation;
		goalLocation.x = goalX;
		goalLocation.y = goalY;

		point southWestPoint;
		southWestPoint.x = blockedSpaceXLow;
		southWestPoint.y = blockedSpaceYLow;

		point northWestPoint;
		northWestPoint.x = blockedSpaceXLow;
		northWestPoint.y = blockedSpaceYHigh;

		point southEastPoint;
		southEastPoint.x = blockedSpaceXHigh;
		southEastPoint.y = blockedSpaceYLow;

		point northEastPoint;
		northEastPoint.x = blockedSpaceXHigh;
		northEastPoint.y = blockedSpaceYHigh;

		// If line between robot and goal cross edge of blockedSpaceBlock
		bool westVertical = checkIfLinesCross(robotLocation, goalLocation, southWestPoint, northWestPoint);
		bool eastVertical = checkIfLinesCross(robotLocation, goalLocation, southEastPoint, northEastPoint);
		bool northHorizontal = checkIfLinesCross(robotLocation, goalLocation, northWestPoint, northEastPoint);
		bool southHorizontal = checkIfLinesCross(robotLocation, goalLocation, southWestPoint, southEastPoint);

		if (westVertical || eastVertical || northHorizontal || southHorizontal) {
			blockedSpaceInPath = true;
			return blockedSpaceInPath;
		}
	}
	return blockedSpaceInPath;
}

bool Navigation::checkIfLinesCross(point firstLineStart, point firstLineEnd, point secondLineStart, point secondLineEnd) {
	bool linesCrossing = false;
	bool endsOfSecondLineBothSideOfFirstLine = false;
	bool endsOfFirstLineBothSideOfSecondLine = false;

	NavigationVector fLineStToEnV;  //AB
	fLineStToEnV.x = firstLineEnd.x - firstLineStart.x;
	fLineStToEnV.y = firstLineEnd.y - firstLineStart.y;

	NavigationVector fLineStToSLineStV;  //AC
	fLineStToSLineStV.x = secondLineStart.x - firstLineStart.x;
	fLineStToSLineStV.y = secondLineStart.y - firstLineStart.y;

	NavigationVector fLineStToSLineEV;  //AD
	fLineStToSLineEV.x = secondLineEnd.x - firstLineStart.x;
	fLineStToSLineEV.y = secondLineEnd.y - firstLineStart.y;

	NavigationVector sLineStToEV;  //CD
	sLineStToEV.x = secondLineEnd.x - secondLineStart.x;
	sLineStToEV.y = secondLineEnd.y - secondLineStart.y;

	NavigationVector sLineStToFLineSV;  //CA
	sLineStToFLineSV.x = firstLineStart.x - secondLineStart.x;
	sLineStToFLineSV.y = firstLineStart.y - secondLineStart.y;

	NavigationVector sLineStToFLineEV;  //CB
	sLineStToFLineEV.x = firstLineEnd.x - secondLineStart.x;
	sLineStToFLineEV.y = firstLineEnd.y - secondLineStart.y;

	//AB X AC
	double firstVecktorProduct = (fLineStToEnV.x * fLineStToSLineStV.y) - (fLineStToEnV.y * fLineStToSLineStV.x);
	//AB X AD
	double secondVectorProduct = (fLineStToEnV.x * fLineStToSLineEV.y) - (fLineStToEnV.y * fLineStToSLineEV.x);
	//CD X CA
	double thirdVectorProduct = (sLineStToEV.x * sLineStToFLineSV.y) - (sLineStToEV.y * sLineStToFLineSV.x);
	//CD X CB
	double fourthVectorProduct = (sLineStToEV.x * sLineStToFLineEV.y) - (sLineStToEV.y * sLineStToFLineEV.x);


	if ((firstVecktorProduct > 0 && secondVectorProduct < 0) || (firstVecktorProduct < 0 && secondVectorProduct > 0)) {
		endsOfSecondLineBothSideOfFirstLine = true;
	}
	if ((thirdVectorProduct > 0 && fourthVectorProduct < 0) || (thirdVectorProduct < 0 && fourthVectorProduct > 0)) {
		endsOfFirstLineBothSideOfSecondLine = true;
	}

	if (endsOfSecondLineBothSideOfFirstLine && endsOfFirstLineBothSideOfSecondLine) {
		linesCrossing = true;
	}
	return linesCrossing;
}
