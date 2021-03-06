#!/bin/bash

file_list=$1
output_dir=$2

hadesroot=/cvmfs/hades.gsi.de/install/5.34.34/hydra2-5.5a/defall.sh

current_dir=$(pwd)
partition=long
time=7-00:00:00
build_dir=/lustre/nyx/hades/user/mmamaev/botvina2hgeant/build

lists_dir=${output_dir}/lists/
mkdir -p $lists_dir
log_dir=${output_dir}/log
mkdir -p $log_dir

split -l 1 -d -a 3 --additional-suffix=.list "$file_list" $lists_dir

n_runs=$(ls $lists_dir/*.list | wc -l)

job_range=1-$n_runs

echo file list=$file_list
echo output_dir=$output_dir
echo log_dir=$log_dir
echo lists_dir=$lists_dir
echo n_runs=$n_runs
echo job_range=$job_range

sbatch -J Botvina -p $partition -t $time -a $job_range -e ${log_dir}/%A_%a.e -o ${log_dir}/%A_%a.o -D $output_dir --export=output_dir=$output_dir,file_list=$file_list,hadesroot=$hadesroot,lists_dir=$lists_dir,build_dir=$build_dir -- /cvmfs/vae.gsi.de/debian8/containers/debian8-user_container_20210327T0735.sif /lustre/nyx/hades/user/mmamaev/botvina2hgeant/macro/batch_run.sh