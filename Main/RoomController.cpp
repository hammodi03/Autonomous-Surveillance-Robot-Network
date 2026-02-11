#include "RoomController.h"
#include <Arduino.h>

RoomController::RoomController() {
}

void RoomController::initRooms() {

  /////////####### NEXT VALUE OBJECT NUMBER :

  //------------- Room 1 ----------------------
  Room room1(4.5, 11, 47, 67.5, 1);
  BlockedSpaceBlock block1(4.5, 7.2, 47, 48.5, 1);
  room1.addBlockedSpaceBlock(block1);
  BlockedSpaceBlock block2(8, 11, 47, 49, 1);
  room1.addBlockedSpaceBlock(block2);
  Passage passage1(0, 7, 8, 47, 1, 2);
  room1.addPassage(passage1);
  ValuableObject valuableObject1Room1(7.5, 7.5, 66, 66, 1, "Obelisk", 1);
  BlockedSpaceBlock block201(6, 7.5, 65.2, 66.8, 1);
  ValuableObject valuableObject2Room1(6.5, 6.5, 55, 55, 1, "AndreasTheMighty", 2);
  BlockedSpaceBlock block202(7, 8.5, 54.7, 55.8, 1);
  room1.addValuableObject(valuableObject1Room1);
  room1.addBlockedSpaceBlock(block201);
  room1.addValuableObject(valuableObject2Room1);
  room1.addBlockedSpaceBlock(block202);
  addRoom(room1);

  //printInformationAboutRoom(1);

  //------------- Room 2 ----------------------
  Room room2(3.5, 11, 42, 47, 2);
  BlockedSpaceBlock block3(3.5, 4.5, 42, 45.5, 2);
  room2.addBlockedSpaceBlock(block3);
  BlockedSpaceBlock block4(4.5, 7.2, 42, 44, 2);
  room2.addBlockedSpaceBlock(block4);
  BlockedSpaceBlock block5(8, 11, 42, 44, 2);
  room2.addBlockedSpaceBlock(block5);
  room2.addPassage(passage1);
  Passage passage2(0, 7, 8, 42, 2, 3);
  room2.addPassage(passage2);
  addRoom(room2);

  //printInformationAboutRoom(2);

  //------------- Room 3 ----------------------
  Room room3(4.5, 17, 19, 42, 3);
  BlockedSpaceBlock block37(4.5, 6.5, 19, 22.3, 3);
  room3.addBlockedSpaceBlock(block37);
  BlockedSpaceBlock block38(7.6, 17, 19, 22.3, 3);
  room3.addBlockedSpaceBlock(block38);
  BlockedSpaceBlock block39(15.5, 17, 19, 26.8, 3);
  room3.addBlockedSpaceBlock(block39);
  BlockedSpaceBlock block391(15.2, 17, 28, 42, 3);
  room3.addBlockedSpaceBlock(block391);
  //Passage passage17(0, 15.5, 17, 45, 3, 2);
  room3.addPassage(passage2);
  Passage passage18(1, 26.8, 28, 17, 3, 4);
  room3.addPassage(passage18);
  Passage passage19(0, 6.5, 7.6, 19, 3, 11);
  room3.addPassage(passage19);
  ValuableObject valuableObject1Room3(5.5, 5.5, 36, 36, 3, "Herkules", 1);
  BlockedSpaceBlock block3911(6, 7.5, 35.5, 36.6, 3);
  ValuableObject valuableObject2Room3(6.5, 6.5, 25, 25, 3, "Odysseys", 2);
  BlockedSpaceBlock block3912(7, 8.5, 24.7, 25.8, 3);
  room3.addValuableObject(valuableObject1Room3);
  room3.addBlockedSpaceBlock(block3911);
  room3.addValuableObject(valuableObject2Room3);
  room3.addBlockedSpaceBlock(block3912);

  ValuableObject comZone(7.3, 7.3, 22, 22, 3, "comZoneRoom3", 3);
  //comZone.setComZone(true);
  //room3.addValuableObject(comZone);

  addRoom(room3);
  //printInformationAboutRoom(3);

  //------------- Room 4 ----------------------

  Room room4(17, 42.5, 20.8, 29, 4);
  BlockedSpaceBlock block6(19.5, 42, 23.5, 29, 4);
  room4.addBlockedSpaceBlock(block6);
  BlockedSpaceBlock block7(41, 42.5, 20.8, 22.5, 4);
  room4.addBlockedSpaceBlock(block7);
  //Passage passage4(1,26.5,28.5,17,4,3);
  room4.addPassage(passage18);
  Passage passage5(0, 21, 23.5, 23.5, 4, 5);
  room4.addPassage(passage5);
  Passage passage6(0, 24.5, 26, 23.5, 4, 6);
  room4.addPassage(passage6);
  Passage passage7(0, 30, 32, 23.5, 4, 7);
  room4.addPassage(passage7);
  Passage passage8(0, 35, 36.5, 23.5, 4, 8);
  room4.addPassage(passage8);
  Passage passage9(0, 37, 39, 23.5, 4, 9);
  room4.addPassage(passage9);
  Passage passage10(1, 23.5, 25, 42.5, 4, 10);
  room4.addPassage(passage10);
  addRoom(room4);

  //printInformationAboutRoom(4);

  //------------- Room 5 ----------------------

  Room room5(20.3, 24, 23.5, 29, 5);
  BlockedSpaceBlock block15(20.3, 21.3, 23.5, 24.2, 5);
  room5.addBlockedSpaceBlock(block15);
  BlockedSpaceBlock block16(23.5, 24, 23.5, 24.5, 5);
  room5.addBlockedSpaceBlock(block16);
  //BlockedSpaceBlock block17(19.5,20,23.5,29, 5);
  //room5.addBlockedSpaceBlock(block17);
  //BlockedSpaceBlock block18(24,24.5,23.5,29, 5);
  //room5.addBlockedSpaceBlock(block18);
  Passage passage11(0, 21.3, 23.5, 23.5, 5, 4);
  room5.addPassage(passage11);
  ValuableObject valuableObject1Room5(22.5, 22.5, 27, 27, 5, "Philippoo", 1);
  BlockedSpaceBlock block161(21, 22.5, 26.2, 29, 5);
  room5.addValuableObject(valuableObject1Room5);
  room5.addBlockedSpaceBlock(block161);
  addRoom(room5);

  //------------- Room 6 ----------------------
  Room room6(24.5, 28.3, 23.5, 29, 6);
  BlockedSpaceBlock block19(26, 28.5, 23.5, 24.5, 6);
  room6.addBlockedSpaceBlock(block19);
  //BlockedSpaceBlock block20(24,24.5,23.5,30, 6);
  //room6.addBlockedSpaceBlock(block20);
  //BlockedSpaceBlock block21(28,28.5,23.5,30, 6);
  //room6.addBlockedSpaceBlock(block21);
  Passage passage12(0, 24.5, 26, 23.5, 6, 4);
  room6.addPassage(passage12);
  ValuableObject valuableObject1Room6(26, 26.5, 26.5, 27, 6, "ZoneRoom6", 1);
  //BlockedSpaceBlock block191(25, 27.5, 25.5, 28, 6);
  valuableObject1Room6.setZone(true);
  room6.addValuableObject(valuableObject1Room6);
  //room6.addBlockedSpaceBlock(block191);
  addRoom(room6);

  //printInformationAboutRoom(6);
  //------------- Room 7 ----------------------
  Room room7(28.7, 32.8, 23.5, 29, 7);
  BlockedSpaceBlock block22(28.7, 30.3, 23.5, 24.2, 7);
  room7.addBlockedSpaceBlock(block22);
  BlockedSpaceBlock block23(31.7, 32.8, 23.5, 24.5, 7);
  room7.addBlockedSpaceBlock(block23);
  //BlockedSpaceBlock block24(28,28.5,23.5,30, 7);
  //room7.addBlockedSpaceBlock(block24);
  //BlockedSpaceBlock block25(33,33.5,23.5,30, 7);
  //room7.addBlockedSpaceBlock(block25);
  Passage passage13(0, 30.3, 31.7, 23.5, 7, 4);
  room7.addPassage(passage13);
  ValuableObject valuableObject1Room7(29.5, 30, 26.5, 27, 7, "ZoneRoom7", 1);
  //BlockedSpaceBlock block231(29.5, 30, 26.5, 27, 7);
  valuableObject1Room7.setZone(true);
  room7.addValuableObject(valuableObject1Room7);
  //room7.addBlockedSpaceBlock(block231);
  addRoom(room7);
  //printInformationAboutRoom(7);
  //------------- Room 8 ----------------------
  Room room8(33.8, 37, 23.5, 29, 8);
  BlockedSpaceBlock block26(33.8, 35.5, 23.5, 24.2, 8);
  room8.addBlockedSpaceBlock(block26);
  BlockedSpaceBlock block27(36.5, 37, 23.5, 24.2, 8);
  room8.addBlockedSpaceBlock(block27);
  // BlockedSpaceBlock block28(33,33.5,23.5,30, 8);
  //room8.addBlockedSpaceBlock(block28);
  //BlockedSpaceBlock block29(36,37,23.5,30, 8);
  //room8.addBlockedSpaceBlock(block29);
  Passage passage14(0, 35.5, 36.5, 23.5, 8, 4);  // RUM 8 -> RUM 4
  //Passage passage8(0, 35, 37.5, 23.5, 4, 8); // RUM 4 -> RUM 8
  room8.addPassage(passage14);
  ValuableObject valuableObject1Room8(34.5, 35, 26.5, 27, 8, "ZoneRoom8", 1);
  //BlockedSpaceBlock block271(34.5, 35, 26.5, 27, 8);
  valuableObject1Room8.setZone(true);
  room8.addValuableObject(valuableObject1Room8);
  //room8.addBlockedSpaceBlock(block271);
  addRoom(room8);
  //printInformationAboutRoom(8);
  // returnGoal.setGoalX(35.8);
  // returnGoal.setGoalY(23.5);
  // passageInfo.nextRoom = 4;

  //------------- Room 9 ---------------------
  Room room9(37.3, 42.5, 23.5, 29, 9);
  BlockedSpaceBlock block30(39.5, 42, 23.5, 24.5, 9);
  room9.addBlockedSpaceBlock(block30);
  //BlockedSpaceBlock block31(36.5,37,23.5,30, 9);
  //room9.addBlockedSpaceBlock(block31);
  BlockedSpaceBlock block32(42, 42.5, 23.5, 29, 9);
  room9.addBlockedSpaceBlock(block32);
  Passage passage15(0, 38, 39.5, 23.5, 9, 4);
  room9.addPassage(passage15);
  ValuableObject valuableObject1Room9(40.5, 40.5, 27, 27, 9, "MonaLisa", 1);
  BlockedSpaceBlock block321(39, 40.5, 26.2, 27.3, 9);
  room9.addValuableObject(valuableObject1Room9);
  room9.addBlockedSpaceBlock(block321);
  addRoom(room9);
  //printInformationAboutRoom(9);

  //------------- Room 10 ----------------------
  Room room10(42.5, 45, 20.8, 29, 10);
  //BlockedSpaceBlock block33(42,42.5,25,30, 10);
  //room10.addBlockedSpaceBlock(block33);
  //BlockedSpaceBlock block34(45,45.5,22,30, 10);
  //room10.addBlockedSpaceBlock(block34);
  //BlockedSpaceBlock block35(42,42.5,22,23.5, 10);
  //room10.addBlockedSpaceBlock(block35);
  //BlockedSpaceBlock block36(42,45,20.5,22, 10);
  //room10.addBlockedSpaceBlock(block36);
  Passage passage16(1, 22.5, 23.5, 42.5, 10, 4);
  room10.addPassage(passage16);
  addRoom(room10);
  //printInformationAboutRoom(10);

  //Passages
  //Passage(int passageDirection 0 = horizontell 1 = vertikal, double passageCoordLow, double passageCoordHigh, double crossCoord, int room1, int room2);
  Passage passage11To3(0, 6, 8, 19.5, 3, 11);
  Passage passage11To12(1, 15, 16.2, 50.8, 11, 12);
  Passage passage12To16(0, 54.5, 56, 9.8, 16, 12);
  Passage passage12To13(0, 54.5, 57, 19.8, 13, 12);
  Passage passage14To13(0, 47.3, 51.5, 30.2, 14, 13);
  Passage passage16To15Top(1, 7.7, 8.7, 49.8, 15, 16);
  Passage passage16To15Mid(1, 6.5, 8, 49.8, 15, 16);
  Passage passage16To15Bottom(1, 1.7, 2.8, 49.8, 15, 16);

  //------------- Room 11 ---------------------
  Room room11(1.5, 50.8, 12.5, 19, 11);                          //YLOW = 14,5?? Beror på om man räknar med den stora biten under eller ej
  BlockedSpaceBlock room11BlockTopLeft(1.5, 2.5, 19, 20.5, 11);  //Uppe i vänstra hörnet
  room11.addBlockedSpaceBlock(room11BlockTopLeft);
  BlockedSpaceBlock room11BlockBottomLeft(1.5, 2.5, 12.5, 15.5, 11);
  room11.addBlockedSpaceBlock(room11BlockBottomLeft);
  BlockedSpaceBlock room11BlockBottomWindows(2.5, 49.5, 10, 12.5, 11);
  room11.addBlockedSpaceBlock(room11BlockBottomWindows);
  BlockedSpaceBlock room11BlockedRightTopWall(49.8, 50.8, 16.2, 19, 11);
  room11.addBlockedSpaceBlock(room11BlockedRightTopWall);
  BlockedSpaceBlock room11BlockedRightBottomWall(49.8, 50.8, 12.5, 15, 11);
  room11.addBlockedSpaceBlock(room11BlockedRightBottomWall);
  room11.addPassage(passage11To3);
  room11.addPassage(passage11To12);

  ValuableObject valuableObject1Room11(29.5, 30, 17, 17, 11, "Sindino", 1);
  BlockedSpaceBlock block322(29.2, 30.3, 15, 16.5, 11);
  room11.addValuableObject(valuableObject1Room11);
  room11.addBlockedSpaceBlock(block322);
  ValuableObject valuableObject2Room11(3.5, 4, 17, 17, 11, "Fredriko", 2);
  BlockedSpaceBlock block323(3.5, 4.8, 15, 16.5, 11);
  room11.addValuableObject(valuableObject2Room11);
  room11.addBlockedSpaceBlock(block323);

  //ValuableObject comZoneRoom11(7.3, 7.3, 17, 17, 11, "comZoneRoom11", 3);
  //comZone.setComZone(true);
  //room11.addValuableObject(comZoneRoom11);

  addRoom(room11);

  //------------- Room 12 ---------------------
  Room room12(50.8, 62, 9.8, 19.8, 12);
  BlockedSpaceBlock room12BlockBottomLeft1(50.8, 55.2, 9.8, 10.8, 12);
  room12.addBlockedSpaceBlock(room12BlockBottomLeft1);
  BlockedSpaceBlock room12BlockedBottomLeft2(50.8, 54.8, 10.8, 11.5, 12);
  room12.addBlockedSpaceBlock(room12BlockedBottomLeft2);
  BlockedSpaceBlock room12BlockedBottomRight1(56.8, 59, 10.8, 11.8, 12);
  room12.addBlockedSpaceBlock(room12BlockedBottomRight1);
  BlockedSpaceBlock room12BlockedBottomRight2(56, 59, 9.8, 10.8, 12);
  room12.addBlockedSpaceBlock(room12BlockedBottomRight2);
  BlockedSpaceBlock room12BlockedRightSideWall(59, 62, 9.8, 19.8, 12);
  room12.addBlockedSpaceBlock(room12BlockedRightSideWall);
  room12.addPassage(passage11To12);
  room12.addPassage(passage12To16);
  room12.addPassage(passage12To13);
  ValuableObject valuableObject1Room12(58.5, 58.5, 15.5, 16, 12, "Magnuso", 1);
  BlockedSpaceBlock block324(57.2, 58.3, 15, 16.5, 12);
  room12.addValuableObject(valuableObject1Room12);
  room12.addBlockedSpaceBlock(block324);
  addRoom(room12);

  //------------- Room 13 ---------------------
  Room room13(45.5, 58.5, 19.8, 30.2, 13);
  BlockedSpaceBlock room13BlockBottom(45.5, 55.2, 19.8, 21.2, 13);
  room13.addBlockedSpaceBlock(room13BlockBottom);
  BlockedSpaceBlock room13BlockBottomMid(50, 55.2, 21.2, 22.2, 13);
  room13.addBlockedSpaceBlock(room13BlockBottomMid);
  BlockedSpaceBlock room13BlockRightWall(56, 58.5, 23, 30.2, 13);
  room13.addBlockedSpaceBlock(room13BlockRightWall);
  BlockedSpaceBlock room13BlockPassageRight(56, 56.5, 19.8, 23, 13);
  room13.addBlockedSpaceBlock(room13BlockPassageRight);
  room13.addPassage(passage12To13);
  room13.addPassage(passage14To13);
  ValuableObject valuableObject1Room13(48.5, 48.5, 24, 24, 13, "Andersani", 1);
  BlockedSpaceBlock block325(49.2, 50.3, 23.2, 24, 13);
  room13.addValuableObject(valuableObject1Room13);
  room13.addBlockedSpaceBlock(block325);
  addRoom(room13);

  //------------- Room 14 ---------------------
  Room room14(45.7, 54.5, 30.2, 42.5, 14);
  BlockedSpaceBlock room14BlockRightWall(52.5, 54.5, 30.2, 42.5, 14);
  room14.addBlockedSpaceBlock(room14BlockRightWall);
  BlockedSpaceBlock room14BlockPassageLeftSide(45.7, 47.3, 30.2, 31.1, 14);
  room14.addBlockedSpaceBlock(room14BlockPassageLeftSide);
  BlockedSpaceBlock room14BlockPassageRightSide(51.5, 52.5, 30.2, 31.3, 14);
  room14.addBlockedSpaceBlock(room14BlockPassageRightSide);
  room14.addPassage(passage14To13);
  ValuableObject valuableObject1Room14(48.5, 48.5, 39.5, 40, 14, "Matuso", 1);
  BlockedSpaceBlock block326(47, 48.5, 39.2, 40.3, 14);
  room14.addValuableObject(valuableObject1Room14);
  room14.addBlockedSpaceBlock(block326);
  addRoom(room14);

  //------------- Room 15 ---------------------
  Room room15(44.5, 49.8, 0.5, 11.3, 15);
  //BlockedSpaceBlock room15LeftSide(42.5, 44.5, 0.5, 12.5, 15);
  //room15.addBlockedSpaceBlock(room15LeftSide);
  //BlockedSpaceBlock room15Bottom(44, 48, 0.5, 2.5, 15);
  //room15.addBlockedSpaceBlock(room15Bottom);
  BlockedSpaceBlock room15RightSideTop(48, 49.8, 10.5, 11.3, 15);
  room15.addBlockedSpaceBlock(room15RightSideTop);
  BlockedSpaceBlock room15PassageBlockTop(48, 49.8, 7.2, 8.7, 15);
  room15.addBlockedSpaceBlock(room15PassageBlockTop);
  BlockedSpaceBlock room15PassageBlockUnder(48, 49.8, 2.8, 4.3, 15);
  room15.addBlockedSpaceBlock(room15PassageBlockUnder);
  BlockedSpaceBlock room15RightSideBottom(48, 49.8, 0.5, 1.7, 15);
  room15.addBlockedSpaceBlock(room15RightSideBottom);
  room15.addPassage(passage16To15Top);
  room15.addPassage(passage16To15Mid);
  room15.addPassage(passage16To15Bottom);

  ValuableObject valuableObject1Room15(45.8, 45.8, 6, 6, 15, "Hammodi", 1);
  BlockedSpaceBlock block327(46, 47.5, 5.2, 6.3, 15);
  room15.addValuableObject(valuableObject1Room15);
  room15.addBlockedSpaceBlock(block327);

  addRoom(room15);

  //------------- Room 16 ---------------------
  Room room16(49.8, 58.5, 0.5, 9.8, 16);
  //BlockedSpaceBlock room16BlockBottom(0.5, 3, 49.5, 58.5, 16);
  //room16.addBlockedSpaceBlock(room16BlockBottom);
  BlockedSpaceBlock room16BlockRightSide(56.5, 58.5, 0.5, 9.8, 16);
  room16.addBlockedSpaceBlock(room16BlockRightSide);
  room16.addPassage(passage16To15Top);
  room16.addPassage(passage16To15Mid);
  room16.addPassage(passage16To15Bottom);
  ValuableObject valuableObject1Room16(54, 54, 5.0, 5.0, 16, "Arjani", 1);
  BlockedSpaceBlock block328(52, 53.5, 4.2, 5.3, 16);
  room16.addValuableObject(valuableObject1Room16);
  room16.addBlockedSpaceBlock(block328);
  addRoom(room16);

  //-------------- ChargeStations ---------------
  ChargeStation cs1(
    5.0, 5.5,    // xLow, xHigh
    45.0, 45.5,  // yLow, yHigh
    2,           // room
    2            // ID
  );
  addChargeStation(cs1);

  ChargeStation cs2(
    45.0, 45.5,
    15.5, 16,
    11,
    11);
  addChargeStation(cs2);

  ChargeStation cs3(
    18.5, 19,
    21.5, 22,
    4,
    4);
  addChargeStation(cs3);

  ChargeStation cs4(
    54, 54.5,
    27.5, 28,
    13,
    13);
  addChargeStation(cs4);

  //------------- Groups for 1 unit -----------------------


  //------------- Groups for >= 4 units -----------------------
  //Group 1
  // addRoomToGroup(1, 1);
  // addRoomToGroup(1, 2);
  // addRoomToGroup(1, 3);

  // //Group 2
  // addRoomToGroup(2, 4);
  // addRoomToGroup(2, 5);
  // addRoomToGroup(2, 6);
  // addRoomToGroup(2, 7);
  // addRoomToGroup(2, 8);
  // addRoomToGroup(2, 9);
  // addRoomToGroup(2, 10);

  // //Group 3
  // addRoomToGroup(3, 11);
  // addRoomToGroup(3, 15);
  // addRoomToGroup(3, 16);

  // //Group 4
  // addRoomToGroup(4, 12);
  // addRoomToGroup(4, 14);
  // addRoomToGroup(4, 13);
}

