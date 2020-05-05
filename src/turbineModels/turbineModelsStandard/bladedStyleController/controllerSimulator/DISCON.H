/*
 * Based on https://github.com/NREL/ROSCO/blob/master/src/DISCON.F90
 * and https://github.com/NREL/ROSCO_toolbox/blob/master/ROSCO_toolbox/control_interface.py
 */

const int AVRSIZE = 500;
const int BUFFERSIZE = 1000;

void DISCON
(
    float avrSWAP[], // The swap array, used to pass data to, and receive data from, the DLL controller.
    int *aviFAIL, // A flag used to indicate the success of this DLL call set as follows: 0 if the DLL call was successful, >0 if the DLL call was successful but cMessage should be issued as a warning messsage, <0 if the DLL call was unsuccessful or for any other reason the simulation is to be stopped at this point with cMessage as the error message.
    const char *accINFILE, // The name of the parameter input file
    const char *avcOUTNAME, // OUTNAME (Simulation RootName)
    char *avcMSG // MESSAGE (Message from DLL to simulation code [ErrMsg])  The message which will be displayed by the calling program if aviFAIL <> 0.
);

