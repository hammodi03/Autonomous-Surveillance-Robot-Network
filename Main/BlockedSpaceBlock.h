#ifndef BLOCKED_SPACE_BLOCK_H
#define BLOCKED_SPACE_BLOCK_H
#include "Space.h"

class BlockedSpaceBlock : public Space {

    public: 
    BlockedSpaceBlock(double xLow, double xHigh, double yLow, double yHigh, int room); 

}; 

#endif