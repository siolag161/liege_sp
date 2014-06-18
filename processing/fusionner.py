#! /usr/bin/env python
from os import listdir
from os.path import isfile, join
import sys

path = sys.argv[1]
out_path = sys.argv[2]

files = [ f for f in listdir(path) if isfile(join(path,f)) ]

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

NBR_CHRS = 23
snp_counts = [0]*(NBR_CHRS+1)
tmp = [0]*(NBR_CHRS+2)
for fn in files:
    chr = chr_from_name(fn)
    chr_int = chr2int(chr)
    max_id = -1
    with open( join(path,fn), 'r') as f:
	reader = csv.reader( f, delimiter=',')
	reader.next()
	for row in reader:
	    current_id = int(row[0])
	    if (current_id > max_id):
		max_id = current_id	
    tmp[chr_int] = max_id + 1

for i in range( NBR_CHRS + 1):
    tmp[i+1] += tmp[i]
    for i in range( NBR_CHRS ):
	snp_counts[i+1] = tmp[i]
	

with open("snp_base_id.txt", 'w') as f:
    fw = csv.writer( f, delimiter=',', quoting=csv.QUOTE_NONE )
    for i in range( NBR_CHRS + 1):
	fw.writerow( [i, snp_counts[i]]  )

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
		chr_int = chr2int(chr)
		BASE = snp_counts[chr_int]
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


    
