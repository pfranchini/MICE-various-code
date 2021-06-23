#!/bin/env /vols/build/mice/ch308/maus-bzr/maus_v3.2.0/third_party/install/bin/python
#
# Added radial cut and five stations cut
#

import MAUS

import sys
import os
import argparse
import math
from math import sqrt

import json
import event_loader
import analysis
from analysis import tools
from analysis import covariances
from analysis import hit_types
import ROOT

NPE_CUT = 5.0

NEIGHBOUR_PT_CUT = 20.0

TOF1_CUT_LOW = 0.0
TOF1_CUT_HIGH = 40.0

TOF2_CUT_LOW = 20.0
TOF2_CUT_LOW = 40.0

TOF_PLOT1 = ROOT.TH1F('tof_plot_2', '', 400, 0.0, 40.0 )
TOF_PLOT2 = ROOT.TH1F('tof_plot_1', '', 400, 20.0, 60.0 )

SPILL = ROOT.TH1F('spill', '', 5000, 0, 5000 )

RADIUS_MAX = 150.0

def passes_cuts( tof_event ) :
  event_spacepoints = tof_event.GetTOFEventSpacePoint()

  tof0_sp_size = event_spacepoints.GetTOF0SpacePointArraySize()
  tof1_sp_size = event_spacepoints.GetTOF1SpacePointArraySize()
  tof2_sp_size = event_spacepoints.GetTOF2SpacePointArraySize()

  if tof0_sp_size < 1 :
    return False
  if tof1_sp_size != 1 or tof2_sp_size != 1 :
    return False

  tof0_sp = event_spacepoints.GetTOF0SpacePointArrayElement(0)
  tof1_sp = event_spacepoints.GetTOF1SpacePointArrayElement(0)
  tof2_sp = event_spacepoints.GetTOF2SpacePointArrayElement(0)

  diff_0_1 = tof1_sp.GetTime() - tof0_sp.GetTime()
  diff_1_2 = tof2_sp.GetTime() - tof1_sp.GetTime()

  TOF_PLOT1.Fill(diff_0_1)
  if diff_0_1 < TOF1_CUT_LOW or diff_0_1 > TOF1_CUT_HIGH :
    return False

  TOF_PLOT2.Fill(diff_1_2)
  if diff_1_2 < TOF2_CUT_LOW or diff_1_2 > TOF2_CUT_HIGH :
    return False

  return True



class Tracker_Analyser(object) :

  def __init__(self, tracker_no) :
    self.__tracker = tracker_no

    self.__event_counter = 0
    self.__good_event_counter = 0
    self.__bad_event_counter = 0
    self.__digit_counter = 0
    self.__cluster_counter = 0
    self.__spacepoint_counter = 0
    self.__spacepoint_CUT_counter = 0
    self.__used_spacepoint_counter = 0
    self.__helical_counter = 0
    self.__straight_counter = 0
    self.__helical_events = 0
    self.__straight_events = 0

    self.__expected_track_events = 0
    self.__no_expected_track_events = 0
    self.__expected_missing_track_events = 0

    self.__new_select_missed = 0
    self.__old_select_missed = 0

    self.__new_missed = 0
    self.__old_missed = 0
    self.__new_found = 0
    self.__old_found = 0

    self.__neighbour_found = 0
    self.__neighbour_missed = 0
    self.__neighbour_expected_missed = 0

    self.__plot_cluster_npe = ROOT.TH1F(str(self.__tracker)+"_cluster_npe", "", 100, 0.0, 100.0)
    self.__plot_spacepoint_npe = ROOT.TH1F(str(self.__tracker)+"_spacepoint_npe", "", 100, 0.0, 100.0)
    self.__plot_spacepoints = ROOT.TH1F(str(self.__tracker)+"_spacepoint", "", 101, -0.5, 100.5)
    self.__plot_spacepoints_CUT = ROOT.TH1F(str(self.__tracker)+"_spacepoint_CUT", "", 101, -0.5, 100.5)

    self.__plot_found_spacepoint_npe = ROOT.TH1F(str(self.__tracker)+"_found_spacepoint_npe", "", 100, 0.0, 100.0)
    self.__plot_missed_spacepoint_npe = ROOT.TH1F(str(self.__tracker)+"_missed_spacepoint_npe", "", 100, 0.0, 100.0)

    self.__plot_spacepoints_CUT_new = ROOT.TH1F(str(self.__tracker)+"_spacepoint_CUT_new", "", 101, -0.5, 100.5)
    self.__plot_spacepoints_CUT_old = ROOT.TH1F(str(self.__tracker)+"_spacepoint_CUT_old", "", 101, -0.5, 100.5)

    self.__plot_spacepoints_station_good = ROOT.TH2F(str(self.__tracker)+"_spacepoint_station_good", "", 5, 0.5, 5.5, 10, -0.5, 9.5)
    self.__plot_spacepoints_station_old = ROOT.TH2F(str(self.__tracker)+"_spacepoint_station_old", "", 5, 0.5, 5.5, 10, -0.5, 9.5)
    self.__plot_spacepoints_station_new = ROOT.TH2F(str(self.__tracker)+"_spacepoint_station_new", "", 5, 0.5, 5.5, 10, -0.5, 9.5)

    self.__plot_chisq_ndf = ROOT.TH1F(str(self.__tracker)+"_chisq_ndf", "", 500, 0.0, 100.0 )


  def analyse_event(self, scifi_event, passed) :
    digits = scifi_event.digits()
    clusters = scifi_event.clusters()
    spacepoints = scifi_event.spacepoints()

    helical_tracks = scifi_event.helicalprtracks()
    straight_tracks = scifi_event.straightprtracks()

    kalman_tracks = scifi_event.scifitracks()

    has_neighbours = False

