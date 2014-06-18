#!/bin/bash    

IN_PATH=
OUT_PATH=
CAST=
SIMI=
MAX_DIS=
EXE=
usage()
{
cat << EOF
======================================================================================================================
usage: $0 options

OPTIONS:
   -h      Show help message
   -e      CAST executable
   -i      input path
   -o 	   output path
   -c      cast
   -s      similarity threshold
   -m      max dist
======================================================================================================================
EOF
}

while getopts "he:i:o:c:s:m:" OPTION
do
    case $OPTION in
        h)
            usage
            exit 1
            ;;
        i)
            IN_PATH=$OPTARG
            ;;
        e)
            EXE=$OPTARG
            ;;
        o)
            OUT_PATH=$OPTARG
            ;;
        c)
            CAST=$OPTARG
            ;;
        m)
            MAX_DIST=$OPTARG
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

if  [[ -z $IN_PATH ]] || [[ -z $EXE ]] || [[ -z $OUT_PATH ]] || [[ -z $CAST ]] || [[ -z $MAX_DIST ]] || [[ -z $SIMI ]]
then
    echo "error: required parameters missings..."
    echo
    usage
    exit 1
fi

DATE=$(date +"%Y_%m_%d")
PARAMS="${CAST}_${MAX_DIST}_${SIMI}"
for D in `find ${IN_PATH} -type d`
do
    CHR_DIR=$(basename $D)
    echo "${EXE}/clust_cast -i ${D}/casecontrol_filtered_data.csv -l ${D}/casecontrol_filtered_label.csv -o ${OUT_PATH}/CAST/${CHR_DIR}_${PARAMS}.txt -c ${CAST} -s ${SIMI} -m ${MAX_DIST} -v 1 "
    eval "${EXE}/clust_cast -i ${D}/casecontrol_filtered_data.csv -l ${D}/casecontrol_filtered_label.csv -o ${OUT_PATH}/CAST/${DATE}/${CHR_DIR}_${PARAMS}.txt -c ${CAST} -s ${SIMI} -m ${MAX_DIST} -v 1 "
    #eval  "./bed2clust.sh -i ${D}/casecontrol_filtered -o ${OUT_PATH} -g ${GENO_TYPE} -p ${PLINK}"
done

echo "./merge.py  ${OUT_PATH}/CAST/${DATE}/ ${OUT_PATH}/CAST/${DATE}/CAST_clustering_${PARAMS}.txt ${OUT_PATH}/CAST/${DATE}/CAST_clustering_offset_${PARAMS}.txt cd_snp_counts.txt"
eval "./merge.py  ${OUT_PATH}/CAST/${DATE}/ ${OUT_PATH}/CAST/${DATE}/CAST_clustering_${PARAMS}.txt ${OUT_PATH}/CAST/${DATE}/CAST_clustering_offset_${PARAMS}.txt cd_snp_counts.txt"

