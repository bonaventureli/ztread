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

/*! \file ikbleif.h
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

#ifndef IK_BLE_IF_H
#define IK_BLE_IF_H

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * int ikbleInitialize(void)
 * @brief Initialize BLE status
 * @param [in] 
 * @return zero is success
 * @verbatim
 * @endverbatim
 */

extern int ikbleInitialize(void);

/*!
 * int  ikbleStart(void)
 * @brief start ble services
 * @param [in] 
 * @return zero is success
 * @verbatim
 * @endverbatim
 */
 
extern int  ikbleStart(void);



#ifdef __cplusplus
}
#endif


#endif/* IK_BLE_IF_H */