#    if self.__tracker == 0 :
#      tracks_expected = scifi_event.get_expected_track_upstream()
#    else :
#      tracks_expected = scifi_event.get_expected_track_downstream()

    num_digits = 0
    num_clusters = 0
    num_spacepoints = 0
    num_spacepoints_CUT = 0
    num_used_spacepoints = 0
    num_helicals = 0
    num_straights = 0

    radial_cut = True
    five_stations_cut = False

    spacepoints_station = { 1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0, 5: 0.0 }


    for helical in helical_tracks :
      if helical.get_tracker() == self.__tracker :
        num_helicals += 1
      elif helical.get_reference_momentum().Pt() < NEIGHBOUR_PT_CUT :
        has_neighbours = True

    for straight in straight_tracks :
      if straight.get_tracker() == self.__tracker :
        num_straights += 1

    total_tracks = num_straights + num_helicals

    for digit in digits :
      if digit.get_tracker() == self.__tracker :
        num_digits += 1

    for cluster in clusters :
      if cluster.get_tracker() == self.__tracker :
        num_clusters += 1

    for spacepoint in spacepoints :
      if spacepoint.get_tracker() == self.__tracker :
        num_spacepoints += 1
        spacepoints_station[ spacepoint.get_station() ] += 1.0

        if spacepoint.get_npe() > NPE_CUT :
          num_spacepoints_CUT += 1

        if spacepoint.get_used() :
          num_used_spacepoints += 1

    if spacepoints_station[1]*spacepoints_station[2]*spacepoints_station[3]*spacepoints_station[4]*spacepoints_station[5] == 1:
      five_stations_cut = True
      #print "Num. spacepoints: " + str(num_spacepoints)

    if num_spacepoints_CUT > 3 :
      self.__plot_spacepoints_CUT_new.Fill( num_spacepoints_CUT )
      for key, value in spacepoints_station.iteritems() :
        self.__plot_spacepoints_station_new.Fill( key, value )
    if total_tracks > 0 :
      self.__plot_spacepoints_CUT_old.Fill( num_spacepoints_CUT )
      for key, value in spacepoints_station.iteritems() :
        self.__plot_spacepoints_station_old.Fill( key, value )


    if passed :

      for spacepoint in spacepoints :
        pos = spacepoint.get_position()
        if sqrt(pos.X()**2+pos.Y()**2) > RADIUS_MAX:
          radial_cut = False

      ### EVENT COUNTER ###
      if radial_cut and five_stations_cut:    
        self.__event_counter += 1

      self.__helical_counter += num_helicals
      self.__straight_counter += num_straights
      self.__digit_counter += num_digits
      self.__cluster_counter += num_clusters
      self.__spacepoint_counter += num_spacepoints

      if has_neighbours :
        self.__neighbour_found += 1

#      if total_tracks == 0 and tracks_expected :
#        self.__expected_missing_track_events += 1

#      if tracks_expected :
#        self.__expected_track_events += 1
#      else :
#        self.__no_expected_track_events += 1

      if total_tracks == 0 and has_neighbours :
        self.__neighbour_missed += 1

