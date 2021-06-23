source /vols/build/mice/shared/source_me

original: /home/hep/ch308/work/My_MAUS/SystemsPaperResults

===============================================

Tracker track finding historic efficiency:
-----------------------------------------

* find_run_numbers.py

* analyse_historic.sh
  (e.g. --> python /home/hep/pfranchi/MICE/Tracker/SystemsPaperResults/efficiency_analysis_BW.py --tof1_cut 0.0 40.0 --tof2_cut 0.0 50.0 -D /home/hep/pfranchi/MICE/Tracker/SystemsPaperResults/data/historical_efficiencies -O 09900 /vols/mice/mice-data/real-recon-data/MAUS-v3.0.1/09900/09900/09900_recon.root )
  produces for each run on root file with the plots and one json file with the data (efficiencies, bad events, ...)

* compare_historic.py:
  python compare_historic.py --helical helical_run_numbers.dat --straight straight_run_numbers.dat data/historical_efficiencies/

* root plot_historical.c 



Tracker resolution plots:
------------------------

* /vols/build/mice/shared/bin/print_list_virtual_planes.py -P /vols/mice/pfranchi/MC/3-140/*_sim.root -O virtual_planes
106 virtual planes

* $MAUS_ROOT_DIR/bin/scifi/tracker_resolution_plots.py -V virtual_planes.json -O test /vols/mice/mice-data/mc-recon-data/grid/MAUS-v2.9.1/08600/08681/0099?_sim.root

Recent MC:
* $MAUS_ROOT_DIR/bin/scifi/tracker_resolution_plots.py -V virtual_planes.json -O test /vols/mice/pfranchi/MC/3-140/000*_sim.root /vols/mice/pfranchi/MC/3-170/000*_sim.root /vols/mice/pfranchi/MC/3-200/000*_sim.root /vols/mice/pfranchi/MC/3-240/000*_sim.root

* root plot_res.c

===============================================

I've tarred my entire analysis directory. You now get to see what my analyses typically look like! Just copy it over from:
/vols/mice/SystemsPaperResults.tar

There are two things to use this stuff:
1. It expects the environment to be configured by the IC MICE code:
    source /vols/build/mice/shared/source_me
Its not strictly necessary, but there are scripts that it uses that aren't in MAUS, and it tracks my analysis development branch so it has bug fixes, etc, that haven't made it to the trunk yet.

2. Main entry points are all executable bash scripts. They have a variable called "LOCAL_DIR" which defines the directory in which the script resides. This allows for absolute paths to be used for everything. So you will have to change this variable to the location of your version.

Sorry this is a bit messy. There are few things there you can explore, but the one of interest is:
    analyse_historic.sh

This submits all the batch analyses, using two data files to list the runs of interest.

The python script:
    find_run_numbers.py
populates these data files. This is where we were seeing the CDB errors.

Finally the python script:
    compare_historic.py
creates the final plots.  I think you might have to have a little play to get everything working. Let me know if you hit any issues, shouldn't be too bad to get it running.
