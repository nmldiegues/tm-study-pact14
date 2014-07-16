#!/bin/sh

FOLDERS="genome intruder intruder-locks kmeans kmeans-locks labyrinth ssca2 ssca2-locks vacation yada redblacktree hashmap hashmap-locks"

rm lib/*.o || true

rm lib/tm.h
rm common/Defines.common.mk
rm common/Makefile
rm lib/thread.h
rm lib/thread.c

cd ssync
make clean
cd ..

for F in $FOLDERS
do
    cd $F
    rm *.o || true
    rm $F
    cd ..
done
