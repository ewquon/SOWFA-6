#!/usr/bin/env python
import os, sys
import numpy as np
import pandas as pd
import xarray as xr

from windtools.openfoam import InputFile # for reading in setUp (optional)
from mmctools.coupling.sowfa import BoundaryCoupling
from mmctools.coupling.domain import Domain

# SETUP

casedir = '.'
output_fields = {
    'U': ('u','v','w'),
}
output_format = {
    'binary': True, # changing this to ascii or ascii+gzip will require changing
    'gzip': False,  # the format parameter in 0.original/U to "ascii"
}

wspd = 10.
angmax = 30.
drivingfreq = 10. # interval between boundaryData planes [s]
period = 600.
simlen = 1800. # simulation length [s]

# Note: The start time here is arbitrary since the simulation time doesn't
#       actually correspond to a real datetime.
sowfastart = '2000-01-01 00:00'

csvout = 'bkgwind_for_paraview' # for debug
csvperiod = 10.0 # should match sliceDataInstanenous writeInterval

#==============================================================================
#
# Execution starts here
#

outdir = os.path.join(casedir,'constant','boundaryData')

sowfa = InputFile(os.path.join(casedir,'setUp'))
sowfadom = Domain(
    xmin=sowfa['xMin'], xmax=sowfa['xMax'],
    ymin=sowfa['yMin'], ymax=sowfa['yMax'],
    zmin=sowfa['zMin'], zmax=sowfa['zMax'],
    nx=sowfa['nx'], ny=sowfa['ny'], nz=sowfa['nz'],
)
print(sowfadom)

# Create dataset

t = np.arange(0, simlen+drivingfreq, drivingfreq)
sowfastart = pd.to_datetime(sowfastart)
datetime = sowfastart + pd.to_timedelta(t, unit='s')

ds = xr.Dataset(
    coords={
        'datetime':datetime,
        'x':[sowfadom.xmin, sowfadom.xmax],
        'y':[sowfadom.ymin, sowfadom.ymax],
        'height': sowfadom.z,
    }
)

## - Setup wind vector
ang = angmax * np.sin(2*np.pi*t/period)
u = wspd * np.cos(np.pi/180*ang)
v = wspd * np.sin(np.pi/180*ang)
ds['u'] = ('datetime', u)
ds['v'] = ('datetime', v)
ds['w'] = 0 * ds['u']
print('Generated boundary dataset:')
print(ds)

## - Write out wind time history
w = np.zeros(u.shape)
df = pd.DataFrame(data={'u':u,'v':v,'w':w}, index=pd.Index(t,name='Time'))
df = df.reindex(np.arange(0,simlen+csvperiod,csvperiod))
df = df.interpolate(method='index')
df.to_csv('bkgwind.csv')
if not os.path.isdir(csvout):
    os.makedirs(csvout)

## - Write out background wind as a series
##   (to plot wind vector in paraview)
print('Writing out background wind as a csv series (for ParaView)...')
xout = sowfa['xMin']
zout = 2*sowfa['zMax']
for i,(ti,row) in enumerate(df.iterrows()):
    if i==0: continue
    with open(os.path.join(csvout,'bkgwind_{:05d}.csv'.format(i)),'w') as f:
        f.write('x,y,z,u,v,w\n{:g},0,{:g},{:g},{:g},{:g}'.format(xout,zout,*row.values))

# Write out boundaryData

BoundaryCoupling(outdir,
    ds.sel(x=sowfadom.xmin),
    name='west',
    dateref=sowfastart
).write(output_fields, **output_format)

BoundaryCoupling(outdir,
    ds.sel(x=sowfadom.xmax),
    name='east',
    dateref=sowfastart
).write(output_fields, **output_format)

BoundaryCoupling(outdir,
    ds.sel(y=sowfadom.ymin),
    name='south',
    dateref=sowfastart
).write(output_fields, **output_format)

BoundaryCoupling(outdir,
    ds.sel(y=sowfadom.ymax),
    name='north',
    dateref=sowfastart
).write(output_fields, **output_format)

# Write out cell-centered velocity field to include in 0.original/U
#
# Note: We assume here that the cells are predictably ordered after running
#       blockMesh. The user should verify the equivalence of the cell-center
#       locations expected by this script (output as 'assumed_blockmesh_cc.csv')
#       with output from the writeCellCenters utility, called after running
#       blockMesh. 

## - Convert domain points to cell centers
x1 = (sowfadom.x[1:] + sowfadom.x[:-1]) / 2
y1 = (sowfadom.y[1:] + sowfadom.y[:-1]) / 2
z1 = (sowfadom.z[1:] + sowfadom.z[:-1]) / 2
x,y,z = np.meshgrid(x1,y1,z1,indexing='ij')
x = x.ravel(order='F')
y = y.ravel(order='F')
z = z.ravel(order='F')

csvfile = os.path.join(casedir,'constant','assumed_blockmesh_cc.csv')
pd.DataFrame({'x':x,'y':y,'z':z}).to_csv(csvfile,index=False)
print('Wrote',csvfile)