#      if total_tracks == 0 and has_neighbours and tracks_expected :
#        self.__neighbour_expected_missed += 1


      for track in kalman_tracks :
        self.__plot_chisq_ndf.Fill(track.chi2()/track.ndf())

      for key, value in spacepoints_station.iteritems() :
        self.__plot_spacepoints_station_good.Fill( key, value )

      for cluster in clusters :
        if cluster.get_tracker() == self.__tracker :
          self.__plot_cluster_npe.Fill(cluster.get_npe())

      for spacepoint in spacepoints :
        if spacepoint.get_tracker() == self.__tracker :
          self.__plot_spacepoint_npe.Fill(spacepoint.get_npe())

          if total_tracks == 1 :
            if spacepoint.is_used() :
              self.__plot_found_spacepoint_npe.Fill( spacepoint.get_npe() )
            else :
              self.__plot_missed_spacepoint_npe.Fill( spacepoint.get_npe() )

      self.__used_spacepoint_counter += num_used_spacepoints

      if num_spacepoints_CUT > 3 :
        self.__good_event_counter += 1
        if total_tracks < 1 :
          self.__old_select_missed += 1
      else :
        if total_tracks > 0 :
          self.__new_select_missed += 1

      ### HELICAL TRACKS COUNTER ###
      if num_helicals > 0 :
        if radial_cut and five_stations_cut:
          self.__helical_events += 1

      ### STRAIGTH TRACKS COUNTER ###
      if num_straights > 0 :
        if radial_cut and five_stations_cut:
          self.__straight_events += 1

      self.__plot_spacepoints.Fill(num_spacepoints)
      self.__plot_spacepoints_CUT.Fill(num_spacepoints_CUT)

    else :
      self.__bad_event_counter += 1

      if num_spacepoints_CUT > 2 :
        if total_tracks < 1 :
          self.__old_missed += 1
        else :
          self.__old_found += 1
      else :
        if total_tracks > 0 :
          self.__new_missed += 1
        else :
          self.__new_found += 1



  def conclude(self) :
    data = {}
    data['means'] = { 'digits' : float(self.__digit_counter)/float(self.__event_counter),\
                      'clusters'    : float(self.__cluster_counter)/float(self.__event_counter),\
                      'spacepoints' : float(self.__spacepoint_counter)/float(self.__event_counter),\
                      'helicals'    : float(self.__helical_counter)/float(self.__event_counter),\
                      'straights'   : float(self.__straight_counter)/float(self.__event_counter),\
                      'npe_cut_spacepoints' : float(self.__spacepoint_CUT_counter)/float(self.__event_counter),\
                      'good_npe_cut_spacepoints' : float(self.__spacepoint_CUT_counter)/float(self.__good_event_counter) }
    data['npe_cut'] = NPE_CUT
    data['totals'] = { 'digits'      : self.__digit_counter,\
                       'clusters'    : self.__cluster_counter,\
                       'spacepoints' : self.__spacepoint_counter,\
                       'used_spacepoints' : self.__used_spacepoint_counter,\
                       'helicals'    : self.__helical_counter,\
                       'straights'   : self.__straight_counter }

    data['event_counters'] = { 'analysed_events' : self.__event_counter,\
                               'neighbour_events_found' : self.__neighbour_found,\
                               'neighbour_events_missed' : self.__neighbour_missed,\
                               'good_events' : self.__good_event_counter,\
                               'good_events_missed' : self.__old_select_missed,\
                               'found_nongood_events' : self.__new_select_missed,\
                               'helical_events' : self.__helical_events,\
                               'straight_events' : self.__straight_events,\
                               'bad_events' : self.__bad_event_counter }

    if self.__neighbour_found > 0 :
      data['efficiencies'] = { 'helical_efficiency' : 100.0*(float(self.__helical_events) / float(self.__event_counter)),\
                               'straight_efficiency' : 100.0*(float(self.__straight_events) / float(self.__event_counter)),\
                               'spacepoints_efficiency' : 100.0*float(self.__used_spacepoint_counter)/float(5.0*self.__event_counter),\
                               'neighbour_efficiency' : 100.0*float(self.__neighbour_found-self.__neighbour_missed) / float(self.__neighbour_found) }
    else :
      data['efficiencies'] = { 'helical_efficiency' : 100.0*(float(self.__helical_events) / float(self.__event_counter)),\
                               'straight_efficiency' : 100.0*(float(self.__straight_events) / float(self.__event_counter)),\
                               'spacepoints_efficiency' : 100.0*float(self.__used_spacepoint_counter)/float(5.0*self.__event_counter),\
                               'neighbour_efficiency' : 0 }

    data['bad_events'] = { 'good_events' : self.__new_found,\
                           'track_events' : self.__old_found,\
                           'good_events_missed' : self.__new_missed,\
                           'track_events_missed' : self.__old_missed }

    data['flagged_events'] = { 'track_expected' : self.__expected_track_events,\
                               'not_expected_events' : self.__no_expected_track_events,\
                               'missing_track_expected' : self.__expected_missing_track_events,\
                               'neighbouring_expected' : self.__neighbour_expected_missed }

    
    return data


  def get_plots(self) :
    plot_dict = {}

    plot_dict['cluster_npe'] = self.__plot_cluster_npe
    plot_dict['spacepoint_npe'] = self.__plot_spacepoint_npe
    plot_dict['spacepoints'] = self.__plot_spacepoints
    plot_dict['spacepoints_CUT'] = self.__plot_spacepoints_CUT

    plot_dict['found_spacepoint_npe'] = self.__plot_found_spacepoint_npe
    plot_dict['missed_spacepoint_npe'] = self.__plot_missed_spacepoint_npe

    plot_dict['new_spacepoints_CUT'] = self.__plot_spacepoints_CUT_new
    plot_dict['old_spacepoints_CUT'] = self.__plot_spacepoints_CUT_old

    plot_dict['spacepoints_station_good'] = self.__plot_spacepoints_station_good
    plot_dict['spacepoints_station_new'] = self.__plot_spacepoints_station_new
    plot_dict['spacepoints_station_old'] = self.__plot_spacepoints_station_old

    return plot_dict



