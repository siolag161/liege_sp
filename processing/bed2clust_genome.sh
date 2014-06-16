#!/bin/bash    

GENOME_DIR=
OUT_PATH=
GENO_TYPE=
FILTER=
PLINK=
usage()
{
cat << EOF
======================================================================================================================
usage: $0 options
This script allows convert .bed file to .csv file compatible with FLTM and other clustering algorithms ( i.e rows as rows as observations while columns denote snps ).

OPTIONS:
   -h      Show help message
   -p      plink path
   -i      .bed genome_path
   -o 	   .csv output path
   -g      Genotype output codage: recessive (0) / dominant (1) / additive (2)
======================================================================================================================
EOF
}

while getopts "hi:o:p:g:f:" OPTION
do
    case $OPTION in
        h)
            usage
            exit 1
            ;;
        i)
            GENOME_DIR=$OPTARG
            ;;
        o)
            OUT_PATH=$OPTARG
            ;;
        g)
            GENO_TYPE=$OPTARG
            ;;
        p)
            PLINK=$OPTARG
            ;;
        ?)
            usage
            exit
            ;;
    esac
done

if  [[ -z $GENOME_DIR ]] || [[ -z $GENO_TYPE ]] || [[ -z $OUT_PATH ]] || [[ -z $PLINK ]] 
then
    echo "error: required parameters missings..."
    echo
    usage
    exit 1
fi

for D in `find ${GENOME_DIR} -type d`
do
    echo "${D}/casecontrol_filtered"
    #echo "$(basename ${D})"
    CHR_DIR=$(basename $D)
    eval  "./bed2clust.sh -i ${D}/casecontrol_filtered -o ${OUT_PATH} -g ${GENO_TYPE} -p ${PLINK}"
done

