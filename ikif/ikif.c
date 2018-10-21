/**
  ********************************************************************************
  * @file    Ikdk.c
  * @author  Application Team
  * @version V1.0.0
  * @date    30-September-2018
  * @brief   This file contains all the functions for the dk api
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 Ingeek</center></h2>
  *
  * Licensed under Ingeek SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "ikif.h"
#include "ikbleif.h"
#include "digital_key_api.h"
#include "ikcmdif.h"

extern int ikCreateOSTask(void);

#define SESSION_MAX 1

dkIOs_t IKTK_SessionContain[SESSION_MAX] ={0};


/** @brief Function for getting vector of random numbers.
 *
 * @param[out] p_buff                               Pointer to unit8_t buffer for storing the bytes.
 * @param[in]  length                               Number of bytes to take from pool and place in p_buff.
 *
 * @retval     Number of bytes actually placed in p_buff.
 */ 
static int ikif_random_vector_generate(void * seed ,unsigned char * p_buff, uint32_t size)
{
    uint8_t available;
    uint32_t err_code;
    err_code = nrf_drv_rng_bytes_available(&available);
    //APP_ERROR_CHECK(err_code);
    uint8_t length = (size<available) ? size : available;
    err_code = nrf_drv_rng_rand(p_buff,length);
    //APP_ERROR_CHECK(err_code);
    return length;
}


uint32_t DKCreateSRV(dkIOs_t * pSession)
{
    static uint32_t session_id = 0;
    ikbleInitialize();
	  ingeek_set_callback(pSession->flashRCallback,pSession->flashWCallback,ikif_random_vector_generate);
    
  if (pSession && session_id < SESSION_MAX) {
      ++session_id;
      IKTK_SessionContain[session_id - 1] = *pSession;
      ikcmdInitialize(pSession->uartTxCallback);
      ingeek_set_callback(pSession->flashRCallback,pSession->flashWCallback,ikif_random_vector_generate);
		  ingeek_se_init();
      return session_id;
  } else {
    return 0;
  }
}

dkRetCode_t DKStartSRV(uint32_t SRVID,pfnNotifyCB_t pfnNotify){
	
	 ikCreateOSTask();
  return DK_SRV_SUCESS;

}

dkRetCode_t DKStopSRV(uint32_t SRVID ){

    if ( SRVID > 0 && SRVID < SESSION_MAX ) {

  } else {
    return DK_SRV_SUCESS;
  }

  return DK_SRV_SUCESS;
}


dkRetCode_t DKCommDataNotify(dkCommType_t inType, const uint8_t *inData,uint32_t inSize )
{
  return ikcmdUartPortRxNotify(inData,inSize);

}