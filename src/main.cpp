#include <array>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

#include "Actions.hpp"
#include "World.hpp"
#include "version.hpp"
#include "logs.hpp"

int add_world_actions(World* world);
int begin_play(World* world);

int main()
{
    constexpr char program_name[] {"Ifinterp"};
    std::cout
        << "*** " << program_name << " " << version_str() << " ***"
        << std::endl;
    std::cout << "Hello IF world!" << std::endl;

    World world;
    std::cout << "Creating world... ";
    if(world.create() == 0) { std::cout << "done."; }
    else { std:: cout << "ERROR"; }
    std::cout << std::endl;

    add_world_actions(&world);

    world.player = Player();
    world.player.location = world.find_room("al'Thor Farm");
    if (world.player.location == nullptr) { logs::err("player is nowhere"); }

    begin_play(&world);

    return 0;
}

// TODO this is only for some quick testing - remove soon
void tmp_look_around(Player* player) {
    std::cout << "\n[" << *player->location->name << "]"
    << "\n" << player->location->description
    << std::endl;
}

int add_world_actions(World* world)
{
    std::string name;
    void (*action_f) (void**);
    Action* action;

    name = "go";
    action_f = [](void** in) -> void {
        Player* player = reinterpret_cast<Player*>(in[0]);
        std::string* direction = reinterpret_cast<std::string*>(in[1]);
        std::cout << "player wants to 'go' " << *direction << std::endl;

        auto found = player->location->links.find(*direction);
        if (found != player->location->links.end()) {
            player->location = found->second.target;
        } else {
            std::cout << "\nYou don't see a way that would lead "
            << *direction << "."
            << std::endl;
        }

    };
    action = new Action {.input_n = 2, .output_n = 0, .f = action_f};

    if (!world->add_action(name, action)) {
        logs::err("duplicate action name '", name, "'");
    }

    return 0;
}

int begin_play(World* world)
{
#ifdef DEBUG
    std::cout << "\n*** DUMP ***" << std::endl;
    world->dump();
    std::cout << "\n*** END DUMP ***" << std::endl;
#endif //DEBUG

    Player* player {&world->player};
    //    player->go("s");
    //    tmp_look_around(player);

    bool should_exit {false};
    std::string buf;

    while(!should_exit) {
        tmp_look_around(player);
        std::cout << "> ";
        getline(std::cin, buf);
        if (buf == "q") { should_exit = true; break; }

        /* TODO clean and implement, this is just POC testing while under
           construction */
        size_t start {0};
        size_t end = buf.find(' ', start);
        std::string token1 = buf.substr(start, end - start);
        std::cout << "'" << token1 << "'" << std::endl;

        start = end +1;
        end = buf.find(' ', start);
        std::string token2 = buf.substr(start, end - start);
        std::cout << "'" << token2 << "'" << std::endl;

        void* params[2] {player, &token2};

        Action* action = world->find_action(token1);
        if (action) { action->f(params); }
        else {
            std::cout << "Not sure what you mean by \"" << token1 << "\""
            << std::endl;
        }
    }

    return 0;
}
