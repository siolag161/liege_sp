#!/bin/bash    

BED_PATH=
OUT_PATH=
usage()
{
cat << EOF
======================================================================================================================
usage: $0 options
This script allows convert .bed file to .csv file compatible with FLTM and other clustering algorithms ( i.e rows as rows as observations while columns denote snps ).

OPTIONS:
   -h      Show help message
   -i      .bed file path
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
            BED_PATH=$OPTARG
            ;;
        o)
            OUT_PATH=$OPTARG
            ;;
        ?)
            usage
            exit
            ;;
    esac
done

if  [[ -z $BED_PATH ]] || [[ -z $OUT_PATH ]] 
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
  echo ".bed file not found at: ${bed_path_without_extension}.bed" 
  echo
  usage
  exit
fi
 

out_path=${OUT_PATH}/${bed_path_dir_name}
mkdir -p $out_path
echo "creating output directory ${out_path}"

# change to the full plink path
echo
bed_path="${bed_path_without_extension}"
out_path="${out_path}/${bed_filename_without_extension}"

echo "${bed_path}.bim"

rm -f 'R_temp.R' # Rcript requires a temporary .R script file
R_command="bim <- read.table(\"${bed_path}.bim\", as.is = T, header = F); fam <- read.table(\"${bed_path}.fam\", as.is = T, header = F); pheno_label = cbind( bim[, c(2,1,3,4)], fam[, 6] ); write.table(pheno_label, file=\"${out_path}_pheno.csv\", row.names=F, col.names=F, sep=\",\");";
echo $R_command > R_temp.R;
Rscript R_temp.R;
rm -f 'R_temp.R' # Rcript requires a temporary .R script file
echo "...please be patient..."
echo
echo "done processing. cleaning up now..."
echo
#eval "rm -f ${out_path}.raw"
echo "done and done. thanks!"
echo
echo
