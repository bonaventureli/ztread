/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup ble_sdk_uart_over_ble_main main.c
 * @{
 * @ingroup  ble_sdk_app_nus_eval
 * @brief    UART over BLE application main file.
 *
 * This file contains the source code for a sample application that uses the Nordic UART service.
 * This application uses the @ref srvlib_conn_params module.
 */

#include <stdint.h>
#include <string.h>
#include "ikcmdif.h"
#include "ikcmdhandle.h"

extern void MslCarcmd(uint8_t cmd,uint8_t param);

pfnWritePortCB_t   g_uartTxCB = 0;


int ikcmdUartSend(uint8_t *inBuf ,uint32_t size)
{
    if (g_uartTxCB)
        g_uartTxCB(inBuf,size,0);
   return 1;
}

int ikcmdInitialize(pfnWritePortCB_t inUartTx)
{
    g_uartTxCB = inUartTx;
	  return 0;
}

dkRetCode_t ikcmdUartPortRxNotify(const uint8_t * inBuf,uint32_t inSize)
{
   MslPeriodTask(inBuf,inSize);
   return DK_PACK_SUCCESS;
}

int  ikcmdStart(void)
{
	
}



void ikcmdSendUart(uint8_t cmd)
{
	switch (cmd){
		case 0:{
			MslCarcmd(MONITOR_CMD_POSITION_MODE,CARCMD_PARAM_0);
		break;
		}
		case 1:{
			MslCarcmd(MONITOR_CMD_POSITION_MODE,CARCMD_PARAM_0);
		break;
		}
		case 2:{
			MslCarcmd(MONITOR_CMD_POSITION_MODE,CARCMD_PARAM_0);
		break;
		}
		case 3:{
			MslCarcmd(MONITOR_CMD_POSITION_MODE,CARCMD_PARAM_0);
		break;
		}
		case 4:{
			MslCarcmd(MONITOR_CMD_POSITION_MODE,CARCMD_PARAM_0);
		break;
		}
		default: break;
	}
}

/** 
 * @}
 */
