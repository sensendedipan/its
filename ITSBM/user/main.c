

#include "itsb.h"
#include "global.h"



bool gCanAskForDataPeriod = false;
bool gMeshFinished = false;
bool gCheckNotesState = false;
bool gGetStartNewMeshAck = false;

node_t node[NODE_ID_NUM_MAX]; //! note: the node[0] store the master's own info.

itsbm_mode_t gMode = ITS_MODE_UNINIT;

int main(void)
{	
	boardInit();
	
    while(1) 
	{	
		taskParseRadioData();
		
		switch(gMode)
		{
		case ITS_MODE_UNINIT:
		case ITS_MODE_IDLE:
			gMode = ITS_MODE_START_MESH;
			break;
		
		case ITS_MODE_START_MESH:
			startNewMesh();
			gMode = ITS_MODE_MESH;
			break;
		
		case ITS_MODE_MESH:
			if (gGetStartNewMeshAck == false) {	//! if not received ask for id msg?
				gMode = ITS_MODE_START_MESH;
				
			} else if (gMeshFinished == true) {
				gMode = ITS_MODE_NORMAL;		
			}	
			break;
		
		case ITS_MODE_NORMAL:
			checkNodesStatePeriod();
			askForDataPeriod();		
			break;
		
		case ITS_MODE_SELF_CHECK:
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
	
	if ((gCanAskForDataPeriod)&&(flag != gCanAskForDataPeriod)) {
		flag = gCanAskForDataPeriod;
		
		if (node_cycle > NODE_ID_NUM_MAX) {
			node_cycle = 1;
		}
		msg_ask_for_data_send(node_cycle); printf(" getting data node %d", node_cycle);	
		node_cycle++;
		
	} else if (flag != gCanAskForDataPeriod) {
		flag = gCanAskForDataPeriod;
	
	} else {

	}		
		
}



void startNewMesh(void)
{
	msg_start_mesh_send();	//! what should do if the slavers miss this msg?
	delayMs(1000);
}




























