void RoomController::setupGroups(int nbr) {
  roomGroups.clear();

  if (nbr == 1) {
    addRoomToGroup(1, 1);
    addRoomToGroup(1, 2);
    addRoomToGroup(1, 3);
    addRoomToGroup(1, 4);
    addRoomToGroup(1, 5);
    addRoomToGroup(1, 6);
    addRoomToGroup(1, 7);
    addRoomToGroup(1, 8);
    addRoomToGroup(1, 9);
    addRoomToGroup(1, 10);
    addRoomToGroup(1, 11);
    addRoomToGroup(1, 15);
    addRoomToGroup(1, 16);
    addRoomToGroup(1, 12);
    addRoomToGroup(1, 14);
    addRoomToGroup(1, 13);

  } else if (nbr == 2) {
    addRoomToGroup(1, 1);
    addRoomToGroup(1, 2);
    addRoomToGroup(1, 3);
    addRoomToGroup(1, 4);
    addRoomToGroup(1, 5);
    addRoomToGroup(1, 6);
    addRoomToGroup(1, 7);
    addRoomToGroup(1, 8);
    addRoomToGroup(1, 9);
    addRoomToGroup(1, 10);

    addRoomToGroup(2, 11);
    addRoomToGroup(2, 15);
    addRoomToGroup(2, 16);
    addRoomToGroup(2, 12);
    addRoomToGroup(2, 14);
    addRoomToGroup(2, 13);

  } else if (nbr == 3) {
    addRoomToGroup(1, 1);
    addRoomToGroup(1, 2);
    addRoomToGroup(1, 3);
    addRoomToGroup(1, 4);
    addRoomToGroup(1, 5);
    addRoomToGroup(1, 6);
    addRoomToGroup(1, 7);
    addRoomToGroup(1, 8);
    addRoomToGroup(1, 9);
    addRoomToGroup(1, 10);

    addRoomToGroup(2, 11);
    addRoomToGroup(2, 15);
    addRoomToGroup(2, 16);

    addRoomToGroup(3, 12);
    addRoomToGroup(3, 14);
    addRoomToGroup(3, 13);

  } else if (nbr >= 4) {
    addRoomToGroup(1, 1);
    addRoomToGroup(1, 2);
    addRoomToGroup(1, 3);

    addRoomToGroup(2, 4);
    addRoomToGroup(2, 5);
    addRoomToGroup(2, 6);
    addRoomToGroup(2, 7);
    addRoomToGroup(2, 8);
    addRoomToGroup(2, 9);
    addRoomToGroup(2, 10);

    addRoomToGroup(3, 11);
    addRoomToGroup(3, 15);
    addRoomToGroup(3, 16);

    addRoomToGroup(4, 12);
    addRoomToGroup(4, 14);
    addRoomToGroup(4, 13);
  }
}



