#!/bin/bash
if [ $# -eq 0 ]
then
	echo "Usage: ./new.sh [PROJECT-NAME]"
	exit 1
fi

mkdir $1
mkdir $1/src
cp -r sample/src/main.c $1/src/main.c
echo -e "PROJECT := $1\n\ninclude ../../lib/lib.mk" > $1/Makefile
