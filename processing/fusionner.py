#! /usr/bin/env python
from os import listdir
from os.path import isfile, join
import sys

path = sys.argv[1]
out_path = sys.argv[2]

files = [ f for f in listdir(path) if isfile(join(path,f)) ]

BASE_ID = 1e6

def base_id_from_name(fn):
    pos = fn.find('ch') 
    next_pos = fn.find('_', pos)
    chr = int(fn[pos+2:next_pos])
    return BASE_ID * chr

#################################################################
#print (join(path, "alibab.csv"))
import csv
with open(out_path, 'w') as ffp:
    headers = ['id', 'latent', 'parent', 'level', 'cardinality']
    fw = csv.writer( ffp, delimiter=',', quoting=csv.QUOTE_NONE )
    fw.writerow(headers)
    for fn in files:
	base_id = base_id_from_name(fn)	
	with open( join(path,fn), 'r') as f:
	    reader = csv.reader( f, delimiter=',')
	    reader.next()
	    #print headers
	    for row in reader:
		row[0] = base_id + int(row[0])
		row[2] = base_id + int(row[2])
		fw.writerow( row )


    
