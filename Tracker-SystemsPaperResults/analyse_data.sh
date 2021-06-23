#!/bin/bash
#

LOCAL_DIR=/home/hep/ch308/work/My_MAUS/SystemsPaperResults

INIT="source $MAUS_ROOT_DIR/env.sh"

#declare -A TOF_CUTS
#TOF_CUTS[09768]="--tof1_cut 27.0 28.0 --tof2_cut 30.5 33.0"
#TOF_CUTS[09952]="--tof1_cut 29.0 31.0 --tof2_cut 34.0 37.0"
#TOF_CUTS[09959]="--tof1_cut 29.0 31.0 --tof2_cut 34.0 37.0"
#TOF_CUTS[09964]="--tof1_cut 27.5 29.5 --tof2_cut 34.0 37.0"
#TOF_CUTS[10510]="--tof1_cut 28.5 30.5 --tof2_cut 34.0 37.0"
#
#RUN_NUMBERS="09768 10510 09952 09959 09964"

declare -A TOF_CUTS
TOF_CUTS[H25c]="--tof1_cut 27.0 28.0 --tof2_cut 30.5 33.0"
TOF_CUTS[H36aa]="--tof1_cut 29.0 31.0 --tof2_cut 34.0 37.0"
TOF_CUTS[H36c]="--tof1_cut 29.0 31.0 --tof2_cut 34.0 37.0"
TOF_CUTS[H36d]="--tof1_cut 27.5 29.5 --tof2_cut 34.0 37.0"
#TOF_CUTS[H61c]="--tof1_cut 28.5 30.5 --tof2_cut 34.0 37.0"

ANALYSIS_CODES="H25c H36aa H36c H36d"


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

#for run_number in $RUN_NUMBERS
#do
#
#  echo $run_number
#  echo
#
##  data=$MC_DATA/${FILE_ARRAY[$run_number]}
#  data=$(wmd -Q find --run_numbers $run_number)
#  tof_cut="${TOF_CUTS[$run_number]}"
#
#  sub short "$INIT && python $LOCAL_DIR/efficiency_analysis_BW.py $tof_cut -D $LOCAL_DIR/data/efficiencies -O $run_number $data"
#
#  echo
#done


for analysis in $ANALYSIS_CODES
do

  echo $run_number
  echo

  data=$(wmd -Q find --analysis $analysis)

  tof_cut="${TOF_CUTS[$analysis]}"

  sub short "$INIT && python $LOCAL_DIR/efficiency_analysis_BW.py $tof_cut -D $LOCAL_DIR/data/efficiencies -O $analysis $data"

  echo
done

