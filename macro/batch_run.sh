#!/bin/bash

format='+%Y/%m/%d-%H:%M:%S'

date $format

job_num=$(($SLURM_ARRAY_TASK_ID))

filelist=$(ls $lists_dir | sed "${job_num}q;d")

cd $output_dir
mkdir -p $job_num
cd $job_num

echo "loading " $hadesroot
source $hadesroot

echo executing $build_dir/convert $lists_dir/$filelist input.evt
$build_dir/convert $lists_dir/$filelist input.evt

#cp $build_dir/../macro/template_config.dat config.dat
#echo $(pwd)/input.evt >> config.dat
#echo $(pwd)/geant_out.root >> config.dat
#
#/cvmfs/hades.gsi.de/install/5.34.34/hgeant2-5.5/hgeant -c -f config.dat no no no
#
#mkdir dst
#
#/lustre/hebe/hades/user/fkornas/sub/apr12/OwnEvt/dst/analysisDST geant_out*.root dst/ 10000

echo JOB FINISHED!
date $format
