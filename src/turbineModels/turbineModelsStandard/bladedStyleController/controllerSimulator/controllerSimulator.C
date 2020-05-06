/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    controllerSimulator

Description
    This simulator reads in verification_data.csv which contains the following
    columns: time, wind speed, rotor speed, generator torque, and blade pitch.
    The first two columns drive the simulation and the first row provides
    initial conditions.

    The simulated time history is compared against the provided history of
    rotor speed, generator torque, and blade pitch.

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "IOdictionary.H"
#include "Function1.H"
#include "OFstream.H"
#include "interpolate2D.H"

//#include "bladedStyleController.H"
extern "C" {
    #include "DISCON.H"
}

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"

    #include "readData.H"
    #include "initAVR.H"

    while(runTime.run())
    {
        runTime++;
        t = runTime.value();
        Info<< "Time = " << t << " " << endl;
        const label itime = runTime.timeIndex();
        wspd[itime] = wspdTable->value(runTime.value());
        Info<< "  ws = " << wspd[itime] << endl;

        // Load current Cq data
        const scalar tsr = rotSpd[itime-1] * R / wspd[itime];
        Info<< "  tsr = " << tsr << endl;
        const scalar Cq = interpolate2D(blPitch[itime-1], tsr,
                                        refPitch,
                                        refTSR,
                                        refCq);
        Info<< "  Cq = " << Cq << endl;

        // Update the turbine state
        const scalar aeroTq = 0.5 * rho*wspd[itime]*wspd[itime] * R * (M_PI * R * R) * Cq;
        Info<< "  aeroTq = " << aeroTq << endl;
        rotSpd[itime] = rotSpd[itime-1] 
                      + (dt/J)*(aeroTq*genEff - Ng*genTq[itime-1]);
        const scalar genSpd = rotSpd[itime] * Ng;
        Info<< "  rotSpd = " << rotSpd[itime] << endl;

        // Call the controller
        #include "callController.H"
        blPitch[itime] = avrSWAP[41];  // TODO: where do these controller states come from?
        genTq[itime] = avrSWAP[46];
        Info<< "  blPitch = " << blPitch[itime] << endl;
        Info<< "  genTq = " << genTq[itime] << endl;

        // Save errors
        rotSpdErr.append(rotSpd[itime] - rotSpdTable->value(t));
        genTqErr.append(genTq[itime] - genTqTable->value(t));
        blPitchErr.append(blPitch[itime] - blPitchTable->value(t));
    }

    // Check errors
    #include "calcErrorStats.H"

    fileName postProcDir = runTime.path()/"postProcessing";
    Info << "Writing output to " << postProcDir << endl;
    if (!isDir(postProcDir))
    {
        mkDir(postProcDir);
    }
    OFstream wspdFile(postProcDir/"windSpeed");
    wspdFile << wspd;
    OFstream rotSpdFile(postProcDir/"rotorSpeed");
    rotSpdFile << rotSpd;
    OFstream genTqFile(postProcDir/"generatorTorque");
    genTqFile << genTq;
    OFstream blPitchFile(postProcDir/"bladePitch");
    blPitchFile << blPitch;

    Info<< "\nEnd\n" << endl;

    return 0;
}


// ************************************************************************* //
