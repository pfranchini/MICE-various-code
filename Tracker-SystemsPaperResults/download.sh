#!/bin/bash  

# Downloader for the IC MICE shared repository

run=`printf %05d $1`
MAUS="3.3.2"
DIRECTORY=/vols/mice/mice-data/real-recon-data/MAUS-v$MAUS/${run::-2}00/$run

echo -e "\n$run"

if [ ! -d "$DIRECTORY" ]; then

    mkdir -p $DIRECTORY
    wget http://gfe02.grid.hep.ph.ic.ac.uk:8301/RECO/MAUS-v$MAUS/1/Step4/${run::-2}00/${run}_offline.tar -O $DIRECTORY/${run}_offline.tar

    cd /vols/mice/mice-data/real-recon-data/MAUS-v$MAUS/${run::-2}00/$run
    tar xzf $DIRECTORY/${run}_offline.tar
    cd -

else

    echo "Already present!"

fi

ls -l $DIRECTORY/*.root

