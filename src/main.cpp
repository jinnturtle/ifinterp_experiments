#include <iostream>
#include <string>
#include <map>

#include "World.hpp"
#include "version.hpp"
#include "logs.hpp"

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

int begin_play(World* world)
{
#ifdef DEBUG
    std::cout << "\n*** DUMP ***" << std::endl;
    world->dump();
    std::cout << "\n*** END DUMP ***" << std::endl;
#endif //DEBUG

    Player* player {&world->player};
    tmp_look_around(player);
    player->go("s");
    tmp_look_around(player);

    std::cout
        << "\n> " << "just kidding, the prompt is not implemented yet"
        << std::endl;

    return 0;
}
