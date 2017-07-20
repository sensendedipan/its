




#include "messages.h"
#include "global.h"
#include "itsb.h"




#define MSG_SEND_USART_BYTES(buf, bytes)  usart1SendBuffer(buf, bytes) 




/****************************************************************/
//! msg_start_mesh_send (master -> slaver)
#define MSG_ID_START_MESH				0X56
#define MSG_START_MESH_PAYLOAD_LEN 		2

void msg_start_mesh_send(void)
{
	static uint8_t msg[MSG_START_MESH_PAYLOAD_LEN + MSG_PAYLOAD_OFFSET];
	static msg_head_t	msg_head;
	static msg_start_mesh_t payload;
	
	msg_head.stx	= STX;
	msg_head.len	= MSG_START_MESH_PAYLOAD_LEN;
	msg_head.msg_id	= MSG_ID_START_MESH;
	
	payload.reserved = 0;
	
	memcpy(&msg, &msg_head, sizeof(msg_head));
	memcpy(&msg[sizeof(msg_head)], &payload, sizeof(payload)); //! payload

	msg[sizeof(msg_head)+sizeof(payload)]   = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)));		//! crc LSB
	msg[sizeof(msg_head)+sizeof(payload)+1] = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)) >> 8);	//! crc MSB
	msg[sizeof(msg_head)+sizeof(payload)+2] = END;
	
	MSG_SEND_USART_BYTES(msg, sizeof(msg));
	printf("start mesh \n");
}




/****************************************************************/
//! msg_ask_for_data_send (master -> slaver)
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
	payload.reserved = 0;
	
	memcpy(&msg, &msg_head, sizeof(msg_head));
	memcpy(&msg[sizeof(msg_head)], &payload, sizeof(payload)); //! payload

	msg[sizeof(msg_head)+sizeof(payload)]   = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)));		//! crc LSB
	msg[sizeof(msg_head)+sizeof(payload)+1] = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)) >> 8);	//! crc MSB
	msg[sizeof(msg_head)+sizeof(payload)+2] = END;
	
	node[node_id].bad_cnt++; //! bigger means the slaver(node) bad and bad and bad....... !

    MSG_SEND_USART_BYTES(msg, sizeof(msg));
}





/****************************************************************/
//! msg_ack_for_data_send

#define MSG_ID_ACK_FOR_DATA				0X52
#define MSG_ACK_FOR_DATA_PAYLOAD_LEN 	9

void msg_ack_for_data_send(uint8_t node_id, uint8_t dev_type, uint8_t fault_type, uint8_t fault_scale) 
{
	static uint8_t msg[MSG_ACK_FOR_DATA_PAYLOAD_LEN + MSG_PAYLOAD_OFFSET];
	static msg_head_t	       msg_head;
	static msg_ack_for_data_t  payload;
	
	msg_head.stx        = STX;
	msg_head.len        = MSG_ACK_FOR_DATA_PAYLOAD_LEN;
	msg_head.seq	    = 0;
	msg_head.msg_id     = MSG_ID_ACK_FOR_DATA;
	
	payload.node_id     = node_id;
	payload.reserved_1  = 0; 							//! reserved
	payload.dev_type    = dev_type; 						//! define later just for test here
	payload.fault_type  = fault_type; 					//! define later just for test here
	payload.fault_scale = fault_scale;
	payload.reserved_4  = 0; 							//! reserved
	
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
#define MSG_ASK_FOR_ID_PAYLOAD_LEN 		15

void msg_ask_for_id_send(uint32_t mac_0, uint32_t mac_1, uint32_t mac_2) 
{
	static uint8_t msg[MSG_ASK_FOR_ID_PAYLOAD_LEN + MSG_PAYLOAD_OFFSET];
	static msg_head_t	     msg_head;
	static msg_ask_for_id_t  payload;
	
	msg_head.stx       = STX;
	msg_head.len       = MSG_ASK_FOR_ID_PAYLOAD_LEN;
	msg_head.seq	   = 0;
	msg_head.msg_id    = MSG_ID_ASK_FOR_ID;
	
	payload.node_id    = 0;			
	payload.mac[0]	   = mac_0;
	payload.mac[1]     = mac_1;
	payload.mac[2]     = mac_2;
	
	memcpy(&msg, &msg_head, sizeof(msg_head));
	memcpy(&msg[sizeof(msg_head)], &payload, sizeof(payload)); //! payload

	msg[sizeof(msg_head)+sizeof(payload)]   = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)));		//! crc LSB
	msg[sizeof(msg_head)+sizeof(payload)+1] = (uint8_t)(crc16(&msg[sizeof(msg_head)], sizeof(payload)) >> 8);	//! crc MSB
	msg[sizeof(msg_head)+sizeof(payload)+2] = END;
	
	MSG_SEND_USART_BYTES(msg, sizeof(msg)); 
}






/****************************************************************/
//! msg_assign_id_send (master -> slaver)

#define	MSG_ID_ASSIGN_ID				0X54
#define MSG_ASSIGN_ID_PAYLOAD_LEN 		15

