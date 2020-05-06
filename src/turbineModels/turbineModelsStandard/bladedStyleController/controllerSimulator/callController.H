DISCON(avrSWAP, aviFAIL, accINFILE, avcOUTNAME, avcMSG);

if(istat != 0)
{
    for(int i=0; i <= BUFFERSIZE; i++)
    {
        if(avcMSG[i] == '\0') break;
        Info << avcMSG[i];
    }
    Info << endl;
    Info << "(error code: " << istat << ")" << endl;
}