if __name__ == "__main__" : 
  ROOT.gROOT.SetBatch( True )
  ROOT.gErrorIgnoreLevel = ROOT.kError

  parser = argparse.ArgumentParser( description="" )

  parser.add_argument( 'maus_root_files', nargs='+', help='List of MAUS '+\
                  'output root files containing reconstructed straight tracks')

  parser.add_argument( '--tof1_cut', nargs=2, type=float, help='')

  parser.add_argument( '--tof2_cut', nargs=2, type=float, help='')

  parser.add_argument( '-D', '--output_directory', default="./", help="Specify the directory in which to save the results.")
  parser.add_argument( '-O', '--output_filename', default="efficiency_analysis", help="Specify the file name to produce.")

  try :
    namespace = parser.parse_args()

    upstream_analyser = Tracker_Analyser(0)
    downstream_analyser = Tracker_Analyser(1)

    TOF1_CUT_LOW = namespace.tof1_cut[0]
    TOF1_CUT_HIGH = namespace.tof1_cut[1]

    TOF2_CUT_LOW = namespace.tof2_cut[0]
    TOF2_CUT_HIGH = namespace.tof2_cut[1]

  except BaseException as ex:
    raise
  else :


    file_reader = event_loader.maus_reader(namespace.maus_root_files)
#    file_reader.set_max_num_events(50000)

    try :
      while file_reader.next_selected_event() :

        #print "Spill: " +  str(file_reader.get_current_spill_number())
        #print "Event: " +  str(file_reader.get_current_event_number())

        SPILL.Fill(file_reader.get_current_spill_number())

        try :
          scifi_event = file_reader.get_event( 'scifi' )
          tof_event = file_reader.get_event( 'tof' )

          if passes_cuts( tof_event ) :
            upstream_analyser.analyse_event( scifi_event, True )
            downstream_analyser.analyse_event( scifi_event, True )
          else :
            upstream_analyser.analyse_event( scifi_event, False )
            downstream_analyser.analyse_event( scifi_event, False )

        except ValueError as ex :
          print "An Error Occured: " + str(ex)
          print "Skipping Event: " +\
                str(file_reader.get_current_event_number()) + " In Spill: " + \
                str(file_reader.get_current_spill_number()) + " In File: " + \
                str(file_reader.get_current_filenumber()) + "\n"
          continue

    except KeyboardInterrupt :
      print
      print " ###  Keyboard Interrupt  ###"
      print
    print "- {0:0.0f} Spills Loaded                                 ".format( \
                                            file_reader.get_total_num_spills())


    print"\n- Analysing Data...\n"

    data_up = upstream_analyser.conclude()
    data_down = downstream_analyser.conclude()

    max_1 = TOF_PLOT1.GetMaximum()
    max_2 = TOF_PLOT2.GetMaximum()
    lower_line_1 = ROOT.TLine(TOF1_CUT_LOW, 0.0, TOF1_CUT_LOW, max_1)
    upper_line_1 = ROOT.TLine(TOF1_CUT_HIGH, 0.0, TOF1_CUT_HIGH, max_1)
    lower_line_2 = ROOT.TLine(TOF2_CUT_LOW, 0.0, TOF2_CUT_LOW, max_2)
    upper_line_2 = ROOT.TLine(TOF2_CUT_HIGH, 0.0, TOF2_CUT_HIGH, max_2)
    TOF_PLOT1.GetListOfFunctions().Add(lower_line_1)
    TOF_PLOT1.GetListOfFunctions().Add(upper_line_1)
    TOF_PLOT2.GetListOfFunctions().Add(lower_line_2)
    TOF_PLOT2.GetListOfFunctions().Add(upper_line_2)

    plots = { 'upstream' : upstream_analyser.get_plots(), 'downstream' : downstream_analyser.get_plots(), 'tof1' : TOF_PLOT1, 'tof2' : TOF_PLOT2, 'spill' : SPILL }
    data = { 'upstream' : data_up, 'downstream' : data_down }

    filename = os.path.join(namespace.output_directory, namespace.output_filename)
    analysis.tools.save_plots(plots, filename+".root")

    with open(filename+".json", "w") as outfile :
      json.dump(data, outfile)
    
