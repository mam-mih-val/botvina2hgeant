#!/bin/bash

template=$1
input_file=$2
output_file=$3

cp $template config.dat

echo $input_file >> config.dat
echo $output_file >> config.dat