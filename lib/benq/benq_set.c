#include "benq.h"

void _BENQ_set_init(BENQ_SendPacket *sp)
{
    sp->length = BENQ_LEN_8;
    sp->cmd_type = BENQ_CMD_SET;
    sp->CR = BENQ_CR;
}

void _BENQ_power_init(BENQ_SendPacket *sp)
{
    _BENQ_set_init(sp);
    sp->cmd_code = 0x21;
}

void BENQ_power_on(BENQ_SendPacket *sp)
{
    _BENQ_power_init(sp);
    sp->value1 = '0';
    sp->value2 = '0';
    sp->value3 = '1';
}

void BENQ_power_blank(BENQ_SendPacket *sp)
{
    _BENQ_power_init(sp);
    sp->value1 = '0';
    sp->value2 = '0';
    sp->value3 = '0';
}

void BENQ_power_standby(BENQ_SendPacket *sp)
{
    _BENQ_power_init(sp);
    sp->value1 = '0';
    sp->value2 = '0';
    sp->value3 = '2';
}


char BENQ_check_set_return(BENQ_SetReturnPacket *rp)
{
    if (rp->length == BENQ_LEN_4 && 
        rp->cmd_type == BENQ_CMD_VALID)
    {
        return BENQ_SUCCESS;
    }

    return BENQ_FAILURE;
}