/**
 * This example shows how to use the Thermo3 Click wrapper of the LetMeCreate
 * library.
 *
 * It reads the temperature from the sensor and exits.
 *
 * The Thermo3 Click must be inserted in Mikrobus 1 before running this program.
 */

#include <stdio.h>
#include <letmecreate/letmecreate.h>


int main(void)
{
    float temperature = 0.f;
    char command[64];
    char ip[16];
    int n;

    printf( "IP to pub:");
    


    //read from stdin
    scanf("%s", ip);



    i2c_init();
    i2c_select_bus(MIKROBUS_1);

    thermo3_click_enable(0);
    thermo3_click_get_temperature(&temperature);

    
    sprintf(command, "mosquitto_pub -h %s -p 1883 -t 'topic1' -m '%.3f' ", ip, temperature);

    //execute command:
    if (n > 0)
        system(command);

    thermo3_click_disable();

    i2c_release();

    return 0;
}
