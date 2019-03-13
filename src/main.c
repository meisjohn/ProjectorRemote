// Main program for ProjectorRemote

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <projectorremote.h>

void handle_commands(void *pvParameter)
{
    int command = PR_CMD_NULL;

    while (1)
    {
        // blocking call
        command = get_command();

        if (command == PR_CMD_POWER_ON)
        {
            send_power_on();
        }
        if (command == PR_CMD_POWER_BLANK)
        {
            send_power_blank();
        }
        if (command == PR_CMD_POWER_STANDBY)
        {
            send_power_standby();
        }
    }

    //prevent receipt of tasks too quickly.
    vTaskDelay(1000 / portTICK_PERIOD_MS);

}

void app_main()
{
    init_wifi();
    init_serial();

    xTaskCreate(&handle_commands, "handle_commands", configMINIMAL_STACK_SIZE, NULL, 5, NULL);

}
