#!/bin/bash

### FIXME variables

set -e
set -v

SERVER=10.54.5.129
PORT=6666

while true; do
	LOADWHAT=$(echo "LOADWHAT" | netcat -vv -q1 $SERVER $PORT)
	if [ -z "$LOADWHAT" ] ; then
		echo "FINISHED"
		exit 0
	fi
	modprobe -v $LOADWHAT
	( echo "LOADED" ; cat /proc/modules ) | netcat -vv -q1 $SERVER $PORT
	echo reboot > /sys/power/disk
	echo disk > /sys/power/state
	sleep 10 # wait for network to recover
	( echo "KNOWNGOOD" ; cat /proc/modules ) | netcat -vv -q1 $SERVER $PORT
done
