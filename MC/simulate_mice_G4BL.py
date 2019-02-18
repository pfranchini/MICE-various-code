#!/bin/env python

"""
Simulate the MICE experiment

This will simulate MICE spills through the entirety of MICE using Geant4, then
digitize and reconstruct TOF and tracker hits to space points.
"""

import io   # generic python library for I/O
import MAUS # MAUS libraries
import os
import json
import time
import sys
import getopt
import subprocess
import shutil
import fileinput

def get_setup_dict(setting_name, d2current, seed, run_number):

    """
    # Settings for a classic 3mm-200MeV/c muon beam
    g4bl = {"run_number":run_number,"q_1":1.018,"q_2":-1.271,"q_3":0.884,"d_1":-1.242,"d_2":d2current,\
            "d_s":3.666,"particles_per_spill":0,"rotation_angle":0,"translation_z":1000.0,\
            "protonabsorberin":1,"proton_absorber_thickness":83,"proton_number":1E12,"proton_weight":1,\
            "particle_charge":'all',"file_path":'MAUS_ROOT_DIR/src/map/MapPyBeamlineSimulation/G4bl',\
            "get_magnet_currents_pa_cdb":"False","random_seed":seed}


    # Settings for a 3mm-140MeV/c pion beam   
    g4bl = {"run_number":run_number,"q_1":0.500,"q_2":-0.624,"q_3":0.434,"d_1":-0.605 ,"d_2":-0.295,\
            "d_s":1.788,"particles_per_spill":0,"rotation_angle":0,"translation_z":1000.0,\
            "protonabsorberin":1,"proton_absorber_thickness":29,"proton_number":1E12,"proton_weight":1,\
            "particle_charge":'all',"file_path":'G4bl_path_UNUSED',\
            "get_magnet_currents_pa_cdb":"False","random_seed":seed,"WKRDIR":"~/"}

    

    # Settings for a 3mm-240MeV/c pion beam
    g4bl = {"run_number":run_number,"q_1":0.733,"q_2":-0.916,"q_3":0.637,"d_1":-0.920,"d_2":-0.469,\
            "d_s":2.638,"particles_per_spill":0,"rotation_angle":0,"translation_z":1000.0,\
            "protonabsorberin":1,"proton_absorber_thickness":29,"proton_number":1E12,"proton_weight":1,\
            "particle_charge":'all',"file_path":'G4bl_path_UNUSED',\
            "get_magnet_currents_pa_cdb":"False","random_seed":seed,"WKRDIR":"~/"}
    """

    # Settings for 3-140+M3-Test4 (e.g. 10049) with D1 and D2 fits
    g4bl = {"run_number":run_number,"q_1":0.515,"q_2":-0.642,"q_3":0.445,"d_1":-0.617,"d_2":-0.303,\
            "d_s":1.873,"particles_per_spill":0,"rotation_angle":0,"translation_z":1000.0,\
            "protonabsorberin":1,"proton_absorber_thickness":29,"proton_number":1E12,"proton_weight":1,\
            "particle_charge":'all',"file_path":'G4bl_path_UNUSED',\
            "get_magnet_currents_pa_cdb":"False","random_seed":seed,"WKRDIR":"~/"}

    # Settings for 3-140+M3-Test4 D1+4%
#    g4bl = {"run_number":run_number,"q_1":0.514,"q_2":-0.642,"q_3":0.445,"d_1":-0.630,"d_2":-0.304,\
#            "d_s":1.878,"particles_per_spill":0,"rotation_angle":0,"translation_z":1000.0,\
#            "protonabsorberin":1,"proton_absorber_thickness":29,"proton_number":1E12,"proton_weight":1,\
#            "particle_charge":'all',"file_path":'G4bl_path_UNUSED',\
#            "get_magnet_currents_pa_cdb":"False","random_seed":seed,"WKRDIR":"~/"}


    # Settings for 7469
#    g4bl = {"run_number":run_number,"q_1":1.018,"q_2":-1.272,"q_3":0.884,"d_1":-1.242,"d_2":-0.386,\
#            "d_s":3.664,"particles_per_spill":0,"rotation_angle":0,"translation_z":1000.0,\
#            "protonabsorberin":1,"proton_absorber_thickness":82,"proton_number":1E12,"proton_weight":1,\
#            "particle_charge":'all',"file_path":'G4bl_path_UNUSED',\
#            "get_magnet_currents_pa_cdb":"False","random_seed":seed,"WKRDIR":"~/"}


    my_dict = {
    #   "simulation_geometry_filename":"/home/mice/new_g4bl_prod/geo07469/ParentGeometryFile.dat",
    #   "simulation_geometry_filename":"/home/phswbb/MC/geo-08709/ParentGeometryFile.dat",  # 3-140
        "simulation_geometry_filename":"../../geo-10049/ParentGeometryFile.dat",  # 3-140+M3-Test4
        "output_root_file_name":setting_name+"_"+str(d2current)+".root",
            "spill_generator_number_of_spills":1,
            "g4bl":g4bl}
         
    
    return my_dict

def setup_from_dict(key_dict):
    my_keys = ""
    for key, value in key_dict.iteritems():
        my_keys += str(key)+" = "+json.dumps(value)+"\n"
    return io.StringIO(unicode(my_keys))

def run():
    """ Run the macro
    """
    
    run_args = ["python", "../../run_g4bl.py", "--configuration_file", "datacard.json"]
    proc = subprocess.Popen(run_args)
    proc.wait()

if __name__ == '__main__':
    
    # Get the name from the command line
    setting_name = ""
    d2current = 0.

    (opts, args) = getopt.getopt(sys.argv[1:], "", ["setting_name=", "d2current="])
    for o, a in opts:
            if o == "--setting_name":
                if a:
                        setting_name = "%s" % a
            if o == "--d2current":
                if a:
                        d2current = float(a)
                       
                    
    if not setting_name or not d2current:
        print "Some of the parameter(s) is/are missing"
        
    # Get the simulation seed and run_numbeer from the current time
    seed = int(time.time())
    run_number = seed
    
    # Fetch the data cards and load them
    setup_dict = get_setup_dict(setting_name, d2current, seed, run_number)
    datacards = setup_from_dict(setup_dict)
    with open('datacard.json', 'w') as fd:
        datacards.seek (0)
        shutil.copyfileobj (datacards, fd)
        
    # Run the simulation (e12 protons on target)
    run()
