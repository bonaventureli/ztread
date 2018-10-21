
/** @file
 *
 * @defgroup ble_sdk_srv_lbs LED Button Service Server
 * @{
 * @ingroup ble_sdk_srv
 *
 * @brief LED Button Service Server module.
 *
 * @details This module implements a custom LED Button Service with an LED and Button Characteristics.
 *          During initialization, the module adds the LED Button Service and Characteristics
 *          to the BLE stack database.
 *
 *          The application must supply an event handler for receiving LED Button Service
 *          events. Using this handler, the service notifies the application when the
 *          LED value changes.
 *
 *          The service also provides a function for letting the application notify
 *          the state of the Button Characteristic to connected peers.
 *
 * @note The application must propagate BLE stack events to the LED Button Service
 *       module by calling ble_lbs_on_ble_evt() from the @ref softdevice_handler callback.
*/

#ifndef BLE_INGEEKSEC_H__
#define BLE_INGEEKSEC_H__

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define GATT_MTU_SIZE_DEFAULT 10

//完整版UUID
#define INGEEKSEC_UUID_BASE        {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0xF0, 0xFF, \
                              0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}

#define INGEEKSEC_UUID_SERVICE     	0xFFF0			//基础服务UUID
#define INGEEKSEC_UUID_STATUS_CHAR 	0xFFF1			//状态UUID
#define INGEEKSEC_UUID_INFO_CHAR    0xFFF2			//信息UUID
#define INGEEKSEC_UUID_AUTH_CHAR		0xFFF3			//授权UUID
#define INGEEKSEC_UUID_SESSION_CHAR 0xFFF4			//会话UUID
#define INGEEKSEC_UUID_CMD_CHAR			0xFFF5			//命令UUID
#define INGEEKSEC_UUID_RSSI_CHAR		0xFFF6			//RSSIUUID
															
//特征值数据通信数据长度
#define INGEEKSEC_INFO_LEN					128					//info len
#define INGEEKSEC_AUTH_LEN					128					//auth len
#define INGEEKSEC_SESSION_LEN				128					//session len
#define INGEEKSEC_CMD_LEN						20					//cmd len
#define INGEEKSEC_RSSI_LEN					5					//rssi len																
#define BLE_INGEEKSEC_MAX_DATA_LEN	(GATT_MTU_SIZE_DEFAULT - 3)					//	(GATT_MTU_SIZE_DEFAULT - 3)
// Forward declaration of the ble_lbs_t type. 
typedef struct ble_IngeekSec_s ble_IngeekSec_t;

typedef void (*ble_IngeekSec_info_write_handler_t) 		(ble_IngeekSec_t * p_IngeekSec, uint8_t * infoValue, uint16_t recInfoLen);
typedef void (*ble_IngeekSec_auth_write_handler_t) 		(ble_IngeekSec_t * p_IngeekSec, uint8_t * authValue, uint16_t recAuthLen);
typedef void (*ble_IngeekSec_session_write_handler_t) (ble_IngeekSec_t * p_IngeekSec, uint8_t * sessionValue, uint16_t recSessionLen);
typedef void (*ble_IngeekSec_cmd_write_handler_t) 		(ble_IngeekSec_t * p_IngeekSec, uint8_t * cmdValue, uint16_t recCmdLen);	
typedef void (*ble_IngeekSec_rssi_write_handler_t) 		(ble_IngeekSec_t * p_IngeekSec, uint8_t * rssiValue, uint16_t recRssiLen);															

/** @brief LED Button Service init structure. This structure contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
    ble_IngeekSec_info_write_handler_t info_write_handler; /**< Event handler to be called when the LED Characteristic is written. */
		ble_IngeekSec_auth_write_handler_t auth_write_handler;
		ble_IngeekSec_session_write_handler_t session_write_handler;
		ble_IngeekSec_cmd_write_handler_t cmd_write_handler;
		ble_IngeekSec_rssi_write_handler_t rssi_write_handler;
} ble_IngeekSec_init_t;

/**@brief LED Button Service structure. This structure contains various status information for the service. */
struct ble_IngeekSec_s
{
    uint16_t                    service_handle;      /**< Handle of LED Button Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t    status_char_handles;    /**< Handles related to the LED Characteristic. */
    ble_gatts_char_handles_t    info_char_handles; /**< Handles related to the Button Characteristic. */
		ble_gatts_char_handles_t		auth_char_handles;
		ble_gatts_char_handles_t		session_char_handles;
		ble_gatts_char_handles_t		cmd_char_handles;
		ble_gatts_char_handles_t		rssi_char_handles;
    uint8_t                     uuid_type;           /**< UUID type for the LED Button Service. */
    uint16_t                    conn_handle;         /**< Handle of the current connection (as provided by the BLE stack). BLE_CONN_HANDLE_INVALID if not in a connection. */
    ble_IngeekSec_info_write_handler_t info_write_handler;   /**< Event handler to be called when the LED Characteristic is written. */
		ble_IngeekSec_auth_write_handler_t auth_write_handler;
		ble_IngeekSec_session_write_handler_t session_write_handler;
		ble_IngeekSec_cmd_write_handler_t cmd_write_handler;
		ble_IngeekSec_rssi_write_handler_t rssi_write_handler;
};


typedef struct
{
		uint8_t *data;
		uint16_t len;
		uint16_t offset;
} data_info;

/**@brief Function for initializing the LED Button Service.
 *
 * @param[out] p_lbs      LED Button Service structure. This structure must be supplied by
 *                        the application. It is initialized by this function and will later
 *                        be used to identify this particular service instance.
 * @param[in] p_lbs_init  Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS If the service was initialized successfully. Otherwise, an error code is returned.
 */
//uint32_t ble_IngeekSec_init(ble_IngeekSec_t * p_IngeekSec, const ble_IngeekSec_init_t * p_IngeekSec_init);
void digitalkey_services_init(void);
/**@brief Function for handling the application's BLE stack events.
 *
 * @details This function handles all events from the BLE stack that are of interest to the LED Button Service.
 *
 * @param[in] p_lbs      LED Button Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
void ble_IngeekSec_on_ble_evt(ble_IngeekSec_t * p_IngeekSec, ble_evt_t * p_ble_evt);

/**@brief Function for sending a button state notification.
 *
 * @param[in] p_lbs      LED Button Service structure.
 * @param[in] button_state  New button state.
 *
 * @retval NRF_SUCCESS If the notification was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_IngeekSec_on_status_change(ble_IngeekSec_t * p_IngeekSec, uint8_t statusValue);

/**@brief Function for setting the Body Sensor Location.
 *
 * @details Sets a new value of the Body Sensor Location characteristic. The new value will be sent
 *          to the client the next time the client reads the Body Sensor Location characteristic.
 *
 * @param[in]   p_hrs                 Heart Rate Service structure.
 * @param[in]   body_sensor_location  New Body Sensor Location.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_IngeekSec_char_set(ble_IngeekSec_t * p_IngeekSec, uint16_t uuid, uint8_t * showValues, uint16_t valueLen);

void IngeekSec_data_free_func(uint8_t *data, uint32_t len);
#endif // BLE_LBS_H__

/** @} */
