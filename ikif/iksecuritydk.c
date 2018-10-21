#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "ikif.h"
#include "ikbleif.h"
#include "ikcmdif.h"

#include "digital_key_api.h"
#include "iksecuritydk.h"


#include "nrf.h"
#include "app_error.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_bas.h"
#include "ble_hrs.h"
#include "ble_dis.h"
#include "ble_conn_params.h"
#include "sensorsim.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_freertos.h"
#include "app_timer.h"
#include "peer_manager.h"
#include "peer_manager_handler.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "fds.h"
#include "ble_conn_state.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"


#define SESSIONMAXBIT 112
#define INFOCHARMAXBIT 29

uint32_t ikSecurityDKHandleCharacter(ble_IngeekSec_t * p_IngeekSec, uint16_t uuid, uint8_t *newValue, uint32_t returnBytes)
 {
	 uint32_t err_code = NRF_SUCCESS;
	 
     switch(uuid)
     {
         case INGEEKSEC_UUID_INFO_CHAR:
         {
            unsigned int outlen;
            uint8_t preply_data[150];
					 
            if(ingeek_push_info(newValue, returnBytes) != INGEEK_OK)
               return NRF_ERROR_INTERNAL;
								
            if(ingeek_get_sec_status() != CARINFO_VALID)
               return NRF_ERROR_INTERNAL;

            if(ingeek_pull_info(preply_data, &outlen) != INGEEK_OK )
               return NRF_ERROR_INTERNAL;
									
            if(outlen != INFOCHARMAXBIT)
               return NRF_ERROR_INTERNAL;
									
            if(ingeek_get_sec_status() != READ_INFO)
               return NRF_ERROR_INTERNAL;
								
            err_code = ble_IngeekSec_char_set(p_IngeekSec, INGEEKSEC_UUID_INFO_CHAR, preply_data, outlen);
            //APP_ERROR_CHECK(err_code);			
							break;
         }
         case INGEEKSEC_UUID_AUTH_CHAR:
        {
             if(ingeek_get_sec_status() != READ_INFO) 
               return NRF_ERROR_INTERNAL;

             if(ingeek_push_auth(newValue, returnBytes, (unsigned char*)1, (unsigned int*)1) != INGEEK_OK)
                return NRF_ERROR_INTERNAL;
					
              err_code = ble_IngeekSec_on_status_change(p_IngeekSec,0x02);
						break;
        }
        case INGEEKSEC_UUID_SESSION_CHAR:
        {
            unsigned int outlen;
            uint8_t preply_data[200];
            if(ingeek_get_sec_status() != WRITE_AUTH)
               return NRF_ERROR_INTERNAL;
							
            ingeek_push_session(newValue, returnBytes, preply_data, &outlen);
							
            if(outlen != SESSIONMAXBIT || ingeek_get_sec_status() != WRITE_SESSION)
              return NRF_ERROR_INTERNAL;

            err_code = ble_IngeekSec_on_status_change(p_IngeekSec,WRITE_SESSION);
            //APP_ERROR_CHECK(err_code);
            err_code = ble_IngeekSec_char_set(p_IngeekSec, INGEEKSEC_UUID_SESSION_CHAR, preply_data, outlen);
            //APP_ERROR_CHECK(err_code);
						break;
         }
         case INGEEKSEC_UUID_CMD_CHAR:
         {
            uint8_t preply_data[200];
            unsigned int outlen;
            DK_Cmd_Meg struct_cmd;
            uint8_t cmd;
					
            if(ingeek_command_input_action(newValue, returnBytes, &struct_cmd) != 0x0000)
                 return NRF_ERROR_INTERNAL;
							
            cmd = (uint8_t)(struct_cmd.command);
            ikcmdSendUart(cmd);								

            if(ingeek_command_output_action(&struct_cmd,preply_data, &outlen) != INGEEK_OK)
               return NRF_ERROR_INTERNAL;
							
            if(outlen != 16)
               return NRF_ERROR_INTERNAL;
							
            err_code = ble_IngeekSec_char_set(p_IngeekSec, INGEEKSEC_UUID_CMD_CHAR, preply_data, outlen);
             break;
         }
         case INGEEKSEC_UUID_RSSI_CHAR:
              break;
         default:
           // should not reach here!
           break;
   }
		 
   return err_code;
}

void Check_Carinfo(void)
{
  
}

/*********************************************************************
*********************************************************************/
