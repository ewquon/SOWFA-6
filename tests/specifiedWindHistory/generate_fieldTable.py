#!/usr/bin/env python
import os, sys
import numpy as np
import pandas as pd

from mmctools.coupling.sowfa import InternalCoupling

# SETUP

wspd = 10. # this should match initial condition defined by U0 in setUp
angmax = 30.
drivingfreq = 10. # interval between boundaryData planes [s]
simlen = 1200. # simulation length [s]

#==============================================================================
#
# Execution starts here
#

# setup driving wind 
t = np.arange(0, simlen+drivingfreq, drivingfreq)
ang = angmax * np.sin(2*np.pi*t/simlen)
u = wspd * np.cos(np.pi/180*ang)
v = wspd * np.sin(np.pi/180*ang)
w = np.zeros(u.shape)

# create dataframe with driving data
df = pd.DataFrame({'u':u,'v':v,'w':w}, index=t)
df['height'] = 90.0  # only specify a single level ==> forcing is uniform with height

# write out file to be used in constant/ABLProperties
to_sowfa = InternalCoupling('constant',df)
to_sowfa.write_timeheight('fieldTable',xmom='u',ymom='v',zmom='w')

