#!/bin/bash

# how to detect specified distribution like xenial, focal, ...

codename=`lsb_release -c|awk '{print $2}'`

if [ $codename == "xenial" ]; then
	echo xenial detected
else
	echo sorry, not xenial
fi
