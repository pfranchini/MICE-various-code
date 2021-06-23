#!/bin/bash
#

###############################################################

DO_STRAIGHT=1
DO_HELICAL=0

#LOCAL_DIR=/home/hep/ch308/work/My_MAUS/SystemsPaperResults
LOCAL_DIR=/home/hep/pfranchi/MICE/Tracker/SystemsPaperResults

HELICAL_RUNS=$LOCAL_DIR/helical_run_numbers.dat
#STRAIGHT_RUNS=$LOCAL_DIR/straight_run_numbers.dat
STRAIGHT_RUNS=$LOCAL_DIR/straight_run_numbers_3-240+M3-Test1.dat

###############################################################


INIT="source $MAUS_ROOT_DIR/env.sh"

#ANALYSIS_CODES="H25c H36aa H36c H36d"
#HELICAL_RUNS=$(cat $LOCAL_DIR/helical_run_numbers.dat | awk '{print $0}')


TERM_OLD=$TERM
unset TERM

if [ -z $MAUS_ROOT_DIR ]
then
  echo "MAUS_ROOT_DIR is not set."
  echo "Please source a MAUS environment."
  echo
  exit 0
fi


# Defined by PF based on TOF1/2 plots
declare -A TOF_CUTS
TOF_CUTS[3,140]="--tof1_cut 29.0 31.0 --tof2_cut 34.0 38.0"
TOF_CUTS[6,140]="--tof1_cut 29.0 30.5 --tof2_cut 34.0 38.0"
TOF_CUTS[10,140]="--tof1_cut 28.0 29.3 --tof2_cut 34.0 38.0"

TOF_CUTS[3,170]="--tof1_cut 28.0 30.0 --tof2_cut 32.0 37.0"
TOF_CUTS[6,170]="--tof1_cut 28.0 28.9 --tof2_cut 31.5 33.5"
TOF_CUTS[10,170]="--tof1_cut 27.0 28.2 --tof2_cut 30.1 34.0"

TOF_CUTS[3,200]="--tof1_cut 27.6 28.8 --tof2_cut 31.1 32.7"
TOF_CUTS[6,200]="--tof1_cut 27.4 28.2 --tof2_cut 30.5 32.0"
TOF_CUTS[10,200]="--tof1_cut 27.1 27.8 --tof2_cut 30.5 32.5"

#TOF_CUTS[3,240]="--tof1_cut 27.0 27.8 --tof2_cut 30.0 31.2"
TOF_CUTS[3,240]="--tof1_cut 27.0 27.8 --tof2_cut 29.6 31.0"
TOF_CUTS[6,240]="--tof1_cut 27.0 27.6 --tof2_cut 30.0 31.0"
TOF_CUTS[10,240]="--tof1_cut 26.6 27.3 --tof2_cut 29.6 31.0"

if [[ "DO_HELICAL" -eq "1" ]]
then
    
    ## Helical:
    
    echo "Helicals..."

    while read line
    do
	run=$(echo $line | cut -f1 -d" ")
	beamline=$(echo $line | cut -f2 -d" ")
	emittance=$(echo $line | cut -f3 -d" ")
	momentum=$(echo $line | cut -f4 -d" ")
	
	echo $run
	
	tof_cut=${TOF_CUTS[$emittance,$momentum]}
	data=$(wmd -Q find --run_numbers $run)
	
	if [ -z $data ]
	then
	    echo "NO DATA"
	    continue
	fi
	
	echo $data
	sub short "$INIT && python $LOCAL_DIR/efficiency_analysis_BW.py $tof_cut -D $LOCAL_DIR/data/historical_efficiencies -O $run $data"
	echo "python $LOCAL_DIR/efficiency_analysis_BW.py $tof_cut -D $LOCAL_DIR/data/historical_efficiencies -O $run $data"

    done < $HELICAL_RUNS
fi

if [[ "DO_STRAIGHT" -eq "1" ]]
then

    ## Straight:
    
    echo "Straights..."
    
    while read line
    do
	run=$(echo $line | cut -f1 -d" ")
	beamline=$(echo $line | cut -f2 -d" ")
	emittance=$(echo $line | cut -f3 -d" ")
	momentum=$(echo $line | cut -f4 -d" ")
	
	echo $run
	
	tof_cut=${TOF_CUTS[$emittance,$momentum]}
	data=$(wmd -Q find --run_numbers $run)
	
	if [ -z $data ]
	then
	    echo "NO DATA"
	    continue
	fi
	
	sub short "$INIT && python $LOCAL_DIR/efficiency_analysis_BW.py $tof_cut -D $LOCAL_DIR/data/historical_efficiencies -O $run $data"
	echo "$LOCAL_DIR/efficiency_analysis_BW.py $tof_cut -D $LOCAL_DIR/data/historical_efficiencies -O $run $data"
	
	echo
	
    done < $STRAIGHT_RUNS
    
fi
  

export TERM=$TERM_OLD
    



