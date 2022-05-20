#!/bin/bash

function echo_average_time() {
    algorithm=$1
    seeds=$2
    file=$3
    num_of_execs=$4
    sum_times=0
    for i in `seq 1 1 $num_of_execs`
    do
        elapsed_time=$(bin/main_exe "$algorithm" "$seeds" "$file" | grep --only-matching '[0-9][0-9]*\.[0-9][0-9]*e*-*[0-9][0-9]*$')
        elapsed_time=$(sed -E 's/([+-]?[0-9.]+)[eE]\+?(-?)([0-9]+)/(\1*10^\2\3)/g' <<< "$elapsed_time") # https://stackoverflow.com/questions/12882611/how-to-get-bc-to-handle-numbers-in-scientific-aka-exponential-notation
        sum_times=$(bc -l <<< "$sum_times + $elapsed_time")
    done
    echo $(bc -l <<< "scale=12; $sum_times / $num_of_execs")
}

files=$(ls data | sed 's/^/data\//' | sort -V | tr '\n' ',')
files_wspaces=$(echo $files | tr ',' ' ')
algorithms="greedy localsearch"
seeds=1,2,3,4,5

for a in $algorithms
do
    echo > average_times_$a.dat
    for f in $files_wspaces
    do  
        # mala significancia estadística... sería mejor hacerlo con google-benchmark
        echo -e "$f\t$(echo_average_time "$a" "$seeds" "$f" 1)" >> average_times_$a.dat
        valores=$(bin/main_exe "$a" "$seeds" "$f" | cut -d$'\t' -f 2,3,4,5,6)
        sum=0
        for v in $valores
        do
            sum=$(bc -l <<< "$sum + $v")
        done
        avg=$(bc -l <<< "$sum / 5")
        echo -e "$f\t$avg" >> dispersion_media_$a.dat
    done
done

bin/main_exe greedy "$seeds" "$files" > resultados_greedy.dat
bin/main_exe localsearch "$seeds" "$files" > resultados_localsearch.dat
