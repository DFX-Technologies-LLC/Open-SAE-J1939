/*
 * Listen_For_Messages.c
 *
 *  Created on: 14 juli 2021
 *      Author: Daniel Mårtensson
 */

#include "Open_SAE_J1939.h"

/* Layers */
#include "../Hardware/Hardware.h"
#include "../ISO_11783/ISO_11783-7_Application_Layer/Application_Layer.h"

/* This function should be called all the time, or be placed inside an interrupt listener */
ENUM_J1939_RX_MSG Open_SAE_J1939_Listen_For_Messages(J1939_t* j1939)
{
    uint32_t ID = 0;
    uint8_t data[8] = { 0 };
    ENUM_J1939_RX_MSG rx_msg = RX_MSG_NONE;
    bool is_new_message = CAN_Read_Message(&ID, data);
    if (is_new_message) {
        /* Save latest */
        j1939->ID = ID;
        memcpy(j1939->data, data, 8);
        j1939->ID_and_data_is_updated = true;

        // priority, extended data page and data page are not used
        uint8_t pdu_format = ID >> 16; // first part of PGN
        uint8_t pdu_specific = ID >> 8; // second part of PGN or destination address
        uint8_t source_address = ID;

        rx_msg = RX_MSG_NOT_SUPPORTED;

        /* Read request from other ECU */
        if (pdu_format == 0xEA
            && (pdu_specific == j1939->information_this_ECU.this_ECU_address
                || pdu_specific == 0xFF)) {
            SAE_J1939_Read_Request(j1939, source_address, data);
            rx_msg = RX_MSG_REQ;
        }
        // else if (pdu_format == 0xD9
        //            && pdu_specific == j1939->information_this_ECU.this_ECU_address) {
        //     SAE_J1939_Read_Request_DM14(j1939, source_address, data);
        //     rx_msg = RX_MSG_REQ_DM14;
        // }
        /* Read status from other ECU */
        else if (pdu_format == 0xE8
                 && pdu_specific == j1939->information_this_ECU.this_ECU_address) {
            SAE_J1939_Read_Acknowledgement(j1939, source_address, data);
            rx_msg = RX_MSG_ACK;
        }
        // else if (pdu_format == 0xD8
        //            && pdu_specific == j1939->information_this_ECU.this_ECU_address) {
        //     SAE_J1939_Read_Response_DM15(j1939, source_address, data);
        //     rx_msg = RX_MSG_DM15;
        // }
        // else if (pdu_format == 0xD7
        //            && pdu_specific == j1939->information_this_ECU.this_ECU_address) {
        //     SAE_J1939_Read_Binary_Data_Transfer_DM16(j1939, source_address, data);
        //     rx_msg = RX_MSG_DM16;
        // }
        /* Read Transport Protocol information from other ECU */
        else if (pdu_format == 0xEC
                 && (pdu_specific == j1939->information_this_ECU.this_ECU_address
                     || pdu_specific == 0xFF)) {
            SAE_J1939_Read_Transport_Protocol_Connection_Management(j1939, source_address, data);
            rx_msg = RX_MSG_TP_CONN_MANAGEMENT;
        } else if (pdu_format == 0xEB
                   && (pdu_specific == j1939->information_this_ECU.this_ECU_address
                       || pdu_specific == 0xFF)) {
            ENUM_J1939_RX_TP_MSG rx_tp_msg_type
                = SAE_J1939_Read_Transport_Protocol_Data_Transfer(j1939, source_address, data);
            if (rx_tp_msg_type == RX_TP_MSG_RESP_REQ_DM1) {
                rx_msg = RX_MSG_RESP_REQ_DM1;
            } else if (rx_tp_msg_type == RX_TP_MSG_RESP_REQ_DM2) {
                rx_msg = RX_MSG_RESP_REQ_DM2;
            } else {
                rx_msg = RX_MSG_TP_CONN_DATA_TRANSFER;
            }

            /* Read response request from other ECU - This are response request. They are responses
             * from other ECU about request from this ECU */
        } else if (pdu_format == 0xEF && pdu_specific == 0x23) {
            SAE_J1939_Read_Response_Request_Proprietary_A(j1939,
                                                          source_address,
                                                          data); /* Manufacturer specific data */
            rx_msg = RX_MSG_RESP_REQ_PROPRIETARY_A;
        } else if (pdu_format == 0xEE && pdu_specific == 0xFF && source_address != 0xFE) {
            SAE_J1939_Read_Response_Request_Address_Claimed(
                j1939,
                source_address,
                data); /* This is a broadcast response request */
            rx_msg = RX_MSG_RESP_REQ_ADDR_CLAIMED;
        } else if (pdu_format == 0xEE && pdu_specific == 0xFF && source_address == 0xFE) {
            SAE_J1939_Read_Address_Not_Claimed(j1939, source_address, data); /* This is error */
            rx_msg = RX_MSG_ADDR_NOT_CLAIMED;

        } else if (pdu_format == 0xFE && pdu_specific == 0xCA) {
            SAE_J1939_Read_Response_Request_DM1(j1939,
                                                source_address,
                                                data,
                                                1); /* Assume that errors_dm1_active = 1 */
            rx_msg = RX_MSG_RESP_REQ_DM1;
        } else if (pdu_format == 0xFE && pdu_specific == 0xCB) {
            SAE_J1939_Read_Response_Request_DM2(j1939,
                                                source_address,
                                                data,
                                                1); /* Assume that errors_dm2_active = 1 */
            rx_msg = RX_MSG_RESP_REQ_DM2;
        } else if (pdu_format == 0xFE && pdu_specific == 0xDA) {
            SAE_J1939_Read_Response_Request_Software_Identification(j1939, source_address, data);
            rx_msg = RX_MSG_RESP_REQ_SOFTWARE_IDENTIFICATION;
        } else if (pdu_format == 0xFD && pdu_specific == 0xC5) {
            SAE_J1939_Read_Response_Request_ECU_Identification(j1939, source_address, data);
            rx_msg = RX_MSG_RESP_REQ_ECU_IDENTIFICATION;
        } else if (pdu_format == 0xFE && pdu_specific == 0xEB) {
            SAE_J1939_Read_Response_Request_Component_Identification(j1939, source_address, data);
            rx_msg = RX_MSG_RESP_REQ_COMPONENT_IDENTIFICATION;
        } else if (pdu_format == 0xFE && pdu_specific >= 0x10 && pdu_specific <= 0x1F) {
            ISO_11783_Read_Response_Request_Auxiliary_Estimated_Flow(
                j1939,
                source_address,
                pdu_specific & 0xF,
                data); /* pdu_specific & 0xF = Valve number. Total 16 valves from 0 to 15 */
            rx_msg = RX_MSG_RESP_REQ_AUX_ESTIMATED_FLOW;
        } else if (pdu_format == 0xC6
                   && pdu_specific == j1939->information_this_ECU.this_ECU_address) {
            ISO_11783_Read_Response_Request_General_Purpose_Valve_Estimated_Flow(j1939,
                                                                                 source_address,
                                                                                 data);
            rx_msg = RX_MSG_RESP_REQ_GP_VALVE_ESTIMATED_FLOW;
        } else if (pdu_format == 0xFF && pdu_specific >= 0x20 && pdu_specific <= 0x2F) {
            ISO_11783_Read_Response_Request_Auxiliary_Valve_Measured_Position(
                j1939,
                source_address,
                pdu_specific & 0xF,
                data); /* pdu_specific & 0xF = Valve number. Total 16 valves from 0 to 15 */
            rx_msg = RX_MSG_RESP_REQ_AUX_VALVE_MEASURED_POSITION;

            /* Read command from other ECU */
        } else if (pdu_format == 0xFE && pdu_specific >= 0x30 && pdu_specific <= 0x3F) {
            ISO_11783_Read_Auxiliary_Valve_Command(
                j1939,
                source_address,
                pdu_specific & 0xF,
                data); /* pdu_specific & 0xF = Valve number. Total 16 valves from 0 to 15 */
            rx_msg = RX_MSG_AUX_VALVE_CMD;
        } else if (pdu_format == 0xC4
                   && pdu_specific == j1939->information_this_ECU.this_ECU_address) {
            ISO_11783_Read_General_Purpose_Valve_Command(
                j1939,
                source_address,
                data); /* General Purpose Valve Command have only one valve */
            rx_msg = RX_MSG_GP_VALVE_CMD;
        } else if (pdu_format == 0x2
                   && (pdu_specific == j1939->information_this_ECU.this_ECU_address
                       || pdu_specific == 0xFF)) {
            SAE_J1939_Read_Address_Delete(j1939, data); /* Not a SAE J1939 standard */
            rx_msg = RX_MSG_NOT_SAE_J1939;
        } else {
            rx_msg = RX_MSG_UNKNOWN; /* The message was not meant for this ECU */
        }
        /* Add more else if statement here */
    }
    return rx_msg;
}
