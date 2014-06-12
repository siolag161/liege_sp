liege_sp
========

### Introduction
In this workspace found several sub-directories which contain GWAS data processing scriptrs and implementations 
for clustering algorithms.

### Clustering Algorithms

+ CAST algorithms
+# DBSCAN


### Processing 

+ .bed file to Clustering INPUT

### Working pileline

1. For each GWAS dataset( `.bed` ), split to obtain 23 separate files, one for each chromosome
2. Use the script to convert a `.bed` to a `.csv` file compatible with the clustering algorithms
3. Perform the cluster analysis on the generated output
