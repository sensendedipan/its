




#include "messages.h"
 





#define MSG_SEND_USART_BYTES(buf, bytes)  usart1SendBuffer(buf, bytes)

uint16_t gRadioNoDataCnt = 0;


/****************************************************************/
//! msg_start_mesh_send
#define MSG_ID_START_MESH				0X56
#define MSG_START_MESH_PAYLOAD_LEN		2

void msg_start_mesh_send(void)
{
	static uint8_t msg[MSG_START_MESH_PAYLOAD_LEN + MSG_PAYLOAD_OFFSET];
	static msg_head_t			msg_head;
	static msg_start_mesh_t		payload;
	
	msg_head.stx	= STX;
	msg_head.len	= MSG_START_MESH_PAYLOAD_LEN;
	msg_head.msg_id	= MSG_ID_START_MESH;
	
	memcpy(&msg, &msg_head, sizeof(msg_head));
	memcpy(&msg[sizeof(msg_head)], &payload, sizeof(payload)); //! payload

	msg[sizeof(msg_head)+sizeof(payload)]   = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)));		//! crc LSB
	msg[sizeof(msg_head)+sizeof(payload)+1] = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)) >> 8);	//! crc MSB
	msg[sizeof(msg_head)+sizeof(payload)+2] = END;

    MSG_SEND_USART_BYTES(msg, sizeof(msg)); 	
}




/****************************************************************/
//! msg_ask_for_data_send
#define MSG_ID_ASK_FOR_DATA				0X51
#define MSG_ASK_FOR_DATA_PAYLOAD_LEN 	3

void msg_ask_for_data_send(uint8_t node_id)
{   
	static uint8_t msg[MSG_ASK_FOR_DATA_PAYLOAD_LEN + MSG_PAYLOAD_OFFSET];
	static msg_head_t          msg_head;					//! msg head
	static msg_ask_for_data_t  payload;						//! msg payload

	msg_head.stx     = STX;
	msg_head.len     = MSG_ASK_FOR_DATA_PAYLOAD_LEN;
	msg_head.msg_id  = MSG_ID_ASK_FOR_DATA;
	
	payload.node_id  = node_id;
	payload.reserved = 55;
	
	memcpy(&msg, &msg_head, sizeof(msg_head));
	memcpy(&msg[sizeof(msg_head)], &payload, sizeof(payload)); //! payload

	msg[sizeof(msg_head)+sizeof(payload)]   = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)));		//! crc LSB
	msg[sizeof(msg_head)+sizeof(payload)+1] = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)) >> 8);	//! crc MSB
	msg[sizeof(msg_head)+sizeof(payload)+2] = END;

    MSG_SEND_USART_BYTES(msg, sizeof(msg)); 
	printf("head = %d  payload = %d tol = %d \n", sizeof(msg_head), sizeof(payload), sizeof(msg));
}





/****************************************************************/
//! msg_ack_for_data_send
#define MSG_ID_ACK_FOR_DATA				0X52
#define MSG_ACK_FOR_DATA_PAYLOAD_LEN 	17

void msg_ack_for_data_send(	uint8_t  node_id, 
							uint8_t	 dev_type, 
							int8_t   temperature,
							uint8_t	 volt_red,
							uint8_t  volt_yellow,
							uint8_t  volt_green,
							uint8_t  current_percentage_red,
							uint8_t  current_percentage_yellow,
							uint8_t  current_percentage_green,
							uint16_t current_red,
							uint16_t current_yellow,
							uint16_t current_green)
{
	static uint8_t msg[MSG_ACK_FOR_DATA_PAYLOAD_LEN + MSG_PAYLOAD_OFFSET];
	static msg_head_t	       msg_head;
	static msg_ack_for_data_t  payload;
	
	msg_head.stx        = STX;
	msg_head.len        = MSG_ACK_FOR_DATA_PAYLOAD_LEN;
	msg_head.msg_id     = MSG_ID_ACK_FOR_DATA;
	
	payload.node_id     = node_id;
	payload.dev_type    = dev_type; //! define later just for test here
	payload.volt_red	= volt_red;
	payload.volt_yellow	= volt_yellow;
	payload.volt_green	= volt_green;
	payload.temperature	= temperature;
	payload.current_percentage_red	= current_percentage_red;
	payload.current_percentage_yellow	= current_percentage_yellow;
	payload.current_percentage_green	= current_percentage_green;
	payload.current_red	= current_red;
	payload.current_yellow	= current_yellow;
	payload.current_green	= current_green;
	
	memcpy(&msg, &msg_head, sizeof(msg_head));
	memcpy(&msg[sizeof(msg_head)], &payload, sizeof(payload)); //! payload

	msg[sizeof(msg_head)+sizeof(payload)]   = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)));		//! crc LSB
	msg[sizeof(msg_head)+sizeof(payload)+1] = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)) >> 8);	//! crc MSB
	msg[sizeof(msg_head)+sizeof(payload)+2] = END;
	
	MSG_SEND_USART_BYTES(msg, sizeof(msg)); 
}





