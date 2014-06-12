##  bed2clust.sh 
### usage:

``` ./bed2clust.sh -i PATH_TO_INPUT_BED_FILE -p PATH_TO_PLINK -g CODING  -o PATH_TO_OUTPUT_DIR ```

where:


* PATH_TO_INPUT_BED_FILE: the path to the input .bed file
* PATH_TO_PLINK: path to PLINK program directory
* CODING: 
    - if 0: recessive coding 0/1; 
    - if 1: dominant coding 0/1;
    - else: additive coding 0/1/2 (the number of minor alleles )
* PATH_TO_OUTPUT_DIR: output directory, will be created if necessary
* example: ./bed2clust.sh -i in/chr02/casecontrol_filtered -o out/ -g 2 -p plink/
* takes a `in/chr02/casecontrol_filtered.bed`, a `in/chr02/casecontrol_filtered.fam` and a `in/chr02/casecontrol_filtered.bim`
  and feed to PLINK. PLINK will convert to obtain a ADDITIVE `out/chr02/casecontrol_filtered.raw` 
  file which will be filtered to remove bad observations*. This results in having 2 files: `out/chr02/casecontrol_filtered_data.csv` 
  and `out/chr02/casecontrol_filtered_label.csv`
* where:
     - `out/chr02/casecontrol_filtered_data.csv` a csv data where columns denoting variables (SNPs)
        and rows denoting observations. `,` is used as delimiter.
     - `out/chr02/casecontrol_filtered_label.csv` a csv data where rows are SNPs.
        4 columns denoting: `chromosome`, `id`, `label` ("SNP_A-1865133_A"..), and `position` on the chromosome


### Under the hood:
* This script call PLINK to transform `.bed` to `.ped` ( which is why path to PLINK has to be specified ). A recoding has to be applied to transform the `genotype coding` to obtain `numeric coding` i.e. one of ADDITIVE/RECESSIVE/DOMINANT. Only ADDITIVE is supported for the time being. 
* Accompagnied with the `.bed` file we have a `.bim` which speficies the location of the SNPs ( i.e `chromosome`, ` genomic location` ...) sometimes the `chromosome` of these SNPs don't match while they are supposed to be the same for all. Typically a few SNPs at the beginning and at the end of the file do not have the same `chromosome` field value as the rest. This is the reason we have to take an addition step to filter out these SNPs, resulting in a sub-set of SNPs.
* In the output file, the `id` field represents the original `id` ( not necessarily starting as `0` )


