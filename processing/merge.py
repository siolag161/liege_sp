#! /usr/bin/env python
from os import listdir
from os.path import isfile, join
import sys
import csv

clustering_path = sys.argv[1]
merge_path = sys.argv[2]
offset_path = sys.argv[3]
snp_path = sys.argv[4]

clustering_files = [ f for f in listdir(clustering_path) if isfile(join(clustering_path,f)) ]


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

########################################################
NCHRS = 23

offset_children = [0]*(NCHRS+2)
offset_latent = [0]*(NCHRS+2)
max_ids = [0]*(NCHRS+2)

with open( snp_path, 'r') as f:
    tmp = [0]*(NCHRS+2)
    fr =  csv.reader( f, delimiter=',')
    for row in fr:
	chr = int(row[0])
	tmp[chr] = int(row[1])
	
    for chr in range(NCHRS + 1):
	tmp[chr+1] += tmp[chr]
	offset_children[chr+1] = tmp[chr]

offset_latent[1] = offset_children[NCHRS+1]
nbr_clusters = [0]*(NCHRS+2)

nbrObs = 0
nbrLatent = 0

for fn in clustering_files:
    chr = chr_from_name(fn)
    chr_int = chr2int(chr)
    max_id = -1
    latent_ids = set([])
    ##obs_ids = set([])
    with open( join(clustering_path,fn), 'r') as f:
	reader = csv.reader( f, delimiter=',')
	reader.next()
	for row in reader:
	    id = int(row[0])
	    level = int(row[3])
	    parent_id = int(row[2])
	    if ( level == 0 ):
		if (parent_id > 0):
		    latent_ids.add(parent_id)		
		if ( id > max_id ):
		    max_id = id
	nbr_clusters[chr_int] = len(latent_ids)
	max_ids[chr_int] = max_id	
	#print chr,max_id
#print nbr_clusters

for i in range(1,NCHRS+1):
    offset_latent[i+1] = offset_latent[i] + nbr_clusters[i]
    offset_latent[i] -= max_ids[i]
    print i, max_ids[i], offset_latent[i]
    
#with open()
with open(merge_path, 'w') as ffp:
    headers = [ 'chr','id', 'latent', 'parent', 'level', 'cardinality']
    fw = csv.writer( ffp, delimiter=',', quoting=csv.QUOTE_NONE )
    fw.writerow(headers)
    for fn in clustering_files:
	chr = chr_from_name(fn)
	chr_int = chr2int(chr)
	latent_ids = set([])
	with open( join(clustering_path,fn), 'r') as f:
	    ###print fn
	    reader = csv.reader( f, delimiter=',')
	    reader.next()
	    for row in reader:
		nrow = headers
		nrow[1:] = row
		level = int(row[3])
		current_id = int(row[0])
		parent_id = int(row[2])
		nrow[0] = chr
		if ( level == 0 ):
		    nrow[1] = current_id + offset_children[chr_int]
		    if (parent_id > 0):
			#nrow[3] = parent_id + offset_latent[chr_int]- max_ids[chr_int]
			nrow[3] = parent_id + offset_latent[chr_int] 
		else:
		    #nrow[1] = current_id + offset_latent[chr_int]- max_ids[chr_int]
		    nrow[1] = current_id + offset_latent[chr_int]
		    #latent_ids.add(parent_id)
		#else:
		    #neg_count += 1
		fw.writerow(nrow)
	    #offset_latent[chr_int+1] = offset_latent[chr_int] + len(latent_ids)

#print offset_latents
#for i in range(NCHRS+1):
    #print i,offset_latent[i]
with open(offset_path, 'w') as ffp:
    headers = [ 'chr','obs', 'latent']
    fw = csv.writer( ffp, delimiter=',', quoting=csv.QUOTE_NONE )
    fw.writerow(headers)
    for i in range( 1, NCHRS +1 ):
	fw.writerow([ i, offset_children[i], offset_latent[i] ])
