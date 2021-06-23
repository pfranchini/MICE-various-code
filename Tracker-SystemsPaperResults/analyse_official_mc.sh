#!/bin/bash
#

LOCAL_DIR=/home/hep/ch308/work/My_MAUS/SystemsPaperResults

CHISQ="100.0"

MC_DATA="/vols/mice/ch308/MC_Data"

INIT="source $MAUS_ROOT_DIR/env.sh"

declare -A FILE_ARRAY
FILE_ARRAY[09952]="09952/*_sim.root"
FILE_ARRAY[09959]="09959/*_sim.root"
FILE_ARRAY[09964]="09964/*_sim.root"

RUN_NUMBERS="09952 09959 09964"


if [ -z $MAUS_ROOT_DIR ]
then
  echo "MAUS_ROOT_DIR is not set."
  echo "Please source a MAUS environment."
  echo
  exit 0
fi


echo
echo "Analysising MC"
echo

for run_number in $RUN_NUMBERS
do

  echo $run_number
  echo

  data=$MC_DATA/${FILE_ARRAY[$run_number]}

#  sub medium "$INIT && python $MAUS_ROOT_DIR/bin/scifi/tracker_resolution_plots.py --track_algorithm 1 --trackers 0 1 -D $LOCAL_DIR/data/resolutions/ -O resolutions_$run_number $data"

  echo "python $MAUS_ROOT_DIR/bin/scifi/tracker_resolution_plots.py --track_algorithm 1 --trackers 0 1 -D $LOCAL_DIR/data/resolutions/ -O resolutions_$run_number $data"

  echo
done

