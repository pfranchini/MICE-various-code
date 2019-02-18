#!/bin/bash
# Runs the FULL simulation of $num jobs on the batch system

###################################################
source ~/MAUS/maus/env.sh 

num=2;     # Number of jobs
start=1;   # First job
setting_name="3-140+M3-Test4"
geometry="10049"
queue="long"

###################################################

stop=$((start + num - 1))

mkdir ${setting_name}
cd ${setting_name}


for i in `eval echo {$start..$stop}`; do

    rm -rf "process_"${i}
    mkdir "process_"${i}
    cd "process_"${i}

    echo "bsub -q $queue python ../../simulate_mice_G4BL.py --setting_name=${setting_name} --d2current=${d2current} > ${setting_name}.log 2>&1; nohup python ../../simulate_beam.py -input_json_file_name G4BLoutput.json -simulation_geometry_filename ../../geo-$geometry/ParentGeometryFile.dat -reconstruction_geometry_filename ../../geo-$geometry/ParentGeometryFile.dat -output_root_file_name ../${setting_name}_sim${i}.root >& sim${i}.log" " "

    bsub -q $queue "python ../../simulate_mice_G4BL.py --setting_name=${setting_name} --d2current=${d2current} > ${setting_name}.log 2>&1; nohup python ../../simulate_beam.py -input_json_file_name G4BLoutput.json -simulation_geometry_filename ../../geo-$geometry/ParentGeometryFile.dat -reconstruction_geometry_filename ../../geo-$geometry/ParentGeometryFile.dat -output_root_file_name ../${setting_name}_sim${i}.root >& sim${i}.log"

    sleep 1
    cd ..

done
