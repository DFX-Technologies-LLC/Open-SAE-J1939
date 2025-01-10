/*
 * Transport_Layer.h
 *
 *  Created on: 14 juli 2021
 *      Author: Daniel Mårtensson
 */

#ifndef SAE_J1939_21_TRANSPORT_LAYER_SAE_J1939_21_TRANSPORT_LAYER_H_
#define SAE_J1939_21_TRANSPORT_LAYER_SAE_J1939_21_TRANSPORT_LAYER_H_

/* Enums and structs */
#include "../../Open_SAE_J1939/Structs.h"
#include "../SAE_J1939_Enums/Enum_Control_Byte.h"
#include "../SAE_J1939_Enums/Enum_DM14_DM15.h"
#include "../SAE_J1939_Enums/Enum_DM1_DM2.h"
#include "../SAE_J1939_Enums/Enum_Group_Function_Value.h"
#include "../SAE_J1939_Enums/Enum_NAME.h"
#include "../SAE_J1939_Enums/Enum_PGN.h"
#include "../SAE_J1939_Enums/Enum_Send_Status.h"

/* Layers */
#include "../../Hardware/Hardware.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RX_TP_MSG_NONE = 0,
    RX_TP_MSG_CM_CONN,
    RX_TP_MSG_TP_CONN_DATA_TRANSFER,
    RX_TP_MSG_RESP_REQ_DM1,
    RX_TP_MSG_RESP_REQ_DM2,
    RX_TP_MSG_DM16,
    RX_TP_MSG_RESP_REQ_SOFTWARE_IDENTIFICATION,
    RX_TP_MSG_RESP_REQ_ECU_IDENTIFICATION,
    RX_TP_MSG_RESP_REQ_COMPONENT_IDENTIFICATION,
    RX_TP_MSG_RESP_REQ_PROPRIETARY_A
} ENUM_J1939_RX_TP_MSG;

/* Acknowledgement */
void SAE_J1939_Read_Acknowledgement(J1939_t* j1939, uint8_t SA, uint8_t data[]);
ENUM_J1939_STATUS_CODES SAE_J1939_Send_Acknowledgement(J1939_t* j1939,
                                                       uint8_t DA,
                                                       uint8_t control_byte,
                                                       uint8_t group_function_value,
                                                       uint32_t PGN_of_requested_info);

/* Request */
void SAE_J1939_Read_Request(J1939_t* j1939, uint8_t SA, uint8_t data[]);
ENUM_J1939_STATUS_CODES SAE_J1939_Send_Request(J1939_t* j1939, uint8_t DA, uint32_t PGN_code);

/* Transport Protocol Connection Management */
void SAE_J1939_Read_Transport_Protocol_Connection_Management(J1939_t* j1939,
                                                             uint8_t SA,
                                                             uint8_t data[]);
ENUM_J1939_STATUS_CODES SAE_J1939_Send_Transport_Protocol_Connection_Management(J1939_t* j1939,
                                                                                uint8_t DA);

/* Transport Protocol Data Transfer */
ENUM_J1939_RX_TP_MSG
SAE_J1939_Read_Transport_Protocol_Data_Transfer(J1939_t* j1939, uint8_t SA, uint8_t data[]);
ENUM_J1939_STATUS_CODES SAE_J1939_Send_Transport_Protocol_Data_Transfer(J1939_t* j1939, uint8_t DA);

#ifdef __cplusplus
}
#endif

#endif /* SAE_J1939_21_TRANSPORT_LAYER_SAE_J1939_21_TRANSPORT_LAYER_H_ */