bool RoomController::addValuableObjectToRoom(ValuableObject valuableObject) {
  bool valuableObjectAdded = false;
  for (size_t i = 0; i < rooms.size(); i++) {
    if (valuableObject.getRoomNr() == rooms[i].getRoomNr()) {
      rooms[i].addValuableObject(valuableObject);
      valuableObjectAdded = true;
      return valuableObjectAdded;
    }
  }
  return valuableObjectAdded;
}

ValuableObject RoomController::getValuableObject(int valuableObjectId) {
  for (size_t i = 0; i < rooms.size(); i++) {
    if (rooms[i].checkPresenceValuableObject(valuableObjectId)) {
      ValuableObject returnValuableObject = rooms[i].removeValuableObject(valuableObjectId);
      return returnValuableObject;
    }
  }
  return ValuableObject();  //Returns empty object
}

ValuableObject *RoomController::getValuableObjectPointer(int valuableObjectId) {
  for (size_t i = 0; i < rooms.size(); i++) {
    ValuableObject *vo = rooms[i].getValuableObject(valuableObjectId);
    if (vo) { return vo; }
  }
  return nullptr;  //Returns empty object
}

int RoomController::checkValuableObjectRoom(int valuableObjectId) {
  for (size_t i = 0; i < rooms.size(); i++) {
    if (rooms[i].checkPresenceValuableObject(valuableObjectId)) {
      return i;
    }
  }
  return 0;  //If valuable object not found
}

