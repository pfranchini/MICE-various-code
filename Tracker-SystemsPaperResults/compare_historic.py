
import MAUS
import ROOT

import analysis
from analysis import tools

import os
import sys

import array
import json
import argparse

ROOT.gStyle.SetTimeOffset(0)



def convert_date(date) :
    root_date = ROOT.TDatime()
    root_date.Set(date)
    return root_date.Convert()


def build_metadata(namespace) :
  meta = {}
  helical_meta = {}
  with open(namespace.helical, 'r') as infile :
    for line in infile :
      data = line.split(' ')
      run = data[0]
      beamline = data[1]
      emittance = data[2]
      momentum = data[3]
      start = data[4] + " " + data[5]
      end = data[6] + " " + data[7]

      helical_meta[run] = { "beamline" : beamline, "emittance" : emittance, "momentum" : momentum, "start_time" : start, "end_time" : end }

  straight_meta = {}
  with open(namespace.straight, 'r') as infile :
    for line in infile :
      data = line.split(' ')
      run = data[0]
      beamline = data[1]
      emittance = data[2]
      momentum = data[3]
      start = data[4] + " " + data[5]
      end = data[6] + " " + data[7]

      straight_meta[run] = { "beamline" : beamline, "emittance" : emittance, "momentum" : momentum, "start_time" : start, "end_time" : end }

  meta = { 'helical' : helical_meta, 'straight' : straight_meta }

  return meta



def analyse_helical(namespace, meta_data) :
  dates = array.array('d')
  up_efficiencies = array.array('d')
  down_efficiencies = array.array('d')

  print "RUN:  UPSTREAM    -     DOWNSTREAM"

  for run, meta in meta_data.iteritems() :
    data_file = os.path.join(namespace.data, run+'.json')

    if not os.path.exists(data_file) :
      continue
    #print(data_file)

    dates.append(convert_date(meta['start_time']))

    with open(data_file, 'r') as infile :
      json_data = json.load(infile)
      
      up_efficiencies.append(json_data['upstream']['efficiencies']['helical_efficiency'])
      down_efficiencies.append(json_data['downstream']['efficiencies']['helical_efficiency'])

      print run+" "+str(json_data['upstream']['efficiencies']['helical_efficiency'])+" "+str(json_data['downstream']['efficiencies']['helical_efficiency'])

  return dates, up_efficiencies, down_efficiencies



def analyse_straight(namespace, meta_data) :
  dates = array.array('d')
  up_efficiencies = array.array('d')
  down_efficiencies = array.array('d')

  print "RUN:  UPSTREAM    -     DOWNSTREAM"

  for run, meta in meta_data.iteritems() :
    data_file = os.path.join(namespace.data, run+'.json')

    if not os.path.exists(data_file) :
      continue
    #print(data_file)

    dates.append(convert_date(meta['start_time']))

    with open(data_file, 'r') as infile :
      json_data = json.load(infile)
      
      up_efficiencies.append(json_data['upstream']['efficiencies']['straight_efficiency'])
      down_efficiencies.append(json_data['downstream']['efficiencies']['straight_efficiency'])

      print run+" "+str(json_data['upstream']['efficiencies']['straight_efficiency'])+" "+str(json_data['downstream']['efficiencies']['straight_efficiency'])

  return dates, up_efficiencies, down_efficiencies



def make_plots(data) :
  dates, up_efficiencies, down_efficiencies = data
  if len(dates) < 2 : return {}
  plots = {}

  plots['up_efficiency'] = ROOT.TGraph(len(dates), dates, up_efficiencies)
  plots['down_efficiency'] = ROOT.TGraph(len(dates), dates, down_efficiencies)

  return plots


if __name__ == "__main__" :
  parser = argparse.ArgumentParser()

  parser.add_argument('--helical', required=True)
  parser.add_argument('--straight', required=True)
  parser.add_argument('-O', '--output_file', default='historical_analysis')
  parser.add_argument('-D', '--output_directory', default='./')
  parser.add_argument('data')

  namespace = parser.parse_args()

  meta_data = build_metadata(namespace)

  print("Analyse helical...")
  print("==================")
  helical_analysis = analyse_helical(namespace, meta_data['helical'])

  print("Analyse straight...")
  print("==================")
  straight_analysis = analyse_straight(namespace, meta_data['straight'])

  helical_plots = make_plots(helical_analysis)

  straight_plots = make_plots(straight_analysis)

  plots = { 'helical' : helical_plots, 'straight' : straight_plots }

  filename = os.path.join(namespace.output_directory, namespace.output_file)
  analysis.tools.save_plots(plots, filename+".root")

