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
    if(world.create() == 0) { std::cout << "done"; }
    else { std:: cout << "ERROR"; }
    std::cout << std::endl;

    world.player = Player {.location = world.find_room("al'Thor Farm")};
    if (world.player.location == nullptr) { logs::err("player is nowhere"); }

    begin_play(&world);

    return 0;
}

int begin_play(World* world)
{
    std::cout << "\n[" << world->player.location->name << "]" << std::endl;
    std::cout
        << "\n> " << "just kidding, the prompt is not implemented yet"
        << std::endl;

    return 0;
}
