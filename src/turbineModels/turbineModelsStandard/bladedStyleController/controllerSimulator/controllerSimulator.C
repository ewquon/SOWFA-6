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

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
//    timeSelector::addOptions();
//    #include "addRegionOption.H"
    #include "setRootCase.H"
    #include "createTime.H"
//    instantList timeDirs = timeSelector::select0(runTime, args);

//    #include "createNamedPolyMesh.H"

    IOdictionary controllerVerificationDict
    (
        IOobject
        (
            "controllerVerificationDict",
            runTime.time().system(),
            runTime,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );
    autoPtr<Function1<scalar>> wspd = Function1<scalar>::New("windSpeed", controllerVerificationDict);
    autoPtr<Function1<scalar>> rotSpd = Function1<scalar>::New("rotorSpeed", controllerVerificationDict);
    autoPtr<Function1<scalar>> genTq = Function1<scalar>::New("generatorTorque", controllerVerificationDict);
    autoPtr<Function1<scalar>> blPitch = Function1<scalar>::New("bladePitch", controllerVerificationDict);

    Info<< "Initial conditions:"
        << " windSpeed=" << wspd->value(0)
        << " rotorSpeed=" << rotSpd->value(0)
        << " generatorTorque=" << genTq->value(0)
        << " bladePitch=" << blPitch->value(0)
        << endl;

    while(runTime.run())
    {
        runTime++;
        //Info<< 
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
