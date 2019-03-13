// benq
// This file specifies the RS232 interface protocols to the BENQ Projectors
// See RS232-Commans_all Product Lines.pdf for reference

#ifndef __BENQ_H_
#define __BENQ_H_

// Commmon Packet values
#define BENQ_CR 0x0D

#define BENQ_CMD_SET 0x73       // 's'
#define BENQ_CMD_GET 0x67       // 'g'
#define BENQ_CMD_RPLY 0x72      // 'r'
#define BENQ_CMD_VALID 0x2B     // '+'
#define BENQ_CMD_INVALID 0x2D   // '-'

#define BENQ_ID_ALL 0x3939      // '99'

#define BENQ_LEN_4  0x34        // '4'
#define BENQ_LEN_5  0x35        // '5'
#define BENQ_LEN_8  0x38        // '8'
#define BENQ_LEN_9  0x39        // '9'
#define BENQ_LEN_10 0x3A        // ':'

#define BENQ_SUCCESS 1
#define BENQ_FAILURE 0

typedef struct BENQ_SendPacket {
    unsigned char length; // Length of the message, excluding CR (BENQ_LEN_8)
    unsigned short id;
    unsigned char cmd_type;
    unsigned char cmd_code;
    unsigned char value1;
    unsigned char value2;
    unsigned char value3;
    unsigned char CR;
    } BENQ_SendPacket;

typedef struct BENQ_SetReturnPacket { 
    unsigned char length; // Length of the message, excluding CR (BENQ_LEN_4)
    unsigned short id;
    unsigned char cmd_type;
    unsigned char CR;
    } BENQ_SetReturnPacket;

typedef struct BENQ_GetReturnPacket { 
    unsigned char length; // Length of the message, excluding CR (BENQ_LEN_8)
    unsigned short id;
    unsigned char cmd_type;
    unsigned char value1;
    unsigned char value2;
    unsigned char value3;
    unsigned char CR;
    } BENQ_GetReturnPacket;

typedef struct BENQ_NoIdSetPacket { 
    unsigned char length; // Length of the message, excluding CR (BENQ_LEN_5)
    unsigned char cmd_code;
    unsigned char value1;
    unsigned char value2;
    unsigned char value3;
    unsigned char CR;
    } BENQ_NoIdSetPacket;

void BENQ_power_on(BENQ_SendPacket*);
void BENQ_power_blank(BENQ_SendPacket*);
void BENQ_power_standby(BENQ_SendPacket*);

char BENQ_check_set_return(BENQ_SetReturnPacket*); // returns BENQ_SUCCESS or BENQ_FAILURE

#endif //__BENQ_H_