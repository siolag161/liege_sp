## CAST algorithms
### Original Paper

The link to the original paper can be found [here](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.34.5341)


### Compilation

In order to compile the code in a Linux environment, one can proceed as follows:


* Modify the CMake configuration file, especially modify these two attributes Boost_INCLUDE_DIR and Boost_LIB_DIR

  ⋅⋅* set(Boost_INCLUDE_DIR `PATH_TO_BOOST_INCLUDE_DIRECTORY` )
  ⋅⋅* set(Boost_LIBRARY_DIR `PATH_TO_BOOST_DIRECTORY_DIRECTORY` )

* Create and go to the build directory

  ** mkdir build | cd build

* Create and go to the build directory

  ⋅⋅* cmake .. 
  ⋅⋅* make

* Execute to get instruction
  ⋅⋅* ../bin/clust_cast -h
  ⋅⋅* which gives ./clust_cast [-h] -i ARG -l ARG -o ARG -c ARG -s ARG -m ARG 
   	-h [ --help ]       	Print help messages
	  -i [ --dinput ]     	Data Input filename
 	  -l [ --lpinput ]    	Label-Pos Input filename
	  -o [ --outputDir ]  	Output Directory
	  -c [ --cast ]       	CAST
	  -s [ --simi ]       	Similarity
  	-m [ --maxDist ]    	Max Distance

### Usage

As given by the command above, the program "clust_cast" takes  6 parameters:

* -i: input data file, as produced by the processing script which has the format: ( rows denote SNPS wheareas columns 
      represent observations )
* -l: input label file, as produced by the processing script which has the format. a .csv file of 4 columns
* -o: output directory. 
   ⋅⋅* for instance: -o out will create a ./out directory and put the results into this directory
   ⋅⋅* organized by the time at which the command is performed
   
* -c: the threshold for CAST. For example: 0.01 could be employed
* -s: similarity transformation flag
   ⋅⋅* if this value if negative ( -1 for example ) then no threshold will be employed. the similarity matrix will be used "as-is"
   ⋅⋅* if positive, then a transformation is applied: values above median become 1 and 0 otherwise
* -m: maximum genomic distance to be considered as possible neighbors ( i.e in the same cluster ). SNPs too far away will never
      be group together.
      
      
      
      
    
