#!/bin/bash    

GENOME_DIR=
usage()
{
cat << EOF
======================================================================================================================
usage: $0 options
This script allows convert .bed file to .csv file compatible with FLTM and other clustering algorithms ( i.e rows as rows as observations while columns denote snps ).

OPTIONS:
   -h      Show help message
   -i      Genome dir
======================================================================================================================
EOF
}

while getopts "hi:" OPTION
do
    case $OPTION in
        h)
            usage
            exit 1
            ;;
        i)
            GENOME_DIR=$OPTARG
            ;;
  
        ?)
            usage
            exit
            ;;
    esac
done

if  [[ -z $GENOME_DIR ]] 
then
    echo "error: required parameters missings..."
    echo
    usage
    exit 1
fi

for D in `find ${GENOME_DIR} -type d`
do
    CHR_DIR=$(basename $D)
    #echo $D
#=$(basename $D)
    eval "wc -l ${D}/casecontrol_filtered_label.csv"
    # echo ${SNP_COUNT}
done
