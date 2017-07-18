

#include "itsb.h"


node_t myNode;




bool gComeBackToMesh = false;			//! if rejected while apply for id, then go to NONE mode, and if the master start a new mesh again, then can come out of NONE mode to apply for id 
bool gNetworkRunning = false;			//! the system is in running mode, if i want to ask for id, should send msg in free time  
bool gCanAskForIdTriger = false;		//! if ask for id, should under own radio period
bool gCanAskForIdDurNormMode = false;	//! 

itsbs_mode_t gMode = ITS_MODE_UNINIT;

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
			gMode = ITS_MODE_GET_PERIOD;
			break;
		
		case ITS_MODE_GET_PERIOD:
			timer3Init(getAskForIdPeriod()*50);
			gMode = ITS_MODE_MESH;
			break;
		
		case ITS_MODE_MESH:
			gMode = taskMesh();
			break;
		
		case ITS_MODE_NORMAL:
			taskNormalMission();
		
			if (myNode.node_id == 0) {
				gMode = ITS_MODE_MESH; //! master start a new mesh, should start go to apply for id 
				gComeBackToMesh = false;
				
			} else if (myNode.bad_cnt > myNode.cfdt) { //! communication is bad ! run selfcheck !
				
				//gMode = ITS_MODE_SELF_CHECK;
				printf("i am running selfcheck! \n");
			}
			break;

		case ITS_MODE_SELF_CHECK:
			 //! run selfcheck!
			break;
		
		case ITS_MODE_NONE:
			ledFlashSet(0, 2000, 1000); //! i am rejected!
			if (gComeBackToMesh) {		//! it can come out of the NONE mode and start apply for id again
				gMode = ITS_MODE_MESH;
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



itsbs_mode_t taskMesh(void)
{
	static uint8_t askForIdCnt = 0;
	static itsbs_mode_t mode = ITS_MODE_MESH;
	
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
			
			if (askForIdCnt == ASK_FOR_ID_RETRY_MAX) {	//! ask for id more than n times, modify send period as 500ms
				timer3Init(500);
			}
			
		} else if (myNode.node_id == 255) { //! rejected my mesh request!!!
			askForIdCnt = 0;
			timer3Init(getAskForIdPeriod()*50);	//! just case of had modified the period
			mode = ITS_MODE_NONE;
			printf("i am ok but rejected ! \n");	
			
		} else { //! get the id succeed !
			askForIdCnt = 0;
			timer3Init(getAskForIdPeriod()*50); //! just case of had modified the period
			mode = ITS_MODE_NORMAL;
			printf("i get my id = %d \n", myNode.node_id);			
		}
	}

	return mode;
}



void taskNormalMission(void)
{

	
}




void taskSelfCheck(void)
{
	
	
}