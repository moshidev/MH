#!/bin/bash

files=$(ls data | sed 's/^/data\//' | sort -V | tr '\n' ',')
seeds=1,2,3,4,5

bin/main_exe greedy "$seeds" "$files" > resultados_greedy.dat
bin/main_exe localsearch "$seeds" "$files" > resultados_localsearch.dat
