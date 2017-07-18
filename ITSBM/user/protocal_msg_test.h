


#include "itsb.h"


const uint16_t HARDWARE_ADDR = 0xFFFF;  // broadcast addr 
const uint8_t  DEST_CHANNEL  = 0x50;    // 433MHz channel  
const uint8_t  STX = 0xFE;
const uint8_t  END = 0xEF;


#define MSG_ID_ASK_FOR_DATA		0X51
#define MSG_ID_ACK_FOR_DATA		0X52
#define MSG_ID_ASK_FOR_ID		0X53
#define	MSG_ID_ASSIGN_ID		0X54
#define	MSG_ID_ACK_FOR_ID		0X55


void msg_ask_for_data_send(uint8_t nodeId)
{   
	uint8_t msg[12] = {0};
	
	msg[0]  = (uint8_t)(HARDWARE_ADDR);      		//! LSB
	msg[1]  = (uint8_t)(HARDWARE_ADDR >> 8); 		//! MSB
	msg[2]  = DEST_CHANNEL;					 		//! channel
	msg[3]  = STX;							 		//! start flag
	msg[4]  = 0x04;                          		//! payload length
	msg[5]  = MSG_ID_ASK_FOR_DATA;			 		//! message id
	msg[6]  = nodeId;                        		//! node id
	msg[7]  = 0x00;                          		//!reserved
	msg[8]  = 0x00; 						 		//!reserved
	msg[9]  = (uint8_t)crc16(&msg[5], 0x04); 		//! crc LSB
	msg[10] = (uint8_t)(crc16(&msg[5], 0x04) >> 8); //£¡crc MSB
	msg[11] = END;

    usart2SendBuffer(msg, sizeof(msg)); 
}



void msg_ack_for_data_send(void) 
{
	
	
	
}



void msg_ask_for_id_send(void) 
{
	
	
	
}



void msg_assign_id_send(void)  
{
	
	
}



void msg_ack_for_id_send(void) 
{
	
	
	
}








