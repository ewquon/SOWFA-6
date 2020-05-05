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

// Turbine properties
const dictionary& turbineDict(controllerVerificationDict.subDict("turbine"));
const float R(readScalar(turbineDict.lookup("rotorRadius"))); // [m]
const float Ng(readScalar(turbineDict.lookup("Ng"))); // gearbox ratio
const float J(readScalar(turbineDict.lookup("J"))); // rotor inertia [kg-m^2]
const float genEff(readScalar(turbineDict.lookup("genEff"))/100.0); // generator efficiency [%]
const float rho(readScalar(turbineDict.lookup("rho"))); // air density [kg/m^3]

// Rotor performance
const List<scalar> refPitch(turbineDict.lookup("pitch"));
const List<scalar> refTSR(turbineDict.lookup("TSR"));
const List<List<scalar> > refCq(turbineDict.lookup("Cq"));


// Driving & verification data
autoPtr<Function1<scalar>> wspdTable = Function1<scalar>::New("windSpeed", controllerVerificationDict);
autoPtr<Function1<scalar>> rotSpdTable = Function1<scalar>::New("rotorSpeed", controllerVerificationDict);
autoPtr<Function1<scalar>> genTqTable = Function1<scalar>::New("generatorTorque", controllerVerificationDict);
autoPtr<Function1<scalar>> blPitchTable = Function1<scalar>::New("bladePitch", controllerVerificationDict);

Info<< "Initial conditions:"
    << " windSpeed=" << wspdTable->value(0)
    << " rotorSpeed=" << rotSpdTable->value(0)
    << " generatorTorque=" << genTqTable->value(0)
    << " bladePitch=" << blPitchTable->value(0)
    << endl;

rotSpd.append(rotSpdTable->value(0));
genTq.append(genTqTable->value(0));
blPitch.append(blPitchTable->value(0));