/****************************************************************/
//! msg_ask_for_id_send

#define MSG_ID_ASK_FOR_ID				0X53
#define MSG_ASK_FOR_ID_PAYLOAD_LEN 		18

void msg_ask_for_id_send(uint32_t mac_0, uint32_t mac_1, uint32_t mac_2, uint8_t c_base_red, uint8_t c_base_yellow, uint8_t c_base_green) 
{
	static uint8_t msg[MSG_ASK_FOR_ID_PAYLOAD_LEN + MSG_PAYLOAD_OFFSET];
	static msg_head_t	     msg_head;
	static msg_ask_for_id_t  payload;
	
	msg_head.stx       = STX;
	msg_head.len       = MSG_ASK_FOR_ID_PAYLOAD_LEN;
	msg_head.msg_id    = MSG_ID_ASK_FOR_ID;
	
	payload.node_id    = 0;			
	payload.mac[0]	   = mac_0;
	payload.mac[1]     = mac_1;
	payload.mac[2]     = mac_2;
	payload.c_base_red	   = c_base_red;
	payload.c_base_yellow  = c_base_yellow;
	payload.c_base_green   = c_base_green;
	
	memcpy(&msg, &msg_head, sizeof(msg_head));
	memcpy(&msg[sizeof(msg_head)], &payload, sizeof(payload)); //! payload

	msg[sizeof(msg_head)+sizeof(payload)]   = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)));		//! crc LSB
	msg[sizeof(msg_head)+sizeof(payload)+1] = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)) >> 8);	//! crc MSB
	msg[sizeof(msg_head)+sizeof(payload)+2] = END;
	
	MSG_SEND_USART_BYTES(msg, sizeof(msg)); 
}






/****************************************************************/
//! msg_assign_id_send

#define	MSG_ID_ASSIGN_ID				0X54
#define MSG_ASSIGN_ID_PAYLOAD_LEN 		15

void msg_assign_id_send(uint8_t node_id, uint32_t mac_0, uint32_t mac_1, uint32_t mac_2, uint8_t cfdt)  
{
	static uint8_t msg[MSG_ASSIGN_ID_PAYLOAD_LEN + MSG_PAYLOAD_OFFSET];
	static msg_head_t	    msg_head;
	static msg_assign_id_t  payload;
	
	msg_head.stx       = STX;
	msg_head.len       = MSG_ASSIGN_ID_PAYLOAD_LEN;
	msg_head.msg_id    = MSG_ID_ASSIGN_ID;
	
	payload.node_id    = node_id;
	payload.mac[0]     = mac_0;
	payload.mac[1]     = mac_1;
	payload.mac[2]     = mac_2;
	payload.cfdt	   = cfdt;
	payload.reserved   = 55;
	
	memcpy(&msg, &msg_head, sizeof(msg_head));
	memcpy(&msg[sizeof(msg_head)], &payload, sizeof(payload)); //! payload

	msg[sizeof(msg_head)+sizeof(payload)]   = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)));		//! crc LSB
	msg[sizeof(msg_head)+sizeof(payload)+1] = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)) >> 8);	//! crc MSB
	msg[sizeof(msg_head)+sizeof(payload)+2] = END;
	
	MSG_SEND_USART_BYTES(msg, sizeof(msg)); 
}





/****************************************************************/
//! msg_ack_for_id_send

#define	MSG_ID_ACK_FOR_ID				0X55
#define MSG_ACK_FOR_ID_PAYLOAD_LEN 		15

void msg_ack_for_id_send(uint8_t node_id, uint32_t mac_0, uint32_t mac_1, uint32_t mac_2) 
{
	static uint8_t msg[MSG_ACK_FOR_ID_PAYLOAD_LEN + MSG_PAYLOAD_OFFSET];
	static msg_head_t	     msg_head;
	static msg_ack_for_id_t  payload;
	
	msg_head.stx       = STX;
	msg_head.len       = MSG_ACK_FOR_ID_PAYLOAD_LEN;
	msg_head.msg_id    = MSG_ID_ACK_FOR_ID;
	
	payload.node_id    = node_id;
	payload.mac[0]     = mac_0;
	payload.mac[1]     = mac_1;
	payload.mac[2]     = mac_2;
	payload.reserved   = 55;	
	
	memcpy(&msg, &msg_head, sizeof(msg_head));
	memcpy(&msg[sizeof(msg_head)], &payload, sizeof(payload)); //! payload

	msg[sizeof(msg_head)+sizeof(payload)]   = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)));		//! crc LSB
	msg[sizeof(msg_head)+sizeof(payload)+1] = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)) >> 8);	//! crc MSB
	msg[sizeof(msg_head)+sizeof(payload)+2] = END;
	
	MSG_SEND_USART_BYTES(msg, sizeof(msg)); 
}









