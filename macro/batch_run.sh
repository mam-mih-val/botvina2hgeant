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

#for start in 0 10000 20000 30000 40000
for start in 0
do
  mkdir $start
  cd $start
  echo goto $start
  echo

  "echo executing $build_dir/convert $lists_dir/$filelist input.evt $start $((start+10000))"
  $build_dir/convert $lists_dir/$filelist input.evt $start $((start+10000))

  cp $build_dir/../macro/config_ag158.dat config.dat
  echo $(pwd)/input.evt >> config.dat
  echo $(pwd)/geant_out.root >> config.dat

  /cvmfs/hades.gsi.de/install/5.34.34/hgeant2-5.5a/hgeant -c -f config.dat no no no
  mkdir dst
  ls geant_out*.root > geant_out.list
  while read line; do
    input_files=$input_files","$line
  done < geant_out.list
  echo "executing /lustre/nyx/hades/user/mmamaev/dst_agag/analysisDST geant_out1.root dst/ 10000 1"
  /lustre/nyx/hades/user/mmamaev/dst_agag/analysisDST geant_out1.root dst/ 10000
  cd ..
done

echo JOB FINISHED!
date $format
