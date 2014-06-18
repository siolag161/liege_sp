#!/bin/bash    

IN_PATH=
OUT_PATH=

EXE=

K=
EPS=
SIMI=
MAX_DIS=
N=
usage()
{
cat << EOF
======================================================================================================================
usage: $0 options

OPTIONS:
   -h      Show help message
   -p      Kmedoids executable
   -i      input path
   -o 	   output path
   -k      k
   -n      n: average number of items per clusters
   -e      epsilon
   -s      similarity threshold
   -m      max dist
======================================================================================================================
EOF
}

while getopts "hp:i:o:k:n:e:s:m:" OPTION
do
    case $OPTION in
        h)
            usage
            exit 1
            ;;
        p)
            EXE=$OPTARG
            ;;
        i)
            IN_PATH=$OPTARG
            ;;
        o)
            OUT_PATH=$OPTARG
            ;;
        k)
            K=$OPTARG
            ;;
        n)
            N=$OPTARG
            ;;
        e)
            EPS=$OPTARG
            ;;
        s)
            SIMI=$OPTARG
            ;;
        m)
            MAX_DIST=$OPTARG
            ;;
        ?)
            usage
            exit
            ;;
    esac
done

if  [[ -z $IN_PATH ]] || [[ -z $EXE ]] || [[ -z $OUT_PATH ]] || [[ -z $K ]] || [[ -z $MAX_DIST ]] || [[ -z $SIMI ]]  || [[ -z $EPS ]] || [[ -z $N ]]
then
    echo "error: required parameters missings..."
    echo
    usage
    exit 1
fi


DATE=$(date +"%Y_%m_%d")
PRS=
KP=
exe=
if [ $K -lt 0 ]
    then
	PRS="AVG_${N}_${MAX_DIST}_${SIMI}"
	KP=" -n ${N} "
	exe="clust_autok"
    else
	PRS="K_${K}_${MAX_DIST}_${SIMI}"
	KP=" -k ${K} "
	exe="clust_kmedoids"
    fi

for D in `find ${IN_PATH} -type d`
do
    CHR_DIR=$(basename $D)    
    echo "${EXE}/${exe} -i ${D}/casecontrol_filtered_data.csv -l ${D}/casecontrol_filtered_label.csv -o ${OUT_PATH}/KMedoids/KMedoids_${CHR_DIR}_${PARAMS}.txt ${KP} -e ${EPS} -s ${SIMI} -x ${MAX_DIST} -v 1 "
    eval "${EXE}/${exe} -i ${D}/casecontrol_filtered_data.csv -l ${D}/casecontrol_filtered_label.csv -o ${OUT_PATH}/KMedoids/KMedoids_${CHR_DIR}_${PARAMS}.txt ${KP} -e ${EPS} -s ${SIMI} -x ${MAX_DIST} -v 1"
done

echo "./fusionner.py ${OUT_PATH}/KMedoids/${DATE}/ ${OUT_PATH}/KMedoids/${DATE}/KMedoids_clustering_${PRS}.txt"
eval "./fusionner.py ${OUT_PATH}/KMedoids/${DATE}/ ${OUT_PATH}/KMedoids/${DATE}/KMedoids_clustering_${PRS}.txt"

