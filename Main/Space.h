#ifndef SPACE_H
#define SPACE_H

class Space {
    private:
        double xLow;
        double xHigh;
        double yLow;
        double yHigh;

        int room; 

    public:
        Space();
        Space(double xLow, double xHigh, double yLow, double yHigh, int room); 

        void setXLow(double xLow); 
        double getXLow();

        void setXHigh(double xHigh); 
        double getXHigh();

        void setYLow(double yLow); 
        double getYLow(); 

        void setYHigh(double yHigh);
        double getYHigh(); 

        void setRoomNr(int room);
        int getRoomNr() const; 
};

#endif