#ifndef PASSAGE_H
#define PASSAGE_H

class Passage {
    private: 
        int passageDirection; // 0 horizontal, 1 vertical
        double passageCoordLow;
        double passageCoordHigh; 
        double crossCoord; 
        int room1; // Upper or most left
        int room2; //Lower or most right
        
        
        
        int room3;
        int room4;
        int room5;
        int room6;
        int room7;
        int room8;
        int room9;
        int room10;

    public: 
        Passage(int passageDirection, double passageCoordLow, double passageCoordHigh, double crossCoord, int room1, int room2);

        int getPassageDirection();
        double getPassageCoordLow(); 
        double getPassageCoordHigh(); 
        double getCrossCoord(); 
        int getRoom1();
        int getRoom2();
        int getRoom3(); 
        int getRoom4(); 
        int getRoom5(); 
        int getRoom6(); 
        int getRoom7(); 
        int getRoom8(); 
        int getRoom9();  
        int getRoom10();  
};

#endif