avrSWAP[1] = t; // current time [s]
avrSWAP[2] = dt; // communication interval [s]
avrSWAP[3] = blPitch[itime-1]; // blade 1 pitch angle [rad]
avrSWAP[32] = blPitch[itime-1]; // blade 2 pitch angle [rad]
avrSWAP[33] = blPitch[itime-1]; // blade 3 pitch angle [rad]
avrSWAP[14] = genSpd * genTq[itime-1] * genEff; // measured electrical power output [W]
avrSWAP[22] = genTq[itime-1]; // measured generator torque [N-m]
avrSWAP[19] = genSpd; // measured generator speed [rad/s]
avrSWAP[20] = rotSpd[itime]; // measured rotor speed [rad/s]
avrSWAP[26] = ws; // hub wind speed [m/s]

DISCON(avrSWAP, aviFAIL, accINFILE, avcOUTNAME, avcMSG);

if(aviFAIL != 0)
{
    for(int i=0; i <= BUFFERSIZE; i++)
    {
        if(avcMSG[i] == '\0') break;
        Info << avcMSG[i];
    }
    Info << "(error code: " << aviFAIL << ")" << endl;
}

