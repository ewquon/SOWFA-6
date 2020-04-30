# Specified Wind History

This small test case specifies a uniform wind with sinusoidally varying wind
direction over time. These are enforced on a periodic domain using the driving-
force controller capability, activated in `constant/ABLProperties` by setting 
the source type to "computed". In this case, there is neutral stratification
with negligible Coriolis force.

## Dependencies
These instructions assume starting from scratch a clean environment. If you
have an existing conda environment that you're happy with, feel free to skip
step 5.

1. `cd /path/to/local/python/libraries` : Where the mmctools library will live
2. ``export PYTHONPATH="`pwd`:$PYTHONPATH"`` : Add location to Python path
3. `git clone https://github.com/a2e-mmc/mmctools.git` : Get mmctools library
4. `cd mmctools && checkout dev` : Switch to the devlopment branch
5. `conda create -n mmc python=3.7` : Create a new environment (optional)
6. `conda activate mmc` : Switch to the new (or an existing) environment
7. `conda update --file environment.yaml` : Install dependencies

## Running the case
This is intended to be a very small test case, to be run quickly on 4 cores.

1. `./generate_fieldTable.py`:

    - Creates a wind time history, saved as `constant/fieldTable`.

2. `./runscript.preprocess.py`:

    - Generates `blockMesh`
    - Renumbers the mesh for computational efficiency
    - Decomposes the mesh for parallel computation

3. `sbatch runscript.solve.1`

