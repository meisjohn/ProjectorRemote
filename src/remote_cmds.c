#include <projectorremote.h>

#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "string.h"

static const char *TAG = "scan";

static const char *WIFISSID = "SSID";
static const char *WIFIPWD = "PWD123456";

static esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {

       case SYSTEM_EVENT_STA_START:
           ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
           ESP_ERROR_CHECK(esp_wifi_connect());
           break;

       case SYSTEM_EVENT_STA_GOT_IP:
           ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
           ESP_LOGI(TAG, "Got IP: %s\n",
                    ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
           break;

       case SYSTEM_EVENT_STA_DISCONNECTED:
           ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
           ESP_ERROR_CHECK(esp_wifi_connect());
           break;

       default:
           break;
    }

    return ESP_OK;
}

void init_wifi()
{
    nvs_flash_init();  
    tcpip_adapter_init();  

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    esp_event_loop_init(wifi_event_handler, NULL); 

    wifi_config_t wifi_config;
    strcpy((char *)wifi_config.sta.ssid,(char *)WIFISSID);
    strcpy((char *)wifi_config.sta.password,(char *)WIFIPWD);
    wifi_config.sta.scan_method = WIFI_FAST_SCAN;
    wifi_config.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
    wifi_config.sta.threshold.rssi = -100;
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config); 
    esp_wifi_start();
}

int get_command()
{
    return PR_CMD_POWER_ON;
}
