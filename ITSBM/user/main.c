

#include "itsb.h"
#include "global.h"



bool gCanAskForData = false;
bool gMeshFinished = false;
bool gCheckNotesState = false;

node_t node[NODE_ID_NUM_MAX]; //! note: the node[0] store the master's own info.



int main(void)
{	
	itsm_mode_t mode = ITS_MODE_UNINIT;
	
	boardInit();
	
	
    while(1) 
	{	
		taskParseRadioData();
		
		switch(mode)
		{
		case ITS_MODE_UNINIT:
		case ITS_MODE_IDLE:
			mode = ITS_MODE_MESH;
			break;
		
		case ITS_MODE_MESH:
			msg_start_mesh_send();
			
			if (gMeshFinished == true) {
				mode = ITS_MODE_NORMAL;		
			}	
			break;
		
		case ITS_MODE_NORMAL:
			checkNodesStatePeriod();
			askForDataPeriod();		
			break;
		
		case ITS_MODE_NONE:
			
			break;
			
		}
		 
	}	

}





void taskParseRadioData(void)
{
	uint8_t data;
	static message_t msg;
	
	while (fifoGetBufDataCount()) {
		fifoPopBuf(&data, 1);
		
		if (msg_parse_char(&msg, data)) {
			handleMessage(&msg);
		}
	}	
	
}


//! twice of askForDataPeriod() in running frequency(rate)
void checkNodesStatePeriod(void) 
{
	static bool flag;
	uint8_t i;
	
	if (flag != gCheckNotesState) {
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
		flag = gCheckNotesState;		
	}		
	
}



void askForDataPeriod(void)
{
	static bool flag;
	static uint8_t node_cycle = 1;
	
	if ((gCanAskForData)&&(flag != gCanAskForData)) {
		flag = gCanAskForData;
		
		if (node_cycle > NODE_ID_NUM_MAX) {
			node_cycle = 1;
		}
		msg_ask_for_data_send(node_cycle); printf("i am asking for data from node %d", node_cycle);	
		node_cycle++;

		
	} else if (flag != gCanAskForData) {
		flag = gCanAskForData;
	
	} else {

	}		
		
}































































