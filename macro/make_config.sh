#!/bin/bash

input_file=$1
output_file=$2

cp template_config.dat config.dat

echo $input_file >> config_$input_file.dat
echo $output_file >> config_$input_file.dat