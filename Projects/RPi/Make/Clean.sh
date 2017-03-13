if [ "$1" = debug ]; then
    make -f Chillisource.mk clean DEBUG=1 ;
    make -f Application.mk clean DEBUG=1
elif [ "$1" = release ]; then
    make -f Chillisource.mk clean DEBUG=0 ;
    make -f Application.mk clean DEBUG=0
else
    echo "No target scheme specified (debug/release)"
fi
