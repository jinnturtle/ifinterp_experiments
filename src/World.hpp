/* This is where the observable world is described, early WIP, will probably
 * move to a separate folder later as one file would grow very large and unruly.
*/

#ifndef SRC_WORLD_HPP_
#define SRC_WORLD_HPP_

#include <string>
#include <map>

#include "Actions.hpp"
#include "logs.hpp"

// TODO reveiw classes (where needed: add constructros, virtualise, etc.)

class Thing {
public:
    virtual ~Thing() = default;

    /* TODO - think how to avoid duplication of data as name is also usually the
       key in associative containers */
    std::string name;
    std::string description;
};

class Container : public Thing {
public:
    virtual ~Container() = default;

    std::map<std::string, Thing*> contents;
};

class Room : public Container {
public:
    virtual ~Room() = default;

    std::map<std::string, Room*> exits;
};


struct Player final {
    Room* location;
};


class World final {
public:
    ~World();

    int create();
    Room* find_room(const std::string& name);

    Player player;

private:
    std::map<std::string, Room*> rooms;
    std::map<std::string, Action*> actions;
};

World::~World() {
    for (auto i {this->rooms.begin()}; i != this->rooms.end(); ++i) {
        delete(i->second);
    }
}

int World::create()
{
    // TODO temporary duplication of data, will change with Room revision
    Room* buf;
    buf = new Room;
    buf->name = "al'Thor Farm";
    buf->description = "A farm belonging to the al'Thor family.";
    this->rooms[buf->name] = buf;

    buf = new Room;
    buf->name = "Emond's Road 1";
    buf->description = "A road connecting the western farms to Emond's Field.";
    this->rooms[buf->name] = buf;

    return 0;
}

Room* World::find_room(const std::string& name)
{
    auto res = this->rooms.find(name);
    if (res != this->rooms.end()) { return res->second; }

    return nullptr;
}

#endif // SRC_WORLD_HPP_
