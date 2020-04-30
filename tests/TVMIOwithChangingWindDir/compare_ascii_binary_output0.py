#!/usr/bin/env python
import numpy as np
import struct

a = {}
b = {}

def readdata(f,N):
    assert (f.read(1) == b'(')
    rawdata = f.read(N*3*8)
    assert (f.read(3) == b');\n')
    data = struct.unpack('{:d}d'.format(N*3), rawdata)
    return np.array(data).reshape((N,3))

def readboundary(f,name,binary=False):
    line = f.readline()
    print(line)
    assert (line.decode('utf-8').strip() == name)
    f.readline() # {
    f.readline() # type
    if binary:
        f.readline() # format
    assert (f.readline().decode('utf-8').split()[0] == 'refValue')
    N = int(f.readline())
    refValue = readdata(f,N)
    assert (f.readline().decode('utf-8').split()[0] == 'valueFraction')
    assert (f.readline().decode('utf-8').split()[0] == 'value')
    N = int(f.readline())
    value = readdata(f,N)
    assert np.all(value == refValue)
    f.readline()
    return value

with open('0.save/U','rb') as f:
    for _ in range(19): f.readline()
    name = f.readline().split()[0].decode('utf-8')
    assert (name == 'internalField')
    N = int(f.readline())
    a[name] = readdata(f,N)
    f.readline()
    assert (f.readline() == b'boundaryField\n')
    for _ in range(9): f.readline()
    a['west'] = readboundary(f,'west')
    a['east'] = readboundary(f,'east')
    a['north'] = readboundary(f,'north')
    a['south'] = readboundary(f,'south')

with open('0/U','rb') as f:
    for _ in range(19): f.readline()
    name = f.readline().split()[0].decode('utf-8')
    assert (name == 'internalField')
    N = int(f.readline())
    b[name] = readdata(f,N)
    f.readline()
    assert (f.readline() == b'boundaryField\n')
    for _ in range(9): f.readline()
    b['west'] = readboundary(f,'west',binary=True)
    b['east'] = readboundary(f,'east',binary=True)
    b['north'] = readboundary(f,'north',binary=True)
    b['south'] = readboundary(f,'south',binary=True)

for key in a.keys():
    print(key, np.max(np.abs(a[key]-b[key])))
    assert np.allclose(a[key], b[key])
