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

    const std::string* name;
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
    Room* new_room(const std::string& name);

    void destroy_rooms();

    std::map<std::string, Room*> rooms;
    std::map<std::string, Action*> actions;
};

World::~World() {
    this->destroy_rooms();
}

Room* World::new_room(const std::string& name) {
    if (this->rooms.find(name) != this->rooms.end()) {
        logs::err("room with this name already exists (", name ,")");
        return nullptr;
    }

    Room* buf {new Room};
    auto res = this->rooms.insert(std::pair<std::string, Room*>{name, buf});
    buf->name = &res.first->first;

    return buf;
}

int World::create()
{
    Room* buf;
    if (buf = this->new_room("al'Thor Farm"); buf == nullptr) { return 1; }
    buf->description = "A farm belonging to the al'Thor family.";

    if (buf = this->new_room("Emond's Road 1"); buf == nullptr) { return 1; }
    buf->description = "A road connecting the western farms to Emond's Field.";

    return 0;
}

Room* World::find_room(const std::string& name)
{
    auto res = this->rooms.find(name);
    if (res != this->rooms.end()) { return res->second; }

    return nullptr;
}

void World::destroy_rooms()
{
    for (auto i {this->rooms.begin()}; i != this->rooms.end(); ++i) {
        delete(i->second);
    }
}

#endif // SRC_WORLD_HPP_
