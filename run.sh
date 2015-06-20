#!/bin/sh

toxaemia_src/Toxaemia &
sleep 1
xwintox_src/XwinTox &

wait %1
wait %2
