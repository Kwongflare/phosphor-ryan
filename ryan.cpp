#include <cstdint>
#include <iostream>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/message/append.hpp>
#include <variant>
#include <fstream>

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

    const char *path = "/var/log/phosphor-ryan.txt";
    std::ofstream file(path);

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

    printf("Method %s\n", reply.is_method_error() ? "failed" : "succeeded");
    printf("sender: %s\n", reply.get_sender());

    std::variant<double> a;
    reply.read(a);

    printf("%f\n", std::get<double>(a));
    
    char buff[100];
    snprintf(buff, sizeof(buff), "%s: %s - %f", argv[5], argv[6], std::get<double>(a));
    std::string data = buff;
    file << buff;

    return ERR_NONE;

}
