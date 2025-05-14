/* This is where the observable world is described, early WIP, will probably
 * move to a separate folder later as one file would grow very large and unruly.
*/

#ifndef SRC_WORLD_HPP_
#define SRC_WORLD_HPP_

#include <string>
#include <map>

#include "Actions.hpp"
#include "logs.hpp"

// TODO reveiw classes (privatise, add constructors, virtualise, etc.)

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

class Room;

struct Room_link {
    Room* target;
};

class Room : public Container {
public:
    virtual ~Room() = default;

    int new_link(const std::string& name, Room* to, bool symmetric = true);
    void dump();

    //private:
    std::map<std::string, Room_link> links;
};

int Room::new_link(const std::string& name, Room* to, bool symmetric)
{
    if (to == nullptr) {
        logs::err("attempted to link room '", *this->name, "' to null: ", name);
        return 1;
    }

    {
        auto found = this->links.find(name);
        if (found != this->links.end()) {
            if (found->second.target == to) { return 0; } // already linked
        }
    }

    this->links[name] = Room_link {.target = to};

    if (symmetric) {
        // TODO this map should live somewhere permanent;
        std::map<std::string, std::string> opposites {
            std::pair<std::string, std::string>("n", "s"),
            std::pair<std::string, std::string>("s", "n"),
            std::pair<std::string, std::string>("e", "w"),
            std::pair<std::string, std::string>("w", "e"),
            std::pair<std::string, std::string>("up", "down"),
            std::pair<std::string, std::string>("down", "up"),
        };
        auto other_direction = opposites.find(name);

        if (other_direction != opposites.end()) {
            to->new_link(other_direction->second, this);
        } else {
            logs::err("no opposite direction found for '", name, "'");
        }
    }

    return 0;
}

void Room::dump()
{
    for (auto i {this->links.begin()}; i != this->links.end(); ++i) {
        logs::info("R.L: ", i->first, " -> ", *i->second.target->name);
    }
}

class Player final {
public:
    Room* location;
};

class World final {
public:
    ~World();

    bool add_action(const std::string& name, Action* action);
    int create();
    void dump();
    Room* find_room(const std::string& name);
    Action* find_action (const std::string& name);

    Player player;

private:
    Room* new_room(const std::string& name);

    void destroy_actions();
    void destroy_rooms();

    std::map<std::string, Room*> rooms;
    std::map<std::string, Action*> actions;
};

World::~World() {
    this->destroy_actions();
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

bool World::add_action(const std::string& name, Action* action)
{
    std::pair<std::string, Action*> buf {name, action};
    return this->actions.insert(std::move(buf)).second;
}

int World::create()
{
    Room* buf;
    if (buf = this->new_room("al'Thor Farm"); buf == nullptr) { return 1; }
    buf->description = "A farm belonging to the al'Thor family.";

    if (buf = this->new_room("Emond's Road 1"); buf == nullptr) { return 1; }
    buf->description = "A road connecting the western farms to Emond's Field.";
    buf->new_link("n", this->find_room("al'Thor Farm"));

    return 0;
}

void World::dump()
{
    std::cout << "ROOMS" << std::endl;
    for (auto i {this->rooms.begin()}; i != this->rooms.end(); ++i) {
        logs::info("R: ", i->first);
        if (i->second != nullptr) { i->second->dump(); }
    }
}


Room* World::find_room(const std::string& name)
{
    auto res = this->rooms.find(name);
    if (res != this->rooms.end()) { return res->second; }

    return nullptr;
}

Action* World::find_action(const std::string& name) {
    auto res = this->actions.find(name);
    if (res != this->actions.end()) { return res->second; }

    return nullptr;
}

void World::destroy_actions()
{
    for (auto i {this->actions.begin()}; i != this->actions.end(); ++i) {
        delete(i->second);
    }
    this->actions.clear();
}

void World::destroy_rooms()
{
    for (auto i {this->rooms.begin()}; i != this->rooms.end(); ++i) {
        delete(i->second);
    }
    this->rooms.clear();
}

#endif // SRC_WORLD_HPP_