void msg_assign_id_send(uint8_t node_id, uint32_t mac_0, uint32_t mac_1, uint32_t mac_2, uint8_t cfdt)  
{
	static uint8_t msg[MSG_ASSIGN_ID_PAYLOAD_LEN + MSG_PAYLOAD_OFFSET];
	static msg_head_t	    msg_head;
	static msg_assign_id_t  payload;
	
	msg_head.stx       = STX;
	msg_head.len       = MSG_ASSIGN_ID_PAYLOAD_LEN;
	msg_head.seq	   = 0;
	msg_head.msg_id    = MSG_ID_ASSIGN_ID;
	
	payload.node_id    = node_id;
	payload.mac[0]     = mac_0;
	payload.mac[1]     = mac_1;
	payload.mac[2]     = mac_2;
	payload.cfdt	   = cfdt;
	payload.reserved   = 0;
	
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
	msg_head.seq	   = 0;
	msg_head.msg_id    = MSG_ID_ACK_FOR_ID;
	
	payload.node_id    = node_id;
	payload.mac[0]     = mac_0;
	payload.mac[1]     = mac_1;
	payload.mac[2]     = mac_2;
	payload.reserved   = 0;	
	
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
	case MSG_ID_ASK_FOR_DATA:
		handle_msg_ask_for_data(msg);
		break;
	
	case MSG_ID_ACK_FOR_DATA:
		handle_msg_ack_for_data(msg);
		break;	
	
	case MSG_ID_ASK_FOR_ID:
		handle_msg_ask_for_id(msg);
		break;	

	case MSG_ID_ACK_FOR_ID:
		handle_msg_ack_for_id(msg);
		break;
	
	case MSG_ID_ASSIGN_ID:
		handle_msg_assign_id(msg);
		break;
	
	case MSG_ID_START_MESH:
		break;
	}
		
}





//! master -> slaver
static void handle_msg_start_mesh(const message_t *msg)
{
	return;
}


//! master -> slaver
static void handle_msg_ask_for_data(const message_t *msg)
{		
	return;
}



//! slaver -> master
static void handle_msg_ack_for_data(const message_t *msg)
{
	uint8_t i;
	msg_ack_for_data_t message;
	memcpy(&message, &msg->payload[0], sizeof(message));
	
	for (i = 1; i <= NODE_ID_NUM_MAX; i++) { //! traverse the entire array which saves all the nodes 
		if (node[i].id == message.node_id) {
			node[i].bad_cnt	  	= 0;         //! if the data ack ok, then the bad_cnt will be reset to 0, which means this slaver(node) is good
			node[i].dev_type  	= message.dev_type;
			node[i].fault_type	= message.fault_type;
			node[i].fault_scale = message.fault_scale;
			node[i].state 	  	= NODE_STATE_ONLINE;
			printf("id: %d state: %d bad_cnt: %d mac: %X-%X-%X \n", node[i].id, node[i].state, node[i].bad_cnt, node[i].mac[0], node[i].mac[1], node[i].mac[2]);	
		}
		
	}

}



//! slaver -> master
static void handle_msg_ask_for_id(const message_t *msg)
{
	uint8_t i;
	bool need_trav_ids = false;
	msg_ask_for_id_t message;
	memcpy(&message, &msg->payload[0], sizeof(message));
	
	gGetStartNewMeshAck = true;
	gMeshFinished = false; //! there still some nodes want to join into the net, mesh not finished 
	gNoneAskForCnt = 0;
	//! traverse the entire array which saves all the nodes. i prefer to traverse mac first, if this mac has been existed, then assign the corresponding id to the node 
	//! if can not find this mac, then traverse the id from 1 to max, assign the seq as id till the corresponding id is zero.
	for (i = 1; i <= NODE_ID_NUM_MAX; i++) {
		if ((node[i].mac[0] == message.mac[0]) && (node[i].mac[1] == message.mac[1]) && (node[i].mac[2] == message.mac[2])) {
			node[i].bad_cnt = 0; //! another place always check this value, if this node down less than n times, and here assign the same id again, we should reset the bad_cnt in case be reseted again 
			need_trav_ids = false;
			msg_assign_id_send(i, message.mac[0], message.mac[1], message.mac[2], NODE_BAD_CNT_MAX);
			break;
		
		} else {
			need_trav_ids = true;
		}
	}
	
	//! if need, traverse the ids
	if (need_trav_ids == true) {
		need_trav_ids = false;
		
		for (i = 1; i <= NODE_ID_NUM_MAX; i++) { 
			if (node[i].id == 0) {				 
				node[i].id     = i;
				node[i].mac[0] = message.mac[0];
				node[i].mac[1] = message.mac[1];
				node[i].mac[2] = message.mac[2];
				
				msg_assign_id_send(i, message.mac[0], message.mac[1], message.mac[2], NODE_BAD_CNT_MAX * NODE_ID_NUM_MAX * TRAVERSE_PERIOD /1000);
				break; //! must quit out otherwise will assign all the idle nodes
		
			} else if (i == NODE_ID_NUM_MAX) { //! it means the active nodes up to MAX, then reject this node mesh
				msg_assign_id_send(255, message.mac[0], message.mac[1], message.mac[2], NODE_BAD_CNT_MAX);
			}				
		}
		
	}
}


//! slaver -> master
static void handle_msg_ack_for_id(const message_t *msg)
{
	msg_ack_for_id_t message;
	memcpy(&message, &msg->payload[0], sizeof(message));	
	
//	printf("ack_for_id---msg_id = %X msg_nodeid = %d  mac = %X-%X-%X \n",	
//		msg->msg_id, 
//		message.node_id,
//		message.mac[0],
//		message.mac[1],
//		message.mac[2]	
//	);	
}


//! master -> slaver
static void handle_msg_assign_id(const message_t *msg)
{
	return;	
}


























