


#ifndef _MESSAGES_H
#define _MESSAGES_H




#include <stdint.h>
#include "itsb.h"



//#pragma pack(1) //! to use sizeof struct

#define  MSG_LEN_MAX		32
#define  MSG_PAYLOAD_OFFSET	7   //! message len = payload len + 7(STX LEN SEQ MSGID CRC16 END)
#define  STX				0xFE
#define  END				0xEF



typedef __packed struct __message 
{
	uint16_t checksum; 	
	uint8_t  stx;
	uint8_t	 len;  
	uint8_t  seq;
	uint8_t  msg_id;
	uint8_t  payload[MSG_LEN_MAX];		
} message_t;




typedef __packed struct __msg_head_t
{
	uint8_t   stx;
	uint8_t	  len;  //! payload len
	uint8_t   seq;
	uint8_t   msg_id;
} msg_head_t;



typedef __packed struct __msg_start_mesh_t
{
	uint16_t reserved;
} msg_start_mesh_t;



typedef __packed struct __msg_ask_for_data_t
{ 
	uint8_t  node_id; 
	uint16_t reserved; 
} msg_ask_for_data_t;



typedef __packed struct __msg_ack_for_data_t
{ 
	uint8_t  node_id; 
	uint8_t  reserved_1;
	uint8_t	 dev_type;
	uint8_t	 volt_red;
	uint8_t  volt_yellow;
	uint8_t  volt_green;
	uint8_t  current_percentage_red;
	uint8_t  current_percentage_yellow;
	uint8_t  current_percentage_green;
	uint16_t current_red;
	uint16_t current_yellow;
	uint16_t current_green;
	uint16_t reserved_2;
} msg_ack_for_data_t;



typedef __packed struct __msg_ask_for_id_t
{ 
	uint8_t  node_id; 
	uint32_t mac[3];
	uint8_t  c_base_red;
	uint8_t  c_base_yellow;
	uint8_t  c_base_green;
	uint16_t reserved;
} msg_ask_for_id_t;



typedef __packed struct __msg_ack_for_id_t
{ 
	uint8_t  node_id; 
	uint32_t mac[3];
	uint16_t reserved;
} msg_ack_for_id_t;




typedef __packed struct __msg_assign_id_t
{ 
	uint8_t  node_id; 
	uint32_t mac[3];
	uint8_t  cfdt;
	uint8_t  reserved;
} msg_assign_id_t;




bool msg_parse_char(message_t *rxmsg, uint8_t c);
void msg_start_mesh_send(void);
void msg_ask_for_data_send(uint8_t node_id);
void msg_ack_for_data_send(	uint8_t node_id, uint8_t dev_type, uint8_t volt_red, uint8_t volt_yellow, uint8_t volt_green, uint8_t current_percentage_red,
							uint8_t current_percentage_yellow, uint8_t current_percentage_green, uint16_t current_red, uint16_t current_yellow, uint16_t current_green);
void msg_ask_for_id_send(uint32_t mac_0, uint32_t mac_1, uint32_t mac_2, uint8_t c_base_red, uint8_t c_base_yellow, uint8_t c_base_green);
void msg_assign_id_send(uint8_t node_id, uint32_t mac_0, uint32_t mac_1, uint32_t mac_2, uint8_t cfdt);
void msg_ack_for_id_send(uint8_t node_id, uint32_t mac_0, uint32_t mac_1, uint32_t mac_2);
void handleMessage(message_t *msg);

static void handle_msg_start_mesh(const message_t *msg);
static void handle_msg_ack_for_data(const message_t *msg);
static void handle_msg_ask_for_data(const message_t *msg);
static void handle_msg_ask_for_id(const message_t *msg);
static void handle_msg_ack_for_id(const message_t *msg);
static void handle_msg_assign_id(const message_t *msg);






typedef enum {
    MSG_PARSE_STATE_UNINIT = 0,
    MSG_PARSE_STATE_IDLE,
    MSG_PARSE_STATE_GOT_STX,
    MSG_PARSE_STATE_GOT_LENGTH,
	MSG_PARSE_STATE_GOT_SEQ,
	MSG_PARSE_STATE_GOT_MSGID,
    MSG_PARSE_STATE_GOT_PAYLOAD,
    MSG_PARSE_STATE_GOT_CRC1,
	MSG_PARSE_STATE_GOT_CRC2,
	MSG_PARSE_STATE_GOT_END,
	MSG_PARSE_STATE_GOT_OK
} msg_parse_state_t; //! The state machine for the comm parser




//#pragma pack()

#endif 