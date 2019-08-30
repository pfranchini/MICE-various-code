# Create a directory and download the correspondent geometry 
#
# Usage: ./do_geometry.sh <run_number>
#

run=$1

mkdir geo-$run
python ${MAUS_ROOT_DIR}/bin/utilities/download_geometry.py --geometry_download_by run_number --geometry_download_run_number $run  --geometry_download_directory=geo-$run
#python download_geometry.py --geometry_download_by run_number --geometry_download_run_number $run  --geometry_download_directory=geo-$run
