#include "PersonList.h"
#include "Person.h"

PersonList::PersonList() {
}

void PersonList::addPerson(int id, double x, double y, int roomNbr) {
  // Kolla om personen redan finns
  for (auto &p : intruders) {
    if (p.getId() == id) {
      p.setX(x);
      p.setY(y);
      p.setRoom(roomNbr);
      return;
    }
  }

  // Om personen inte finns, skapa en ny och lägg till
  Person newPerson;
  newPerson.setId(id);
  newPerson.setX(x);
  newPerson.setY(y);
  newPerson.setRoom(roomNbr);
  
  intruders.push_back(newPerson);
}

bool PersonList::removePersonById(int id) {
  for (auto it = intruders.begin(); it != intruders.end(); ++it) {
    if (it->getId() == id) {
      intruders.erase(it);
      return true;
    }
  }
  return false;
}

Person PersonList::getPersonById(int id) {
  Person person;

  for (int i = 0; i < intruders.size(); i++) {
    if (id == intruders[i].getId()) {
      person = intruders[i];
      return person;
    }
  }

  return person;
}

Person* PersonList::getPersonByIndex(int index){
  return &intruders[index];
  }


int PersonList::getTotalPeople() const {
  return intruders.size();
}

void PersonList::clear() {
  intruders.clear();
}
