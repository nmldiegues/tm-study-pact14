#!/bin/sh

FOLDERS="genome intruder kmeans labyrinth ssca2 vacation yada redblacktree hashmap"

bash clean.sh

echo "Configuring $1"
cp backends/$1/tm.h lib/tm.h
cp backends/$1/Defines.common.mk common/Defines.common.mk
cp backends/$1/Makefile common/Makefile
cp backends/$1/thread.h lib/thread.h
cp backends/$1/thread.c lib/thread.c

alias=$2

if [[ $# != 3 ]] ; then
    prob=10
else
    prob=$3
fi

if [[ $# != 1 ]] ; then
    cd ssync
    make LOCK_VERSION="-DUSE_$alias"
    rc=$?
    if [[ $rc != 0 ]] ; then
         echo ""
         echo "=================================== ERROR BUILDING with lock $alias ==================================="
         echo ""
         exit 1
    fi
    cd ..;
fi

for F in $FOLDERS
do
    cd $F
    make -f Makefile LOCK_VERSION="-DUSE_$alias" PROB="-DFALLBACK_PROB=$prob"
    rc=$?
    if [[ $rc != 0 ]] ; then
	echo ""
        echo "=================================== ERROR BUILDING $F with lock $alias ===================================="
	echo ""
        exit 1
    fi
    cd ..
done
