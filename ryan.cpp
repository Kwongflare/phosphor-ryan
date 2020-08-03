#include <cstdint>
#include <iostream>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/message/append.hpp>
#include <variant>
#include <fstream>
#include <map>
#include <string>
#include <iterator>

/** An example dbus client application.
 **  Calls org.freedesktop.login1's ListUsers interface to find all active
 **  users in the system and displays their username.
 **/

using namespace std;
using namespace sdbusplus;

#define REQUIRED_NUM_ARGS 4
#define ERR_NONE 0
#define ERR_TOO_FEW_ARGS 1

const char* PHOSPHOR_SENSOR_VAL_INTF = "xyz.openbmc_project.Sensor.Value";
const char* DBUS_PROPERTIES_INTF = "org.freedesktop.DBus.Properties";
const char* PROPERTIES_GET = "Get";
const char* ADC_SENSOR_SERVICE = "xyz.openbmc_project.ADCSensor";
const char* ADCS_VOL_3_3V_AUX = "/xyz/openbmc_project/sensors/voltage/VOL_3_3V_AUX";
const char* ADCS_VOL_5V_AUX = "/xyz/openbmc_project/sensors/voltage/VOL_5_V_AUX";
const char* ADCS_VOL_BATTERY = "/xyz/openbmc_project/sensors/voltage/VOL_BATTERY";
const char* ADCS_VOL_CPU9_VCORE = "/xyz/openbmc_project/sensors/voltage/VOL_CPU0_VCORE";
const char* ADCS_VOL_CPU1_VCORE = "/xyz/openbmc_project/sensors/voltage/VOL_CPU1_VCORE";
const char* ADCS_VOL_DIMM_ABC = "/xyz/openbmc_project/sensors/voltage/VOL_DIMM_ABC";
const char* ADCS_VOL_DIMM_DEF = "/xyz/openbmc_project/sensors/voltage/VOL_DIMM_DEF";
const char* ADCS_VOL_DIMM_GHJ = "/xyz/openbmc_project/sensors/voltage/VOL_DIMM_GHJ";
const char* ADCS_VOL_DIMM_KLM = "/xyz/openbmc_project/sensors/voltage/VOL_DIMM_KLM";
const char* ADCS_VOL_PCH_1_05V = "/xyz/openbmc_project/sensors/voltage/VOL_1_05V";
const char* ADCS_VOL_PCH_1_8V = "/xyz/openbmc_project/sensors/voltage/VOL_1_8V";
const char* ADCS_VOL_VNN = "/xyz/openbmc_project/sensors/voltage/VOL_VNN";
const char* ADCS_VOL_SYS0_VCCIO = "/xyz/openbmc_project/sensors/voltage/VOL_SYS0_VCCIO";
const char* ADCS_VOL_SYS1_VCCIO = "/xyz/openbmc_project/sensors/voltage/VOL_SYS1_VCCIO";
const char* ADCS_VOL_SYS_3_3V = "/xyz/openbmc_project/sensors/voltage/VOL_SYS_3_3V";
const char* ADCS_VOL_SYS_V = "/xyz/openbmc_project/sensors/voltage/VOL_SYS_V";

typedef enum {
    VOL_3_3V_AUX,
    VOL_5V_AUX,
    VOL_BATTERY,
    VOL_CPU0_VCORE,
    VOL_CPU1_VCORE,
    VOL_DIMM_ABC,
    VOL_DIMM_DEF,
    VOL_DIMM_GHJ,
    VOL_DIMM_KLM,
    VOL_PCH_1_05V,
    VOL_PCH_1_8V,
    VOL_PCH_VNN,
    VOL_SYS0_VCCIO,
    VOL_SYS1_VCCIO,
    VOL_SYS_3_3V,
    VOL_SYS_V
}ADC_SENSOR;

typedef struct {
    ADC_SENSOR sensor;
    const char* object;
} sensor_dbus_obj_t;

std::string get_time()
{
    std::string retVal;
    const char *service = "xyz.openbmc_project.Time.Manager";
    const char *object = "/xyz/openbmc_project/time/bmc";
    const char *target = "xyz.openbmc_project.Time.EpochTime";
    const char *property = "Elapsed";

    auto bus = bus::new_default_system();
    auto message = bus.new_method_call(service, object, "org.freedesktop.DBus.Properties", "Get");
    message.append(target, property);

    return retVal;
}


int main( int argc, char *argv[] )
{

    if (argc < REQUIRED_NUM_ARGS)
    {
        printf("Too few arguments\n");
        return ERR_TOO_FEW_ARGS;
    }
    map<int, string> adc_map;
    adc_map.insert(pair<int, string>(0, ADCS_VOL_3_3V_AUX));
    adc_map.insert(pair<int, string>(1, ADCS_VOL_5V_AUX));
    adc_map.insert(pair<int, string>(2, ADCS_VOL_BATTERY));
    adc_map.insert(pair<int, string>(3, ADCS_VOL_CPU9_VCORE));
    adc_map.insert(pair<int, string>(4, ADCS_VOL_CPU1_VCORE));
    adc_map.insert(pair<int, string>(5, ADCS_VOL_DIMM_ABC));
    adc_map.insert(pair<int, string>(6, ADCS_VOL_DIMM_DEF));
    adc_map.insert(pair<int, string>(7, ADCS_VOL_DIMM_GHJ));
    adc_map.insert(pair<int, string>(8, ADCS_VOL_DIMM_KLM));
    adc_map.insert(pair<int, string>(9, ADCS_VOL_PCH_1_05V));
    adc_map.insert(pair<int, string>(10, ADCS_VOL_PCH_1_8V));
    adc_map.insert(pair<int, string>(11, ADCS_VOL_VNN));
    adc_map.insert(pair<int, string>(12, ADCS_VOL_SYS0_VCCIO));
    adc_map.insert(pair<int, string>(13, ADCS_VOL_SYS1_VCCIO));
    adc_map.insert(pair<int, string>(14, ADCS_VOL_SYS_3_3V));
    adc_map.insert(pair<int, string>(15, ADCS_VOL_SYS_V));

    map<int, string>::iterator itr;


    const char *path = "/var/log/phosphor-ryan.txt";
    std::ofstream file(path);

    printf("Arg1: %s\n", argv[1]);
    printf("Arg2: %s\n", argv[2]);
    printf("Arg3: %s\n", argv[3]);
    printf("Arg4: %s\n", argv[4]);

    auto b = bus::new_default_system();
    printf("Bus found\n");
    auto m = b.new_method_call(argv[1], argv[2], argv[3], argv[4]);

    for (itr = adc_map.begin(); itr != adc_map.end(); itr++)
    {
        auto m = b.new_method_call(ADC_SENSOR_SERVICE, itr->second.c_str(),
                DBUS_PROPERTIES_INTF, PROPERTIES_GET);
        m.append(PHOSPHOR_SENSOR_VAL_INTF, "Value");
        auto reply = b.call(m);
        std::variant<double> a;
        reply.read(a);
        printf("%f\n", std::get<double>(a));
    }
    
//    char buff[100];
//    snprintf(buff, sizeof(buff), "%s: %s - %f", argv[5], argv[6], std::get<double>(a));
//    std::string data = buff;
//    file << buff;

    return ERR_NONE;

}
