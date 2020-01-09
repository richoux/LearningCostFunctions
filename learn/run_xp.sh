#!/bin/bash

cd spaces

for file in $(ls ad*-la.txt lt*-la.txt le*-la.txt cm*-la.txt ol*-la.txt ); do
    parse=(${file//-/ })
    constraint=${parse[0]}
    vars=${parse[1]}
    domain=${parse[2]}
    sample=${parse[3]}
    param=${parse[4]}
    
    if [[ $sample == 100 ]]; then		
				if [[ $param != "la.txt" ]]; then
						for i in {1..100}; do
								../bin/learn_cf_ga -c $constraint -n $vars -d $domain -p $param -i $file --xp >> ../results/$constraint-$vars-$domain-$sample-$param.txt
						done
				else
						for i in {1..100}; do
								../bin/learn_cf_ga -c $constraint -n $vars -d $domain -i $file --xp >> ../results/$constraint-$vars-$domain-$sample.txt
						done
				fi
    fi
done