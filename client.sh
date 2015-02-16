#!/bin/bash

### FIXME variables

set -e
set -v

SERVER="$1"
PORT="$2"

if [ -z "$SERVER" ] ; then
	SERVER="10.54.5.129"
fi
if [ -z "$PORT" ] ; then
	PORT="6666"
fi

while true; do
	LOADWHAT=$(echo "LOADWHAT" | netcat -vvn -q1 $SERVER $PORT)
	if [ -z "$LOADWHAT" ] ; then
		echo "FINISHED"
		exit 0
	fi
	echo "modprobe'ing $LOADWHAT"
	modprobe -v $LOADWHAT
	( echo "LOADED" ; cat /proc/modules ) | netcat -vvn -q1 $SERVER $PORT
	echo "REBOOTING"
	echo reboot > /sys/power/disk
	echo disk > /sys/power/state
	echo "Sleeping a bit for the network to get back up"
	sleep 1 # wait for network to recover
	echo "Sending known good modules"
	( echo "KNOWNGOOD" ; cat /proc/modules ) | netcat -vvn -q1 $SERVER $PORT
done
