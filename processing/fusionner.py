#! /usr/bin/env python
from os import listdir
from os.path import isfile, join
import sys

path = sys.argv[1]
out_path = sys.argv[2]
count_path = sys.argv[3]

files = [ f for f in listdir(path) if isfile(join(path,f)) ]

snp_counts = {}
BASE_ID = 1e6

def chr_from_name(fn):
    pos = fn.find('ch') 
    next_pos = fn.find('_', pos)
    chr = fn[pos+2:next_pos]
    return chr

def chr2int(chr):
    if (chr != 'X'):
	chr = int(chr)
    else:
	chr = 23
    return chr

#################################################################
import csv

files = [ f for f in listdir(path) if isfile(join(path,f)) ]

with open(count_path, 'r') as f:
    reader = csv.reader( f, delimiter=',')
    for row in reader:
	snp = int(row[0])
	count = int(row[1])
	snp_counts[snp] = count

neg_count = 0
with open(out_path, 'w') as ffp:
    headers = [ 'chr','id', 'latent', 'parent', 'level', 'cardinality']
    fw = csv.writer( ffp, delimiter=',', quoting=csv.QUOTE_NONE )
    fw.writerow(headers)
    for fn in files:
	chr = chr_from_name(fn)
	with open( join(path,fn), 'r') as f:
	    print fn
	    reader = csv.reader( f, delimiter=',')
	    reader.next()
	    for row in reader:
		nrow = headers
		nrow[1:] = row
		chr_int = chr2int(fn)
		BASE = snp_counts[chr_int - 1]
		current_id = int(row[0])
		parent_id = int(row[2])
		nrow[0] = chr
		nrow[1] = current_id + BASE
		if (parent_id > 0):
		    nrow[3] = parent_id + BASE
		else:
		    neg_count += 1
		fw.writerow(nrow)
	    print neg_count
	    print


    
