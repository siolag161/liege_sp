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

DATE=$(date +"%Y_%m_%d")
PARAMS="${MIN_PTS}_${EPS}_${MAX_POS}"
for D in `find ${IN_PATH} -type d`
do_
    echo $D
    CHR_DIR=$(basename $D)
    echo "${EXE}/clust_dbscan -i ${D}/casecontrol_filtered_data.csv -l ${D}/casecontrol_filtered_label.csv -o ${OUT_PATH}/DBSCAN/${DATE}/${CHR_DIR}_${PARAMS}.txt -e ${EPS} -s ${SIMI} -m ${MIN_PTS} -x ${MAX_POS} -v 1"
    eval "${EXE}/clust_dbscan -i ${D}/casecontrol_filtered_data.csv -l ${D}/casecontrol_filtered_label.csv -o ${OUT_PATH}/DBSCAN/${DATE}/${CHR_DIR}_${PARAMS}.txt -e ${EPS} -s ${SIMI} -m ${MIN_PTS} -x ${MAX_POS} -v 1"
don
echo "./fusionner.py  ${OUT_PATH}/CAST/${DATE}/ ${OUT_PATH}/DBSCAN/${DATE}/clustering_${PARAMS}.txt"
eval "./fusionner.py  ${OUT_PATH}/CAST/${DATE}/ ${OUT_PATH}/DBSCAN/${DATE}/clustering_${PARAMS}.txt"
