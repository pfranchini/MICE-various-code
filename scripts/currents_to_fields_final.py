############################################################################
# 
#    Produces fields from the currents (fit Dipole1 and fit Dipole2)
#
#    Syntax: python currents_to_fields.py <run number | beam line tag>
#
############################################################################

import json
import cdb
import xboa.common
import sys, getopt

DS = ["DS"]
Q123 = ["Q1", "Q2", "Q3"]
Q4TO9 = ["Q4", "Q5", "Q6", "Q7", "Q8", "Q9",]
D12 = ["D1", "D2"]
D1 = ["D1"]
D2 = ["D2"]
ALL_MAGS = sorted(Q123 + Q4TO9 + D1 + D2 + DS)

def d12_current(field):
    current = 253.91*field+39.59*field**3-55.998*field**2
    return current

def d1_field(current):
    
    field = 0.00966745 + 0.00382304*current + 3.41212e-6*current**2 - 8.34544e-9*current**3
    return field

def d2_field(current):
    field = -0.00503767 + 0.00436283*current - 1.30544e-6*current**2 + 2.46632e-9*current**3
    return field

def q123_current(field_gradient):
    return 96.00*field_gradient

def q123_field_gradient(field_gradient):
    return field_gradient/96.00

def q4to9_current(field_gradient):
    return 174.09*field_gradient

def q4to9_field_gradient(current):
    return current/174.09

def ds_field(field):
    return field*5./870.

def ds_current(current):
    return 870*current/5.

def currents_to_fields(magnets):
    if sorted(magnets.keys()) != ALL_MAGS:
        raise KeyError("Missing keys")
    currents = {}
    for key in magnets.keys():
        if key in Q4TO9:
            currents[key] = q4to9_field_gradient(magnets[key])
        elif key in Q123:
            currents[key] = q123_field_gradient(magnets[key])
        elif key in D1:
            currents[key] = -d1_field(magnets[key])
        elif key in D2:
            currents[key] = -d2_field(magnets[key])
        elif key in DS:
            currents[key] = ds_field(magnets[key])
    return currents


def fields_to_currents(magnets):
    if sorted(magnets.keys()) != ALL_MAGS:
        raise KeyError("Missing keys")
    currents = {}
    for key in magnets.keys():
        if key in Q4TO9:
            currents[key] = q4to9_current(magnets[key])
        elif key in Q123:
            currents[key] = q123_current(magnets[key])
        elif key in D12:
            currents[key] = d12_current(magnets[key])
        elif key in DS:
            currents[key] = ds_current(magnets[key])
    return currents


def get_fields_for_beamline(run_number):
    print "Getting fields for beamline run", run_number
    bl = cdb.Beamline()
    beamline_all = bl.get_beamline_for_run(run_number)[run_number]
    get_beamline(beamline_all, str(run_number))

def get_fields_for_tag(tag):
    print "Getting fields for beamline tag", tag
    bl = cdb.Beamline()
    beamline_all = bl.get_beamline_for_tag(tag)[tag]
    beamline_all = massage_tag(beamline_all, polarity = 1)
    get_beamline(beamline_all, str(tag))

def get_fields_hardcoded():
    beamline_all = {
        'beam_stop': 'Open',
        'diffuser_thickness': 0,
       'proton_absorber_thickness': 82,
        'magnets': {
            'Q1': 102.4, 
            'Q2': 127.9, 
            'Q3': 89.0, 
#
            'D1': 323.2,
            'DS': 668.6, 
            'D2': 94.2, 
#
            'Q4': 158.1, 
            'Q5': 212.0, 
            'Q6': 140.6, 
#
            'Q7': 138.7, 
            'Q8': 209.8, 
            'Q9': 179.2, 
        },
    }
    beamline_all = massage_tag(beamline_all, polarity = 1)
    get_beamline(beamline_all, 'user defined beamline')

def massage_tag(beamline_all, polarity):
    for magnet in beamline_all['magnets']:
        beamline_all['magnets'][magnet] = {
            'set_current':beamline_all['magnets'][magnet],
            'polarity':polarity
        }
    return beamline_all

def get_beamline(beamline_all, name):
    beamline = beamline_all['magnets']
    print beamline.keys()
    currents = {}
    def_polarity = {"Q2":-1, "Q5":-1, "Q8":-1, "D1":1, "D2":1, "DS":1}
    for magnet in ALL_MAGS:
        print beamline[magnet]["polarity"]
        currents[magnet] = beamline[magnet]["set_current"]
        if magnet in def_polarity.keys():
            currents[magnet] *= beamline[magnet]["polarity"]*def_polarity[magnet]
        if magnet == "DS" and currents[magnet] == None:
            currents[magnet] = 0.
    print "currents:"
    for key in sorted(currents.keys()):
        print "   ", key+":", currents[key]
    magnets = currents_to_fields(currents)
    print "fields:"

    print "{"
    for key in sorted(magnets.keys()):
        print ('     "'+ key + '": ' + str(round(magnets[key],3)) + ",")
    thickness = beamline_all["proton_absorber_thickness"]
    print '     "proton_absorber": '+str(thickness)
    print "}"

    # File:
    f1=open("./json/"+name+".json", 'w+')
    print >> f1, "{" 
    for key in sorted(magnets.keys()):
        print >>f1, '     "'+ key + '": ' + str(round(magnets[key],3)) + ","
    print >> f1, '     "proton_absorber": '+str(thickness)
    print >> f1, "}"

    f1.close()

    currents_2 = fields_to_currents(magnets)
    #for key in sorted(currents_2.keys()):
        #if currents[key] - currents_2[key] > 1e-6:
            #raise ValueError("Urk")
    print "MAUS keys:"
    print '    # cdb derived currents for', name
    key_sort = {"Q1":"q_1", "Q2":"q_2", "Q3":"q_3", "D1":"d_1", "D2":"d_2", "DS":"d_s", "Q4":"q_4", "Q5":"q_5", "Q6":"q_6", "Q7":"q_7", "Q8":"q_8", "Q9":"q_9"}
    for key in sorted(key_sort):
        if key in key_sort:
            maus_key = key_sort[key]
            print 'param -unset '+maus_key+'='+str(round(magnets[key],3))

    print "Got fields for beamline", name
    


def main(arg):
    
    if arg.isdigit():
        get_fields_for_beamline(int(arg))
    else:
        get_fields_for_tag(arg)

    #get_fields_hardcoded()

if __name__ == "__main__":
    main(sys.argv[1])

