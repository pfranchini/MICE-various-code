#!/bin/env python
#
#


import MAUS

import os
import sys
import copy
import shutil
import datetime

import re
import json
import argparse

import cdb


QUIET = False
DEFAULT_DATA_FILE = "/vols/mice/ch308/analysis_runs_database.json"
CDB_SERVER = "http://cdb.mice.rl.ac.uk"

def run_update(namespace) :
  print "Initializing..."
  print
  # CDB Interface
  cdb_channel = cdb.CoolingChannel(CDB_SERVER)
  cdb_beamline = cdb.Beamline(CDB_SERVER)

  # Get relevant run list
  all_runs = sorted( cdb_beamline.get_run_numbers() )
  the_runs = []
  for run in all_runs :
    if int(run) > namespace.start_run :
      the_runs.append(run)


  # Some pretty output
  sys.stdout.write("Updating:   0%\b\b\b\b")
  sys.stdout.flush()
  total = len(the_runs)
  percent = int(-1)

  helical_runs = []
  straight_runs = []
  not_used = 0
  num_errors = 0

  prog = re.compile("([0-9]+)-([0-9]+)\+M.+-.*")
  prog = re.compile("([0-9]+)-([0-9]+)\+M.+-*")

  # Loop over all relevant run numbers
  for counter, run in enumerate(the_runs) :
    run = run.zfill(5)
#    print(run)
    int_run = int(run)
    try :

      run_channel = cdb_channel.get_coolingchannel_for_run(run)
      run_beamline = cdb_beamline.get_beamline_for_run(run)[int_run]
###   unused: run_absorber = cdb_channel.get_absorber_for_run(run).keys()[0][1]

      match = prog.match( run_beamline['optics'] )
      if match is None :
        print "Not found: ", run_beamline['optics'], run
        continue

      print "Found: ", run_beamline['optics'], run

      beamline = match.group(0)
      emittance = match.group(1)
      momentum = match.group(2)

      new_percent = int((float(counter) / float(total)) * 100.0)
      if new_percent != percent :
        sys.stdout.write("{0:>3}".format(new_percent)+"\b\b\b")
        sys.stdout.flush()
        percent = new_percent

      if type(run_channel) == list :
        continue

      magnets = run_channel['magnets']
      upstream_on = False
      downstream_on = False

      for magnet in magnets :
        if magnet['name'] == "SSU" :
          for coil in magnet['coils'] :
            if coil['name'] == "SSU-C" :
              if float(coil['iset']) > 10.0 :
                upstream_on = True

        if magnet['name'] == "SSD" :
          for coil in magnet['coils'] :
            if coil['name'] == "SSD-C" :
              if float(coil['iset']) > 10.0 :
                downstream_on = True

      if upstream_on and downstream_on :
        helical_runs.append((run_beamline['run_number'], beamline, emittance, momentum, run_beamline['start_time'], run_beamline['end_time']))
      elif (not upstream_on) and (not downstream_on) :
        straight_runs.append((run_beamline['run_number'], beamline, emittance, momentum, run_beamline['start_time'], run_beamline['end_time']))
      else :
        not_used += 1


    except cdb._exceptions.CdbPermanentError as ex :
      print "CDB Exception in Run:", run
      num_errors += 1
    except cdb._exceptions.CdbTemporaryError as ex :
      print
      print "An Temporary Error Occured. Please try Again Later"
      print
      return

  print
  print "\rComplete. Found:"
  print
  print "Helical Runs             : ", len(helical_runs)
  print "Straight Runs            : ", len(straight_runs)
  print "Non-Analysis Runs        : ", not_used
  print
  print "CDB Errors : ", num_errors
  print

  with open("helical_run_numbers.dat", "w") as outfile :
    for run_number, beamline, emittance, momentum, start, end in helical_runs :
      if end is None :
        end = datetime.datetime.min
      outfile.write("{0:s} {1:s} {2:s} {3:s} {4:s} {5:s}\n".format(str(run_number), beamline, emittance, momentum, start.isoformat(" "), end.isoformat(" ")))

  with open("straight_run_numbers.dat", "w") as outfile :
    for run_number, beamline, emittance, momentum, start, end in straight_runs :
      if end is None :
        end = datetime.datetime.min
      outfile.write("{0:s} {1:s} {2:s} {3:s} {4:s} {5:s}\n".format(str(run_number), beamline, emittance, momentum, start.isoformat(" "), end.isoformat(" ")))


if __name__ == "__main__" :
  parser = argparse.ArgumentParser(description="Updates the run list against the defined Analysis Codes using the CDB")

  parser.add_argument('--start_run', type=int, default=7400, help="Specify the starting point to update from.")

  namespace = parser.parse_args()

  run_update(namespace)

