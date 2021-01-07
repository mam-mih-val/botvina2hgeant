#!/bin/bash

format='+%Y/%m/%d-%H:%M:%S'

date $format

job_num=$(($SLURM_ARRAY_TASK_ID))

filelist=$(sed '${job_num}q;d' `ls $lists_dir`)

cd $output_dir
mkdir -p $job_num
cd $job_num

echo "loading " $hadesroot
source $hadesroot

$build_dir/HT2AT -i $input_files -o output.root -c $config_file

echo JOB FINISHED!
date $format
