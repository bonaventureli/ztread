/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by ingeek Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.ingeek.com
*
* Copyright (C) 2018 Ingeek Information Security Consulting Associates Co. All rights reserved.
***********************************************************************************************************************/

/*! \file Ikif.h
 *  \author 
 *  \brief DKAPI INTERFACE
 */
 
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 26.10.2018 1.00     First Release
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

#ifndef IKTK_API_H
#define IKTK_API_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

/*!
 *  \struct eDKRetCode_t
 *  \brief
 */
typedef enum {
   DK_SRV_FAILURE = 0, //!< failue
   DK_SRV_SUCESS,      //!< success
   DK_PACK_SUCCESS,    //!< pack type error 
   DK_PACK_TYPE_ERROR, //!< pack type error 
   DK_PACK_CRC_ERROR,  //!< pack length bit data error
   DK_IO_ABNORMAL,     //!< IO abnormal
}dkRetCode_t;


/*!
 *  \struct eDKCommType_t
 *  \brief
 */
typedef enum {
   DK_COMM_UART = 1, //!< UART
   DK_COMM_ETHERNET, //!< ETHERNET
   DK_COMM_FLASH,    //!< FLASH
}dkCommType_t;



/*!
 * typedef int (*pfnReadPortCB_t)(eDKPORT port,uint8_t *outBuf, uint32_t size,uint32_t offset)
 * read content from flash/uart
 * @param [out] outBuf: read flash or uart content and write to outBuf
 * @param [in] size: outBuf size .
 * @param [in] offset:  read flash offset position
 * @return 	actual read size from port.
 * Example usage:
 * @verbatim
 * @endverbatim
 */
typedef int (*pfnReadPortCB_t)(uint8_t *outBuf, uint32_t size,uint32_t offset);
/*!
 * typedef int (*pfnWritePortCB_t)(const uint8_t *inBuf, uint32_t size,uint32_t offset)
 * write content to flash or uart
 * @param [in] inBuf: content will be writed to flash or uart.
 * @param [in] size: inBuf size .
 * @param [in] offset:  write to flash offset position
 * @return actual write size to port.
 * Example usage:
 * @verbatim
 * @endverbatim
 */
typedef int (*pfnWritePortCB_t)(const uint8_t *inBuf, uint32_t size,uint32_t offset);

/*!
 * typedef int (*pfnRandomGenerateCB_t)(uint8_t *outBuffer, uint32_t inSize)
 * write content to flash or uart
 * @param [in] outBuffer: random will write to output buffer.
 * @param [in] inSize: request random length.
 * @return actual random size length.
 * Example usage:
 * @verbatim
 * @endverbatim
 */
typedef int (*pfnRandomGenerateCB_t)(void *seed,unsigned char *outBuffer, uint32_t inSize);

/*!
 * typedef int (*pfnNotifyCB_t)(uint_t SRVID,eDKSRVStatus reason)
 * when digital key service start end ,it will call pfnNotifyCB_t
 * @param [in] SRVID
 * @param [in] reason .
 * @return
 * Example usage:
 * @verbatim
 * @endverbatim
 */
typedef int (*pfnNotifyCB_t)(uint32_t uiSRVID,dkRetCode_t reason);

/*!
 *  \struct dkIOs_t
 *  \brief 
 */
typedef struct
{
   pfnWritePortCB_t     uartTxCallback; //write uart callback funtion point
   pfnReadPortCB_t      flashRCallback; //read flash callback funtion point
   pfnWritePortCB_t     flashWCallback; //write flash callback funtion point
} dkIOs_t;


/*!
 * uint_t DKCreateSRV(dkSession_t * pSession )
 * @brief creates a new services instance, and returns a services handle
 * @param [in] pdkio , User Must Initializing function pointer in structure .
 * @return If create success, return a non zero services id.
 * Example usage:
   <pre>
    //demo one.
    #include "ikif.h"
    dkIOs_t dkios = {0,0,0,0,0,0};
    uint32_t dkid = 0;

    dkid = DKCreateSRV(&dkios);
    if( dkid > 0 )
    {
      // start Digital Services 
       DKStartSRV(dkid,0); 
    }
#############################################################################################
    //demo two.
    #include "ikif.h"

     int pfnWriteUartPort(const uint8_t *inBuf, uint32_t size,uint32_t offset)
     {
         return 1;
     }

     int pfnWriteflashPort(const uint8_t *outBuf, uint32_t size,uint32_t offset)
     {
         return 1;
     }

     int pfnReadflashPort(uint8_t *outBuf, uint32_t size,uint32_t offset)
     {
         return 1;
     }
    int random_vector_generate(void * seed ,unsigned char * p_buff, uint32_t size)
   {

   }
    dkIOs_t dkios = {0,pfnWriteUartPort,pfnReadflashPort,pfnWriteflashPort,random_vector_generate};
    uint32_t dkid = 0;

    dkid = DKCreateSRV(&dkios);
    if( dkid > 0 )
    {
      // start Digital Services 
       DKStartSRV(dkid,0); 
    }
   
</pre>
 * @verbatim
 * @endverbatim
 */

extern uint32_t DKCreateSRV(dkIOs_t * pdkio);

/*!
 * eDKStatus DKStartSRV(uint_t SRVID,pfnNotifyCB_t pfnNotify)
 * @brief Start a service with valid srv ID
 * @param [in] SRVID:  valid services ID .
 * @param [in] pfnNotify, when start services end, client will call pfnNotify to notify services
 * @return returned a dkRetCode_t type
  * Example usage:
   <pre>
    //Declare a variable .
    dkIOs_t dkios = {0,0,0,0};
    uint32_t dkid = 0;
    // Attempt to create Digital Services group.
    dkid = DKCreateSRV(&dkios);
    if( dkid > 0 )
    {
      // start Digital Services 
       DKStartSRV(dkid,0); 
    }
   </pre>
 * @verbatim
 * @endverbatim
 */

extern dkRetCode_t DKStartSRV(uint32_t SRVID,pfnNotifyCB_t pfnNotify);

/*!
 * eDKStatus DKStopSRV(uint_t session )
 * @brief stop a services with valid ID
 * @param [in] SRVID , valid services ID .
 * @return returned a dkRetCode_t type
 * @verbatim
 * @endverbatim
 */

extern dkRetCode_t DKStopSRV(uint32_t SRVID );


/*!
 * dkRetCode_t DKCommDataNotify(uint32_t inType, const uint8_t *inData,uint32_t inSize )
 * @brief domain send data to dk lib
 * @param [in] inType , dkCommType_t
 * @param [in] inData , buffer  .
 * @param [in] inSize , buffer size  .
 * @return returned a dkRetCode_t type
 * @verbatim
 * @endverbatim
 */

extern dkRetCode_t DKCommDataNotify(dkCommType_t inType, const uint8_t *inData,uint32_t inSize );


#ifdef __cplusplus
}
#endif


#endif/* IKTK_API_H */
