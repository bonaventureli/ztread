/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the license.txt file.
 */

#include "ikblesec.h"
#include "ble_srv_common.h"
#include "sdk_common.h"
#include "app_error.h"
#include "iksecuritydk.h"


/*****************************************************************************
* data handle
*****************************************************************************/
static data_info g_send_data = {NULL, 0, 0};
static data_info g_rcv_data = {NULL, 0, 0};
static 	ble_IngeekSec_t    m_IngeekSec;
/**@brief Function for handling the Connect event.
 *
 * @param[in] p_lbs      LED Button Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
static void on_connect(ble_IngeekSec_t * p_IngeekSec, ble_evt_t * p_ble_evt)
{
    p_IngeekSec->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


/**@brief Function for handling the Disconnect event.
 *
 * @param[in] p_lbs      LED Button Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
static void on_disconnect(ble_IngeekSec_t * p_IngeekSec, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_IngeekSec->conn_handle = BLE_CONN_HANDLE_INVALID;
}


/**@brief Function for handling the Write event.
 *
 * @param[in] p_lbs      LED Button Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
static void on_write(ble_IngeekSec_t * p_IngeekSec, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
		int chunk_size = 0;	
		if(g_rcv_data.offset == 0)
			{
				g_rcv_data.data = (uint8_t *)malloc(INGEEKSEC_INFO_LEN);	
			}	
		//info write handler
    if ((p_evt_write->handle == p_IngeekSec->info_char_handles.value_handle) &&	 
				(p_evt_write->len <= INGEEKSEC_INFO_LEN) && 
				(p_IngeekSec->info_write_handler != NULL))	
			{		
					chunk_size = p_evt_write->len > BLE_INGEEKSEC_MAX_DATA_LEN ? BLE_INGEEKSEC_MAX_DATA_LEN : p_evt_write->len;
					memcpy(g_rcv_data.data + g_rcv_data.offset, p_evt_write->data, chunk_size);
					g_rcv_data.offset += chunk_size;
				
					if(chunk_size < BLE_INGEEKSEC_MAX_DATA_LEN || g_rcv_data.offset == 80)
					{
							p_IngeekSec->info_write_handler(p_IngeekSec, g_rcv_data.data, g_rcv_data.offset);
							IngeekSec_data_free_func(g_rcv_data.data, g_rcv_data.offset);
							g_rcv_data.offset = 0;
					}	
			}
		//auth write handler
		if ((p_evt_write->handle == p_IngeekSec->auth_char_handles.value_handle) &&	 
				(p_evt_write->len <= INGEEKSEC_AUTH_LEN)	&&  
				(p_IngeekSec->auth_write_handler != NULL))
			{
					chunk_size = p_evt_write->len > BLE_INGEEKSEC_MAX_DATA_LEN ? BLE_INGEEKSEC_MAX_DATA_LEN : p_evt_write->len;
					memcpy(g_rcv_data.data + g_rcv_data.offset, p_evt_write->data, chunk_size);
					g_rcv_data.offset += chunk_size;
				
					if(chunk_size < BLE_INGEEKSEC_MAX_DATA_LEN || g_rcv_data.offset == 80)
					{
							p_IngeekSec->auth_write_handler(p_IngeekSec, g_rcv_data.data, g_rcv_data.offset);
							IngeekSec_data_free_func(g_rcv_data.data, g_rcv_data.offset);
							g_rcv_data.offset = 0;
					}
			}
		//session write handler
		if ((p_evt_write->handle == p_IngeekSec->session_char_handles.value_handle) && 
				(p_evt_write->len <= INGEEKSEC_SESSION_LEN)	&& 
				(p_IngeekSec->session_write_handler != NULL))
			{
					chunk_size = p_evt_write->len > BLE_INGEEKSEC_MAX_DATA_LEN ? BLE_INGEEKSEC_MAX_DATA_LEN : p_evt_write->len;
					memcpy(g_rcv_data.data + g_rcv_data.offset, p_evt_write->data, chunk_size);
					g_rcv_data.offset += chunk_size;
				
					if(chunk_size < BLE_INGEEKSEC_MAX_DATA_LEN)
					{
							p_IngeekSec->session_write_handler(p_IngeekSec, g_rcv_data.data, g_rcv_data.offset);
							IngeekSec_data_free_func(g_rcv_data.data, g_rcv_data.offset);
							g_rcv_data.offset = 0;
					}
			}
		//cmd write handler
		if ((p_evt_write->handle == p_IngeekSec->cmd_char_handles.value_handle) && 
				(p_evt_write->len <= INGEEKSEC_CMD_LEN) && 
				(p_IngeekSec->cmd_write_handler != NULL))
			{
					chunk_size = p_evt_write->len > BLE_INGEEKSEC_MAX_DATA_LEN ? BLE_INGEEKSEC_MAX_DATA_LEN : p_evt_write->len;
					memcpy(g_rcv_data.data + g_rcv_data.offset, p_evt_write->data, chunk_size);
					g_rcv_data.offset += chunk_size;
				
					if(chunk_size < BLE_INGEEKSEC_MAX_DATA_LEN || g_rcv_data.offset == 20)
					{
							p_IngeekSec->cmd_write_handler(p_IngeekSec, g_rcv_data.data, g_rcv_data.offset);
							IngeekSec_data_free_func(g_rcv_data.data, g_rcv_data.offset);
							g_rcv_data.offset = 0;
					}
			}
		//rssi write handler
		if ((p_evt_write->handle == p_IngeekSec->rssi_char_handles.value_handle) && 
        (p_evt_write->len <= INGEEKSEC_RSSI_LEN) &&
        (p_IngeekSec->rssi_write_handler != NULL))
			{
					chunk_size = p_evt_write->len > BLE_INGEEKSEC_MAX_DATA_LEN ? BLE_INGEEKSEC_MAX_DATA_LEN : p_evt_write->len;
					memcpy(g_rcv_data.data + g_rcv_data.offset, p_evt_write->data, chunk_size);
					g_rcv_data.offset += chunk_size;
				
					if(chunk_size < BLE_INGEEKSEC_MAX_DATA_LEN)
					{
							p_IngeekSec->rssi_write_handler(p_IngeekSec, g_rcv_data.data, g_rcv_data.offset);
							IngeekSec_data_free_func(g_rcv_data.data, g_rcv_data.offset);
							g_rcv_data.offset = 0;
					}
			}		
}


void ble_IngeekSec_on_ble_evt(ble_IngeekSec_t * p_IngeekSec, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_IngeekSec, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_IngeekSec, p_ble_evt);
            break;
            
        case BLE_GATTS_EVT_WRITE:
            on_write(p_IngeekSec, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}

static void info_write_handler(ble_IngeekSec_t * p_IngeekSec, uint8_t * infoValue, uint8_t recInfoLen)
{
		uint32_t err_code = 100000;
		
		if (recInfoLen > 0)
		{
		    err_code = ikSecurityDKHandleCharacter(p_IngeekSec, INGEEKSEC_UUID_INFO_CHAR, infoValue, recInfoLen);
            if (err_code != NRF_SUCCESS &&
                err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
                err_code != NRF_ERROR_INVALID_STATE)
            {	
                APP_ERROR_CHECK(err_code);
            }
		}			
}

static void auth_write_handler(ble_IngeekSec_t * p_IngeekSec, uint8_t * authValue, uint16_t recAuthLen)
{
		uint32_t err_code = 10000;
	
		if (recAuthLen)
		{
             err_code = ikSecurityDKHandleCharacter(p_IngeekSec, INGEEKSEC_UUID_AUTH_CHAR, authValue, recAuthLen);
            if (err_code != NRF_SUCCESS &&
                err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
                err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
		}	
}

static void session_write_handler(ble_IngeekSec_t * p_IngeekSec, uint8_t * sessionValue, uint16_t recSessionLen)
{
		uint32_t err_code = 100000;
	
		if (recSessionLen)
		{
		     err_code = ikSecurityDKHandleCharacter(p_IngeekSec, INGEEKSEC_UUID_SESSION_CHAR, sessionValue, recSessionLen);
            if (err_code != NRF_SUCCESS &&
                err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
                err_code != NRF_ERROR_INVALID_STATE)
            {	
                APP_ERROR_CHECK(err_code);
            }
		}	
}

static void cmd_write_handler(ble_IngeekSec_t * p_IngeekSec, uint8_t * cmdValue, uint16_t recCmdLen)
{
		uint32_t err_code = 10000;
	
		if (recCmdLen)
		{
            err_code = ikSecurityDKHandleCharacter(p_IngeekSec, INGEEKSEC_UUID_CMD_CHAR, cmdValue, recCmdLen);
            if (err_code != NRF_SUCCESS &&
                err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
                err_code != NRF_ERROR_INVALID_STATE)
            {	
                APP_ERROR_CHECK(err_code);
            }
		}	
}

static void rssi_write_handler(ble_IngeekSec_t * p_IngeekSec, uint8_t * rssiValue, uint16_t recRssiLen)
{
		uint32_t err_code = 10000;
	
		if (recRssiLen)
		{
            err_code = ikSecurityDKHandleCharacter(p_IngeekSec, INGEEKSEC_UUID_RSSI_CHAR, rssiValue, recRssiLen);
            if (err_code != NRF_SUCCESS &&
                err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
                err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
		}	
}
/**@brief Function for adding the LED Characteristic.
 *
 * @param[in] p_lbs      LED Button Service structure.
 * @param[in] p_lbs_init LED Button Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t info_char_add(ble_IngeekSec_t * p_IngeekSec, const ble_IngeekSec_init_t * p_IngeekSec_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
		uint8_t info = 0x00;
    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;
	
    ble_uuid.type = p_IngeekSec->uuid_type;
    ble_uuid.uuid = INGEEKSEC_UUID_INFO_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = INGEEKSEC_INFO_LEN;
    attr_char_value.p_value      = &info;

    return sd_ble_gatts_characteristic_add(p_IngeekSec->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_IngeekSec->info_char_handles);
}


/**@brief Function for adding the Button Characteristic.
 *
 * @param[in] p_lbs      LED Button Service structure.
 * @param[in] p_lbs_init LED Button Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t status_char_add(ble_IngeekSec_t * p_IngeekSec, const ble_IngeekSec_init_t * p_IngeekSec_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
		uint8_t status = 0xFF;
    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    
    memset(&char_md, 0, sizeof(char_md));
    
    char_md.char_props.read   = 1;
    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_IngeekSec->uuid_type;
    ble_uuid.uuid = INGEEKSEC_UUID_STATUS_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint8_t);
    attr_char_value.p_value      = &status;

    return sd_ble_gatts_characteristic_add(p_IngeekSec->service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_IngeekSec->status_char_handles);
}

/**@brief Function for adding the LED Characteristic.
 *
 * @param[in] p_lbs      LED Button Service structure.
 * @param[in] p_lbs_init LED Button Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t auth_char_add(ble_IngeekSec_t * p_IngeekSec, const ble_IngeekSec_init_t * p_IngeekSec_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
		uint8_t auth = 0x00;
    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_IngeekSec->uuid_type;
    ble_uuid.uuid = INGEEKSEC_UUID_AUTH_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = INGEEKSEC_AUTH_LEN;
    attr_char_value.p_value      = &auth;

    return sd_ble_gatts_characteristic_add(p_IngeekSec->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_IngeekSec->auth_char_handles);
}

/**@brief Function for adding the LED Characteristic.
 *
 * @param[in] p_lbs      LED Button Service structure.
 * @param[in] p_lbs_init LED Button Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t session_char_add(ble_IngeekSec_t * p_IngeekSec, const ble_IngeekSec_init_t * p_IngeekSec_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
		uint8_t session = 0x00;
    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_IngeekSec->uuid_type;
    ble_uuid.uuid = INGEEKSEC_UUID_SESSION_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = INGEEKSEC_SESSION_LEN;
    attr_char_value.p_value      = &session;

    return sd_ble_gatts_characteristic_add(p_IngeekSec->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_IngeekSec->session_char_handles);
}

/**@brief Function for adding the LED Characteristic.
 *
 * @param[in] p_lbs      LED Button Service structure.
 * @param[in] p_lbs_init LED Button Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t cmd_char_add(ble_IngeekSec_t * p_IngeekSec, const ble_IngeekSec_init_t * p_IngeekSec_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
		uint8_t cmd = 0x00;
    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_IngeekSec->uuid_type;
    ble_uuid.uuid = INGEEKSEC_UUID_CMD_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = INGEEKSEC_CMD_LEN;
    attr_char_value.p_value      = &cmd;

    return sd_ble_gatts_characteristic_add(p_IngeekSec->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_IngeekSec->cmd_char_handles);
}

/**@brief Function for adding the LED Characteristic.
 *
 * @param[in] p_lbs      LED Button Service structure.
 * @param[in] p_lbs_init LED Button Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t rssi_char_add(ble_IngeekSec_t * p_IngeekSec, const ble_IngeekSec_init_t * p_IngeekSec_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
		uint8_t rssi = 0x00;
    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_IngeekSec->uuid_type;
    ble_uuid.uuid = INGEEKSEC_UUID_RSSI_CHAR;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = INGEEKSEC_RSSI_LEN;
    attr_char_value.p_value      = &rssi;

    return sd_ble_gatts_characteristic_add(p_IngeekSec->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_IngeekSec->rssi_char_handles);
}


uint32_t ble_IngeekSec_init(ble_IngeekSec_t * p_IngeekSec, const ble_IngeekSec_init_t * p_IngeekSec_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure.
    p_IngeekSec->conn_handle       = BLE_CONN_HANDLE_INVALID;
    p_IngeekSec->info_write_handler = p_IngeekSec_init->info_write_handler;
		p_IngeekSec->auth_write_handler = p_IngeekSec_init->auth_write_handler;
		p_IngeekSec->session_write_handler = p_IngeekSec_init->session_write_handler;
		p_IngeekSec->cmd_write_handler = p_IngeekSec_init->cmd_write_handler;
		p_IngeekSec->rssi_write_handler = p_IngeekSec_init->rssi_write_handler;

    // Add service.
    ble_uuid128_t base_uuid = {INGEEKSEC_UUID_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_IngeekSec->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_IngeekSec->uuid_type;
    ble_uuid.uuid = INGEEKSEC_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_IngeekSec->service_handle);
    VERIFY_SUCCESS(err_code);

    // Add characteristics.
    err_code = status_char_add(p_IngeekSec, p_IngeekSec_init);
    VERIFY_SUCCESS(err_code);
		//info characteristics
    err_code = info_char_add(p_IngeekSec, p_IngeekSec_init);
    VERIFY_SUCCESS(err_code);
		//auth	characteristics
		err_code = auth_char_add(p_IngeekSec, p_IngeekSec_init);
    VERIFY_SUCCESS(err_code);
		//session	characteristics
		err_code = session_char_add(p_IngeekSec, p_IngeekSec_init);
    VERIFY_SUCCESS(err_code);
		//cmd	characteristics
		err_code = cmd_char_add(p_IngeekSec, p_IngeekSec_init);
    VERIFY_SUCCESS(err_code);
		//rssi	characteristics
		err_code = rssi_char_add(p_IngeekSec, p_IngeekSec_init);
    VERIFY_SUCCESS(err_code);
		
    return NRF_SUCCESS;
}

uint32_t ble_IngeekSec_on_status_change(ble_IngeekSec_t * p_IngeekSec, uint8_t statusValue)
{
    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(statusValue);
    
    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_IngeekSec->status_char_handles.value_handle;
    params.p_data = &statusValue;
    params.p_len = &len;
    
    return sd_ble_gatts_hvx(p_IngeekSec->conn_handle, &params);
}

//读回调接口，用来向char里面set值
uint32_t ble_IngeekSec_char_set(ble_IngeekSec_t * p_IngeekSec, uint16_t uuid, uint8_t * showValues, uint16_t valueLen)
{
		ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, valueLen);

    gatts_value.len     = valueLen;
    gatts_value.offset  = 0;
    gatts_value.p_value = showValues;
		
		switch (uuid)
		{
			case	INGEEKSEC_UUID_INFO_CHAR:
				return sd_ble_gatts_value_set(p_IngeekSec->conn_handle, p_IngeekSec->info_char_handles.value_handle, &gatts_value);
				break;
			case	INGEEKSEC_UUID_AUTH_CHAR:
				return sd_ble_gatts_value_set(p_IngeekSec->conn_handle, p_IngeekSec->auth_char_handles.value_handle, &gatts_value);
				break;
			case	INGEEKSEC_UUID_SESSION_CHAR:
				return sd_ble_gatts_value_set(p_IngeekSec->conn_handle, p_IngeekSec->session_char_handles.value_handle, &gatts_value);
				break;
			case	INGEEKSEC_UUID_CMD_CHAR:
				return sd_ble_gatts_value_set(p_IngeekSec->conn_handle, p_IngeekSec->cmd_char_handles.value_handle, &gatts_value);
				break;
			case	INGEEKSEC_UUID_RSSI_CHAR:
				return sd_ble_gatts_value_set(p_IngeekSec->conn_handle, p_IngeekSec->rssi_char_handles.value_handle, &gatts_value);
				break;
			default:
				break;
		}	
}

void IngeekSec_data_free_func(uint8_t *data, uint32_t len)
{
	if(data)
		free(data);
		data = NULL;
}



void digitalkey_services_init(void)
{
    // YOUR_JOB: Add code to initialize the services used by the application.
    uint32_t                           err_code;
    ble_IngeekSec_init_t               m_IngeekSec_init;
    // Initialize XXX Service.
    memset(&m_IngeekSec_init, 0, sizeof(m_IngeekSec_init));
		
    m_IngeekSec_init.info_write_handler = info_write_handler;
		m_IngeekSec_init.auth_write_handler = auth_write_handler;
		m_IngeekSec_init.session_write_handler = session_write_handler;
		m_IngeekSec_init.cmd_write_handler = cmd_write_handler;
		m_IngeekSec_init.rssi_write_handler = rssi_write_handler;
   
    err_code = ble_IngeekSec_init(&m_IngeekSec, &m_IngeekSec_init);
    //APP_ERROR_CHECK(err_code);
	
    
}