bool msg_parse_char(message_t *rxmsg, uint8_t c)
{
	bool parse_flag = false;
	static message_t rxmsg_t;	
	static uint16_t bufferIndex;
	static msg_parse_state_t status;
	
	switch (status)
	{
	case MSG_PARSE_STATE_UNINIT:
	case MSG_PARSE_STATE_IDLE:
		if (c == STX) {
			rxmsg_t.stx = c;
			status = MSG_PARSE_STATE_GOT_STX; 
		} 
		break;

	case MSG_PARSE_STATE_GOT_STX:
		rxmsg_t.len = c;
		status = MSG_PARSE_STATE_GOT_LENGTH; 
		break;

	case MSG_PARSE_STATE_GOT_LENGTH:
		rxmsg_t.seq = c;
		status = MSG_PARSE_STATE_GOT_SEQ;
		break;
	
	case MSG_PARSE_STATE_GOT_SEQ:
		rxmsg_t.msg_id = c;
		status = MSG_PARSE_STATE_GOT_MSGID;
		break;
	
	case MSG_PARSE_STATE_GOT_MSGID:
		rxmsg_t.payload[bufferIndex++] = c;
		if (bufferIndex == rxmsg_t.len) {
			status = MSG_PARSE_STATE_GOT_PAYLOAD;
		}
		break;

	case MSG_PARSE_STATE_GOT_PAYLOAD:
			rxmsg_t.checksum = (uint16_t)c;                
			status = MSG_PARSE_STATE_GOT_CRC1;
		break;

	case MSG_PARSE_STATE_GOT_CRC1:
			rxmsg_t.checksum |= (uint16_t)(c << 8);				
			status = MSG_PARSE_STATE_GOT_CRC2;		
		break;
	
	case MSG_PARSE_STATE_GOT_CRC2:
		if ((crc16(&rxmsg_t.payload[0], bufferIndex) == rxmsg_t.checksum) && (c == END)) {
			memcpy(rxmsg, &rxmsg_t, sizeof(rxmsg_t));
			parse_flag = true;	
		}
		bufferIndex = 0;                                   
		status = MSG_PARSE_STATE_UNINIT;
        break;
	
	default:
		bufferIndex = 0;
		status = MSG_PARSE_STATE_UNINIT;

	}
	
	return parse_flag;
}




void handleMessage(message_t *msg)
{
	switch(msg->msg_id)
	{
	
	case MSG_ID_ASK_FOR_ID:
		handle_msg_ask_for_id(msg);
		break;
	
	case MSG_ID_ASK_FOR_DATA:
		handle_msg_ask_for_data(msg); 
		break;
		
	case MSG_ID_ASSIGN_ID:
		handle_msg_assign_id(msg);
		break;
	
	case MSG_ID_ACK_FOR_DATA:
		handle_msg_ack_for_data(msg);
		break;
	
	case MSG_ID_ACK_FOR_ID:
		handle_msg_ack_for_id(msg);
		break;
	
	case MSG_ID_START_MESH:
		handle_msg_start_mesh(msg);
		break;

	}
		
}




//! master -> slaver
static void handle_msg_start_mesh(const message_t *msg)
{
	myNode.node_id = 0;			//! start to mesh again
	gNetworkRunning = false;	//! means the net is in meshing mode
	printf("start a new mesh \n");
}


//! master -> slaver
static void handle_msg_ask_for_data(const message_t *msg)
{
	msg_ask_for_data_t message;
	memcpy(&message, &msg->payload[0], sizeof(message));
	
	gRadioNoDataCnt = 0;	//! when received a msg, this value will be reset to 0.
	gNetworkRunning = true;	//! means the net is in running mode
	
	if (message.node_id == 0) {
		return;	//! there must be something wrong with master, just give no ack
	}
 
	if (message.node_id == myNode.node_id) {
		myNode.bad_cnt = 0;     //! can receive this msg means communication is OK!		
		msg_ack_for_data_send(myNode.node_id, 25, 0, 11, 22, 33, 44, 55, 66, 77, 88, 99); 
		printf("ack for data my id : %d \n", myNode.node_id);
		ledFlashSet(1, 200, 5);
		
	}		
	
}



//! master -> slaver
static void handle_msg_assign_id(const message_t *msg)
{
	msg_assign_id_t message;
	memcpy(&message, &msg->payload[0], sizeof(message));	
	
	if ((message.mac[0] != myNode.mac[0])||(message.mac[1] != myNode.mac[1])||(message.mac[2] != myNode.mac[2])) {
		printf("this is not my mac! \n");
		if (message.node_id == myNode.node_id) { //! my id is reassigned to other nodes, i have to apply for a new id 
			myNode.node_id = 0;
			printf("i have to re apply for my id \n");
		}
		
	} else {
		myNode.node_id	= message.node_id;
		myNode.cfdt		= message.cfdt;
	}
}



//! slaver -> master
static void handle_msg_ack_for_data(const message_t *msg)
{
	gCanAskForIdDurNormMode = true;	//! if received this msg, then there is some free time for me to apply for id 
	
	return;	
}


//! slaver -> master
static void handle_msg_ask_for_id(const message_t *msg)
{

}


//! slaver -> master
static void handle_msg_ack_for_id(const message_t *msg)
{
	
}






















