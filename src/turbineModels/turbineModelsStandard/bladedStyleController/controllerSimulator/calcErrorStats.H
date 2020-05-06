Info << endl;

scalar rotSpdErrMax(-1);
scalar genTqErrMax(-1);
scalar blPitchErrMax(-1);
scalar rotSpdErrMaxTime;
scalar genTqErrMaxTime;
scalar blPitchErrMaxTime;
forAll(rotSpdErr, itime)
{
    if(mag(rotSpdErr[itime]) > rotSpdErrMax)
    {
        rotSpdErrMax = mag(rotSpdErr[itime]);
        rotSpdErrMaxTime = itime * dt;
    }
    if(mag(genTqErr[itime]) > genTqErrMax)
    {
        genTqErrMax = mag(genTqErr[itime]);
        genTqErrMaxTime = itime * dt;
    }
    if(mag(blPitchErr[itime]) > blPitchErrMax)
    {
        blPitchErrMax = mag(blPitchErr[itime]);
        blPitchErrMaxTime = itime * dt;
    }
}
Info<< "Max rotor speed error      : " << rotSpdErrMax
    << " (at t= " << rotSpdErrMaxTime << ")"
    << endl;
Info<< "Max generator torque error : " << genTqErrMax
    << " (at t= " << genTqErrMaxTime << ")"
    << endl;
Info<< "Max blade pitch error      : " << blPitchErrMax
    << " (at t= " << blPitchErrMaxTime << ")"
    << endl;

