

#include "itsb.h"
#include "global.h"

uint32_t myMac[3];
uint8_t  node_current;
uint8_t  node_cycle = 1;
uint16_t g_id_request;
bool g_flag_ask_for_data = false;
bool g_flag_check_nodes = false;

node_t node[NODE_ID_NUM_MAX];

uint8_t g_test;









int main(void)
{	
	itsm_mode_t mode = ITS_MODE_UNINIT;
	
	boardInit();
	
	while (!getCpuId(&myMac[0], &myMac[1], &myMac[2])) {
		ledFlashSet(1, 1000, 10);
	}
	
    ledFlashSet(1, 1000, 10);
		
	
	
	
    while(1) 
	{	
		
		taskParseRadioData();
		
		checkNodesState(g_flag_check_nodes);
		
		switch(mode)
		{
		case ITS_MODE_UNINIT:
		case ITS_MODE_IDLE:
			mode = ITS_MODE_MESH;
			break;
		
		case ITS_MODE_MESH:
			msg_start_mesh_send();
			mode = ITS_MODE_NORMAL;			
			break;
		
		case ITS_MODE_NORMAL:
			
			break;
		
		case ITS_MODE_NONE:
			
			break;
			
		}
		
		if (g_id_request >= 40) {
			if (g_flag_ask_for_data) {
				g_flag_ask_for_data = false;
				
				if (node_cycle > NODE_ID_NUM_MAX) {
					node_cycle = 1;
				}
				msg_ask_for_data_send(node_cycle);	
				node_cycle++;				
			}			
			
		}
		 
	}	

}





void taskParseRadioData(void)
{
	uint8_t data;
	static message_t msg;
	
	while (fifoGetBufDataCount()) {
		fifoPopBuf(&data, 1);
		if
			(msg_parse_char(&msg, data)) {
			handleMessage(&msg);
		}
	}	
	
}



void checkNodesState(bool flag)
{
	static bool temp;
	uint8_t i;
	
	if (flag != temp) {
		for (i = 1; i <= NODE_ID_NUM_MAX; i++) { //! check all the nodes 
			if (node[i].bad_cnt >= NODE_BAD_CNT_MAX) {
				node[i].id = 0;
				node[i].bad_cnt	  	= 0;
				node[i].mac[0]	 	= 0;
				node[i].mac[1]	  	= 0;
				node[i].mac[2]	  	= 0;
				node[i].dev_type	= 0;
				node[i].fault_scale = 0;
				node[i].fault_type  = 0;
				node[i].state 	  	= NODE_STATE_OFFLINE;	
				memset(&node[i], 0, sizeof(node_t));
			}
		
		}
		temp = flag;		
	}		
	
}





































































