#!/bin/sh

# IRIXuser added - delete if non-applicable #
export LD_LIBRARY_PATH=/ws/tox/staging/lib

case "$PWD" in
*xwintox_src*)
	cd ..
	;;
*) ;;
esac

sigint()
{
    for proc in `jobs -p`
    do
        kill $proc
    done
}

trap sigint SIGINT

toxaemia_src/Toxaemia &
sleep 1
xwintox_src/XwinTox &

wait %1
wait %2
