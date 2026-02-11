#ifndef PERSONLIST_H
#define PERSONLIST_H

#include <vector>
class Person;

class PersonList {
  private:

  public:
  PersonList();
  std::vector<Person> intruders;

  void addPerson(int id, double x, double y, int roomNbr);
  bool removePersonById(int id);
  Person getPersonById(int id);
  int getTotalPeople() const;
  Person* getPersonByIndex(int index);

  void clear();
};

#endif
