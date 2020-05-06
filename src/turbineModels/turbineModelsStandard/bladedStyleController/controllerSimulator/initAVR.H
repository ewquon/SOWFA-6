// Define saved quantities for verification
const scalar dt = runTime.deltaTValue();
const label N = runTime.endTime().value() / dt;
List<scalar> wspd(N+1, 0.0);
List<scalar> rotSpd(N+1, 0.0);
List<scalar> genTq(N+1, 0.0);
List<scalar> blPitch(N+1, 0.0);
List<scalar> rotSpdErr;
List<scalar> genTqErr;
List<scalar> blPitchErr;

wspd[0] = wspdTable->value(0.0);
rotSpd[0] = rotSpdTable->value(0.0);
genTq[0] = genTqTable->value(0.0);
blPitch[0] = blPitchTable->value(0.0);

// Define exchange variables
float avrSWAP[AVRSIZE];
int istat = 1;
int *aviFAIL = &istat;
const char accINFILE[] = "DISCON.IN";
const char avcOUTNAME[] = "SOWFA Controller Simulation";
char avcMSG[BUFFERSIZE];

for(int i=0; i<= AVRSIZE; i++)
{
    avrSWAP[i] = 0.0;
}

// Define some avrSWAP parameters
scalar t = runTime.value();
Info<< "dt = " << dt << endl;
avrSWAP[2] = dt;
avrSWAP[60] = 3; // number of blades
avrSWAP[20] = rotSpd[0]; // initial rotation speed [rad/s]
avrSWAP[26] = wspd[0]; // initial hub wind speed [m/s]
avrSWAP[48] = BUFFERSIZE; // size of avcMSG
avrSWAP[49] = sizeof(accINFILE) / sizeof(*accINFILE); // size of accINFILE
avrSWAP[50] = sizeof(avcOUTNAME) / sizeof(*avcOUTNAME); // size of avcOUTNAME

// Initialize controller
avrSWAP[0] = 0; // Status flag: 0 is first call, 1 for all subsequent time steps, -1 for final call
#include "callController.H"

avrSWAP[0] = 1;

if(dbg)
{
    OFstream avr0File("avrSWAP0");
    for(int i=0; i<AVRSIZE; i++)
    {
        avr0File << avrSWAP[i] << endl;
    }
}

