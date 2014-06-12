## DBSCAN algorithms
### Original Paper

[Wikipedia link](http://en.wikipedia.org/wiki/DBSCAN)


### Compilation

In order to compile the code in a Linux environment, one can proceed as follows:


* Modify the CMake configuration file, especially modify these two attributes Boost_INCLUDE_DIR and Boost_LIB_DIR

  set(Boost_INCLUDE_DIR `PATH_TO_BOOST_INCLUDE_DIRECTORY` )
  
  set(Boost_LIBRARY_DIR `PATH_TO_BOOST_DIRECTORY_DIRECTORY` )

* Create and go to the build directory
 
  mkdir build | cd build

* Create and go to the build directory

   cmake ..
   
   make
   
* This will create 2 executables, one for test and one for the main program
  ../bin/clust_dbscan -h

  which gives: USAGE: clust_dbscan [-h] -i ARG -l ARG -o ARG -x ARG -m ARG -e ARG -s ARG 

  -h [ --help ]       	Print help messages
  
	-i [ --dinput ]     	Data Input filename
	
	-l [ --lpinput ]    	Label-Pos Input filename
	
	-o [ --outputDir ]  	Output Directory
	
	-x [ --maxPos ]     	Max Position
	
	-m [ --minPts ]     	Min Points
	
	-e [ --eps ]        	Epsilon
	
	-s [ --simi ]       	Similarity Threshold (if < 0 then no threshold)


### Usage

As given by the command above, the program "clust_dbscan" takes 7 parameters:

* -i: input data file, as produced by the processing script which has the format: ( rows denote SNPS wheareas columns 
      represent observations )
* -l: input label file, as produced by the processing script which has the format. a .csv file of 4 columns
* -o: output directory. 

   * for instance: -o out will create a ./out directory and put the results into this directory
   * organized by the time at which the command is performed
   
* -e: the reachability epsilon. should be from 0 to 1
* -m: the minimum number of points in a cluster to be considered "not noisy". 
* -s: similarity transformation flag
   * if this value if negative ( -1 for example ) then no threshold will be employed. the similarity matrix will be used "as-is"
   * if positive, then a transformation is applied: values above median become 1 and 0 otherwise
* -x: maximum genomic distance to be considered as possible neighbors ( i.e in the same cluster ). SNPs too far away will never
      be group together.
      
      
      
      
    
