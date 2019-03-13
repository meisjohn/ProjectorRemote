#ifndef __PROJECTORREMOTE_H_
#define  __PROJECTORREMOTE_H_

void init_wifi();
int get_command();

void init_serial();
void send_power_on();
void send_power_blank();
void send_power_standby();

#define PR_CMD_NULL 0x00
#define PR_CMD_POWER_ON 0x01
#define PR_CMD_POWER_STANDBY 0x02
#define PR_CMD_POWER_BLANK 0x03

#endif //__PROJECTORREMOTE_H_