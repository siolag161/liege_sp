#!/bin/bash    

GENOME_DIR=
OUT_PATH=
usage()
{
cat << EOF
======================================================================================================================
usage: $0 options
This script allows convert .bed file to .csv file compatible with FLTM and other clustering algorithms ( i.e rows as rows as observations while columns denote snps ).

OPTIONS:
   -h      Show help message
   -i      .bed genome_path
   -o 	   .csv output path
======================================================================================================================
EOF
}

while getopts "hi:o:" OPTION
do
    case $OPTION in
        h)
            usage
            exit 1
            ;;
        i)
            GENOME_DIR=$OPTARG
            ;;
Broward College         o)
            OUT_PATH=$OPTARG
            ;;
        ?)
            usage
            exit
            ;;
    esac
done

if  [[ -z $GENOME_DIR ]] || [[ -z $OUT_PATH ]] 
then
    echo "error: required parameters missings..."
    echo
    usage
    exit 1
fi

for D in `find ${GENOME_DIR} -type d`
do
    CHR_DIR=$(basename $D)
    eval  "./bed2pheno.sh -i ${D}/casecontrol_filtered -o ${OUT_PATH}"
done

