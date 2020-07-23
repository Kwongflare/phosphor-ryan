#include <cstdint>
#include <iostream>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/message/append.hpp>

/** An example dbus client application.
 **  Calls org.freedesktop.login1's ListUsers interface to find all active
 **  users in the system and displays their username.
 **/

#define REQUIRED_NUM_ARGS 4
#define ERR_NONE 0
#define ERR_TOO_FEW_ARGS 1


int main( int argc, char *argv[] )
{
    using namespace sdbusplus;

    if (argc < REQUIRED_NUM_ARGS)
    {
        printf("Too few arguments\n");
        return ERR_TOO_FEW_ARGS;
    }

   printf("Arg1: %s\n", argv[1]);
   printf("Arg2: %s\n", argv[2]);
   printf("Arg3: %s\n", argv[3]);
   printf("Arg4: %s\n", argv[4]);

    auto b = bus::new_default_system();
    printf("Bus found\n");
    auto m = b.new_method_call(argv[1], argv[2], argv[3], argv[4]);

    if (argc > 5)
    {
        m.append(argv[5], argv[6]);
    }

    printf("Method call created\n");

    auto reply = b.call(m);
    printf("Method called \n");

    std::vector<std::string> names;
    reply.read(names);

    printf("reply read \n");

    for (auto& name : names)
    {
        printf("%s\n", std::string(name).c_str());
    }

    return ERR_NONE;

}
