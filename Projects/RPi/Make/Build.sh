#!/bin/bash

# 1. Clean the output files so make knows to try and remake
# 2. Make the exe
# 3. Copy the assets and libs ready for packaging
if [ "$1" = debug ]; then
    make cleangenerated -f ChilliSource.mk DEBUG=1 ;
    make cleangenerated -f Application.mk DEBUG=1 ;
    make -f Chillisource.mk DEBUG=1 &&
    make -f Application.mk DEBUG=1 &&
    python "../../../../ChilliSource/Tools/Scripts/copy_rpi_resources.py" "../../../../Projects/RPi/" "../../../../Projects/RPi/Output/$1"
elif [ "$1" = release ]; then
    make cleangenerated -f ChilliSource.mk DEBUG=0 ;
    make cleangenerated -f Application.mk DEBUG=0 ;
    make -f Chillisource.mk DEBUG=0 &&
    make -f Application.mk DEBUG=0 &&
    python "../../../../ChilliSource/Tools/Scripts/copy_rpi_resources.py" "../../../../Projects/RPi/" "../../../../Projects/RPi/Output/$1"
else
    echo "No target scheme specified (debug/release)"
fi
