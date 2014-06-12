#!/bin/bash    

BED_PATH=
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
   -i      .bed file path
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
            BED_PATH=$OPTARG
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

if  [[ -z $BED_PATH ]] || [[ -z $GENO_TYPE ]] || [[ -z $OUT_PATH ]] || [[ -z $PLINK ]] 
then
    echo "error: required parameters missings..."
    echo
    usage
    exit 1
fi

bed_path=$BED_PATH
bed_path_without_extension="${bed_path%.*}"

bed_path_dir="$(dirname ${bed_path})"
bed_path_dir_name="$(basename ${bed_path_dir})"

bed_filename="${bed_path##*/}"
bed_filename_without_extension="${bed_filename%.*}"

if [ ! -f "${bed_path_without_extension}.bed" ]; then
  echo ".bed file not found" 
  echo
  usage
  exit
fi
 

out_path=${OUT_PATH}/${bed_path_dir_name}
mkdir -p $out_path
echo "creating output directory ${out_path}"
echo

# change to the full plink path
echo "===== converting from .bed to .ped, delim = '\tab' ====="
echo
ped_path=${bed_path_without_extension}
out_path="${out_path}/${bed_filename_without_extension}"

command=
if  [[ $GENO_TYPE == "0" ]]; then
    echo "+ Option to output recessive turned on ( BUT NOT YET SUPPORTED )!!! "
    echo 
    usage
    exit
elif [[ $GENO_TYPE == "1" ]]; then
    echo "+ Option to ouput dominant turned on ( BUT NOT YET SUPPORTED )!!! "
    echo
    usage
    exit
else
    echo "+ Option to ouput additive turned on"	
    command="${PLINK}/plink --noweb --silent --bfile  ${ped_path} --recodeA --out ${out_path}"
fi
echo
echo "===== executing PLINK: ${command} ====="
echo
echo "...please be patient..."
eval $command
echo
echo "done executing PLINK (for converting to ADDITIVE)."
echo
echo "===== Now filtering bad observations and tranpose for obtaining the correct format ====="
echo

rm -f 'R_temp.R' # Rcript requires a temporary .R script file
R_command="bim <- read.table(\"${ped_path}.bim\", as.is = T, header = F); chromosome = as.numeric(tail(names(sort(table(bim[,1]))), 1)); idx = which(bim[,1] == chromosome) + 6; raw_ped <- read.table(\"${out_path}.raw\", as.is = TRUE, header = T); raw_ped = raw_ped[, idx]; idx = idx - 6; bim = bim[idx, c(1,2,4)]; bim = cbind( idx, bim ); label = bim[, c(2,1,3,4)]; colnames(label) = c(\"chr\", \"idx\", \"label\", \"position\"); write.table( t(raw_ped), file=\"${out_path}_data.csv\", row.names=F, col.names=F, sep=\",\"); write.table( label, file=\"${out_path}_label.csv\", row.names=F, col.names=F, sep=\",\");";
echo $R_command > R_temp.R;
Rscript R_temp.R;
rm -f 'R_temp.R' # Rcript requires a temporary .R script file
echo "...please be patient..."
echo
echo "done processing. cleaning up now..."
echo

eval "rm -f ${out_path}.raw"

echo "done and done. thanks!"
echo
echo