void RoomController::addRoom(Room room) {
  rooms.push_back(room);
}

Room RoomController::getRoom(int roomNr) {
  for (size_t i = 0; i < rooms.size(); i++) {
    if (rooms[i].getRoomNr() == roomNr) {
      Room returnRoom = rooms[i];
      return returnRoom;  //Returns copy of room for reading attributes
    }
  }
  return Room();  //Return empty object
}

Room *RoomController::getRoomPointer(int roomNr) {
  for (auto &room : rooms) {
    if (room.getRoomNr() == roomNr) {
      return &room;
    }
  }
  return nullptr;
}

void RoomController::addRoomToGroup(int groupId, int roomNr) {
  roomGroups[groupId].push_back(roomNr);
}

bool RoomController::isRoomInGroup(int groupId, int roomNr) {
  if (roomGroups.find(groupId) == roomGroups.end()) return false;

  for (int rn : roomGroups[groupId]) {
    if (rn == roomNr) return true;
  }
  return false;
}

std::vector<int> RoomController::getRoomsInGroup(int groupId) {
  if (roomGroups.find(groupId) != roomGroups.end())
    return roomGroups[groupId];
  return {};
}

void RoomController::printInformationAboutRoom(int roomNr) {
  double lowX = 0;
  double highX = 0;
  double lowY = 0;
  double highY = 0;
  String blockedSpaceNrInfo = "";
  String passagesNrInfo = "";
  String valuableObjectsNrInfo = "";

  for (size_t i = 0; i < rooms.size(); i++) {
    if (rooms[i].getRoomNr() == roomNr) {
      lowX = rooms[i].getXLow();
      highX = rooms[i].getXHigh();
      lowY = rooms[i].getYLow();
      highY = rooms[i].getYHigh();
      auto blocks = rooms[i].getBlockedSpaceBlocks();
      if (blocks.empty()) {
        blockedSpaceNrInfo = "No blocked Spaces";
      } else {
        blockedSpaceNrInfo = "Blocked spaces: " + String(blocks.size());
      }
      auto passages = rooms[i].getPassages();
      if (passages.empty()) {
        passagesNrInfo = "No passages in room";
      } else {
        passagesNrInfo = "Passages: " + String(passages.size());
      }
      auto valuableObjects = rooms[i].getValuableObjects();
      if (valuableObjects.empty()) {
        valuableObjectsNrInfo = "No valuable objects in room";
      } else {
        valuableObjectsNrInfo = "valuable objects: " + String(valuableObjects.size());
      }
    }
  }
  Serial.print("************* Room: ");
  Serial.print(roomNr);
  Serial.print(" ***************");
  Serial.println();
  Serial.print("lowX: ");
  Serial.print(lowX);
  Serial.print(" ");
  Serial.print("highX: ");
  Serial.print(highX);
  Serial.print(" ");
  Serial.print("lowY: ");
  Serial.print(lowY);
  Serial.print(" ");
  Serial.print("highY: ");
  Serial.print(highY);
  Serial.print(" ");
  Serial.println();
  Serial.print(blockedSpaceNrInfo);
  Serial.println();
  Serial.print(passagesNrInfo);
  Serial.println();
  Serial.print(valuableObjectsNrInfo);
  Serial.println();
  Serial.print("*************************************");
  Serial.println();
}
void RoomController::addChargeStation(const ChargeStation &station) {
  chargeStations.push_back(station);
}

bool RoomController::updateChargingStatus(double robotX, double robotY) {
  bool anyCharging = false;
  const double tolerance = 0.5;  // Felmarginal på 0.5

  for (size_t i = 0; i < chargeStations.size(); ++i) {
    ChargeStation &cs = chargeStations[i];
    bool inside =
      robotX >= cs.getXLow() - tolerance && robotX <= cs.getXHigh() + tolerance && robotY >= cs.getYLow() - tolerance && robotY <= cs.getYHigh() + tolerance;

    cs.setCharging(inside);

    if (inside) {
      anyCharging = true;
      Serial.print("Robot står på laddstation ID: ");
      Serial.println(cs.getId());
    }
  }
  return anyCharging;
}

bool RoomController::isPointInsideRoom(int roomNr, double x, double y) {
  for (auto &r : rooms) {
    if (r.getRoomNr() == roomNr) {
      return (x >= r.getXLow() && x <= r.getXHigh() && y >= r.getYLow() && y <= r.getYHigh());
    }
  }
  return false;
}
