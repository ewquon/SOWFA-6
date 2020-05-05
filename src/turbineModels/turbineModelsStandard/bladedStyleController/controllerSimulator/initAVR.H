// Define saved quantities for verification
const scalar dt = runTime.deltaTValue();
const label N = runTime.endTime().value() / dt;
List<scalar> rotSpd(N+1, 0.0);
List<scalar> genTq(N+1, 0.0);
List<scalar> blPitch(N+1, 0.0);
List<scalar> rotSpdErr;
List<scalar> genTqErr;
List<scalar> blPitchErr;

rotSpd[0] = rotSpdTable->value(0.0);
genTq[0] = genTqTable->value(0.0);
blPitch[0] = blPitchTable->value(0.0);

// Define exchange variables
float avrSWAP[AVRSIZE];
int aviFAIL[] = {1};
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
avrSWAP[20] = rotSpdTable->value(0); // initial rotation speed [rad/s]
avrSWAP[26] = wspdTable->value(0); // initial hub wind speed [m/s]

const int char_buffer = 500; // TODO: IS THIS CORRECT?
avrSWAP[0] = 0; // Status flag: 0 is first call, 1 for all subsequent time steps, -1 for final call
avrSWAP[58] = BUFFERSIZE; // TODO: IS THIS A TYPO (should be 48)?
avrSWAP[48] = BUFFERSIZE; // size of avcMSG??
avrSWAP[49] = sizeof(accINFILE) / sizeof(*accINFILE); // size of accINFILE
avrSWAP[50] = sizeof(avcOUTNAME) / sizeof(*avcOUTNAME); // size of avcOUTNAME
avrSWAP[51] = BUFFERSIZE; // TODO: IS THIS USED?

// Initialize controller
DISCON(avrSWAP, aviFAIL, accINFILE, avcOUTNAME, avcMSG);

avrSWAP[0] = 1;

