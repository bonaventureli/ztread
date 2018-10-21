#ifndef SECURITY_DIGITAL_KEY_H
#define SECURITY_DIGITAL_KEY_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "ikblesec.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern uint32_t ikSecurityDKHandleCharacter(ble_IngeekSec_t * p_IngeekSec, uint16_t uuid, uint8_t *newValue,uint32_t returnBytes);
extern void ikSecurityDKCheckCarinfo(void);

#ifdef __cplusplus
}
#endif

#endif
