#!/bin/bash    

IN_PATH=
OUT_PATH=
SIMI=
MAX_POS=
EXE=

EPS=
MIN_PTS=
usage()
{
cat << EOF
======================================================================================================================
usage: $0 options

OPTIONS:
   -h      Show help message
   -d      dbscan executable
   -i      input path
   -o 	   output path
   -e      epsilon
   -m      minPts
   -s      similarity threshold
   -x      max dist
======================================================================================================================
EOF
}

while getopts "hd:i:o:e:x:s:m:" OPTION
do
    case $OPTION in
        h)
            usage
            exit 1
            ;;
        i)
            IN_PATH=$OPTARG
            ;;
        d)
            EXE=$OPTARG
            ;;
        o)
            OUT_PATH=$OPTARG
            ;;
	e)
            EPS=$OPTARG
            ;;
	m)
            MIN_PTS=$OPTARG
            ;;
        x)
            MAX_POS=$OPTARG
            ;;
        s)
            SIMI=$OPTARG
            ;;
        ?)
            usage
            exit
            ;;
    esac
done

if  [[ -z $IN_PATH ]] || [[ -z $EXE ]] || [[ -z $OUT_PATH ]] || [[ -z $EPS ]] || [[ -z $MAX_POS ]] || [[ -z $SIMI ]] || [[ -z $MIN_PTS ]] 
then
    echo "error: required parameters missings..."
    echo
    usage
    exit 1
fi

for D in `find ${IN_PATH} -type d`
do
    echo $D
    CHR_DIR=$(basename $D)
    echo "${EXE}/clust_dbscan -i ${D}/casecontrol_filtered_data.csv -l ${D}/casecontrol_filtered_label.csv -o ${OUT_PATH}/DBSCAN/${CHR_DIR} -e ${EPS} -s ${SIMI} -m ${MIN_PTS} -x ${MAX_POS}"
    # eval "${EXE}/clust_dbscan -i ${D}/casecontrol_filtered_data.csv -l ${D}/casecontrol_filtered_label.csv -o ${OUT_PATH}/DBSCAN/${CHR_DIR} -e ${EPS} -s ${SIMI} -m ${MAX_DIST} "
    #eval  "./bed2clust.sh -i ${D}/casecontrol_filtered -o ${OUT_PATH} -g ${GENO_TYPE} -p ${PLINK}"
done

