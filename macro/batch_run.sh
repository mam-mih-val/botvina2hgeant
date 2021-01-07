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

$build_dir/convert $filelist input.evt

cp $build_dir/../macro/make_config/template_config.dat config.dat
echo $(pwd)/input.evt >> config.dat
echo $(pwd)/geant_out.root >> config.dat

/cvmfs/hades.gsi.de/install/5.34.34/hgeant2-5.5/hgeant -c -f config.dat no no no

mkdir dst

/lustre/hebe/hades/user/fkornas/sub/apr12/OwnEvt/dst/analysisDST geant_out.root dst/ 10000

echo JOB FINISHED!
date $format
