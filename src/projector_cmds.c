
#include "driver/uart.h"
#include "esp_log.h"
#include <string.h>
#include <benq.h>

#define RP_UART_NUM UART_NUM_1

#define RP_UART_TXD  (GPIO_NUM_16)
#define RP_UART_RXD  (GPIO_NUM_17)
#define RP_UART_RTS  (UART_PIN_NO_CHANGE)
#define RP_UART_CTS  (UART_PIN_NO_CHANGE)

#define RP_BUF_SIZE (1024)
#define BENQ_MAX_BUFFER 12

const char log_name[] = "SERIAL_IF";

void init_serial()
{
        uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(RP_UART_NUM, &uart_config);
    uart_set_pin(RP_UART_NUM, RP_UART_TXD, RP_UART_RXD, RP_UART_RTS, RP_UART_CTS);
    uart_driver_install(RP_UART_NUM, RP_BUF_SIZE * 2, 0, 0, NULL, 0);

    esp_log_level_set(log_name, ESP_LOG_INFO);
    ESP_LOGI(log_name, "Serial Interface Initialized");

}

int send_data( const char* data, int data_len)
{
    const int tx_bytes = uart_write_bytes(RP_UART_NUM, data, data_len);
    ESP_LOGI(log_name, "Wrote %d bytes", tx_bytes);
    return tx_bytes;
}

int receive_data(uint8_t *recv_data, const int max_data_len)
{

    const int rx_bytes = uart_read_bytes(RP_UART_NUM, recv_data, max_data_len-1, 1000 / portTICK_RATE_MS);
    if (rx_bytes > 0) {
        recv_data[rx_bytes] = 0;
        ESP_LOGI(log_name, "Read %d bytes: '%s'", rx_bytes, recv_data);
        ESP_LOG_BUFFER_HEXDUMP(log_name, recv_data, rx_bytes, ESP_LOG_INFO);
    }

    return rx_bytes;
}

uint32_t send_benq_packet(BENQ_SendPacket* sp)
{
    static uint8_t recv_buffer[BENQ_MAX_BUFFER];
    int bytes = 0;
    uint32_t rtn = 0;

    memset(recv_buffer, 0, BENQ_MAX_BUFFER);

    // since the length is specified as an ascii value
    // (except for 3A for 10 bytes), take the integer
    // value of the len, subtracted by the integer value of
    // the zero character (0x30) and add one for the CR
    const int sp_buffer_len = ((int)(sp->length) - 0x29); 

    bytes = send_data((char*)sp,sp_buffer_len);

    rtn = BENQ_SUCCESS;

    // If the command was specifically addressed
    if ( sp->id != BENQ_ID_ALL)
    {
        // check the response
        bytes = receive_data(recv_buffer, BENQ_MAX_BUFFER);

        if (bytes != 0)
        {

            if (sp->cmd_type == BENQ_CMD_SET && bytes == sizeof(BENQ_SetReturnPacket))
            {
                BENQ_SetReturnPacket rp;
                memcpy(&rp, recv_buffer, bytes);
                rtn = BENQ_check_set_return(&rp);
            }
            else if (sp->cmd_type == BENQ_CMD_GET && bytes == sizeof(BENQ_GetReturnPacket))
            {
                BENQ_GetReturnPacket rp;
                memcpy(&rp, recv_buffer, bytes);
                rtn = (rp.value1 << 2*8) + 
                      (rp.value2 << 1*8) + 
                      (rp.value3);
            }
        }
    }

    return rtn;
}

void send_power_on()
{
    BENQ_SendPacket sp;
    uint32_t rtn; 
    sp.id = BENQ_ID_ALL;

    BENQ_power_on(&sp);
    rtn = send_benq_packet(&sp);
    ESP_LOGI(log_name, "Send Power On: %s", (rtn == BENQ_SUCCESS? "SUCCESS": "ERROR"));
}

void send_power_blank()
{
    BENQ_SendPacket sp;
    uint32_t rtn; 
    sp.id = BENQ_ID_ALL;

    BENQ_power_blank(&sp);
    rtn = send_benq_packet(&sp);
    ESP_LOGI(log_name, "Send Power On: %s", (rtn == BENQ_SUCCESS? "SUCCESS": "ERROR"));
}

void send_power_standby()
{
    BENQ_SendPacket sp;
    uint32_t rtn; 
    sp.id = BENQ_ID_ALL;

    BENQ_power_standby(&sp);
    rtn = send_benq_packet(&sp);
    ESP_LOGI(log_name, "Send Power On: %s", (rtn == BENQ_SUCCESS? "SUCCESS": "ERROR"));
}