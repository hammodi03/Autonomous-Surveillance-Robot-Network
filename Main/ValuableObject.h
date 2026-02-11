#ifndef VALUABLE_OBJECT_H
#define VALUABLE_OBJECT_H

#include <string>
#include "Space.h"

class ValuableObject : public Space {
private:
    std::string name;
    int id;
    bool stolen = false;
    bool isZone = false;
    bool isComZone = false;

public:
    ValuableObject();
    ValuableObject(double xLow, double xHigh, double yLow, double yHigh,
                   int room,
                   const std::string& name,
                   int id,
                   bool stolen = false,
                   bool isZone = false,
                   bool isComZone = false);

    void setName(const std::string& name);
    std::string getName() const;

    void setId(int id);
    int getId() const;

    void setStolen(bool stolen);
    bool isStolen() const;

    void setZone(bool isZone);
    bool getIsZone() const;

    void setComZone(bool isComZone);
    bool getIsComZone() const;
};

#endif
