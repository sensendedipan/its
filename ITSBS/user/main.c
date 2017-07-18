

#include "itsb.h"


node_t myNode;
itss_mode_t mode = ITS_MODE_UNINIT;



bool gComeBackToMesh = false;			//! if rejected while apply for id, then go to NONE mode, and if the master start a new mesh again, then can come out of NONE mode to apply for id 
bool gNetworkRunning = false;			//! the system is in running mode, if i want to ask for id, should send msg in free time  
bool gCanAskForIdTriger = false;		//! if ask for id, should under own radio period
bool gCanAskForIdDurNormMode = false;	//! 


int main(void)
{	
	boardInit();
	

    while(1) 
	{	
	
		taskParseRadioData();	 		
		
		switch(mode)
		{
		case ITS_MODE_UNINIT:
		case ITS_MODE_IDLE:
			mode = ITS_MODE_GET_RADIO_PERIOD;
			break;
		
		case ITS_MODE_GET_RADIO_PERIOD:
			timer3Init(getAskForIdPeriod()*50);
			mode = ITS_MODE_MESH;
			break;
		
		case ITS_MODE_MESH:
			taskMesh();
			break;
		
		case ITS_MODE_NORMAL:
			taskNormalMission();
		
			if (myNode.node_id == 0) {
				mode = ITS_MODE_MESH; //! master start a new mesh, should start go to apply for id
			}

			break;

		case ITS_MODE_NONE:
			ledFlashSet(0, 2000, 1000); //! i am rejected!
			if (gComeBackToMesh) {		//! it can come out of the NONE mode and start apply for id again
				mode = ITS_MODE_MESH;
				gComeBackToMesh = false;
			}
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



void taskMesh(void)
{
	static uint8_t askForIdCnt = 0;
	
	if (gCanAskForIdTriger) {
		gCanAskForIdTriger = false;
		
		if (myNode.node_id == 0) {
			if (gNetworkRunning) { //! the network is running normally
				if (gCanAskForIdDurNormMode) { //! check if it is the free time that can send ask for id msg
					gCanAskForIdDurNormMode = false;
					askForIdCnt++; printf("normal mode: ask for id times: %d \n", askForIdCnt);
					msg_ask_for_id_send(myNode.mac[0], myNode.mac[1], myNode.mac[2]);						
				}
				
			} else { //! network is running mesh, just send ask for id msg under my period
				msg_ask_for_id_send(myNode.mac[0], myNode.mac[1], myNode.mac[2]);						
				askForIdCnt++; printf("mesh mode: ask for id times: %d \n", askForIdCnt);
			}
			
		} else if (myNode.node_id == 255) { //! rejected my mesh request!!!
			mode = ITS_MODE_NONE;
			askForIdCnt = 0;
			printf("i am rejected ! \n");	
			
		} else {
			printf("my_id = %d \n", myNode.node_id);
			mode = ITS_MODE_NORMAL;
		}
	}				
}



void taskNormalMission(void)
{
	if (myNode.bad_cnt > myNode.cfdt) { //! communication is bad ! run selfcheck !
		
		//! run selfcheck!
		printf("i am running selfcheck! \n");
	}
	
}


