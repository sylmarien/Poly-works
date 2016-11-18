#!/bin/bash

#for batchSizes in 50 500 5000
#do
#  for learningRates in 0.05 0.01
#  do
#    for activationParameter in 0.1 0.01 0.001
#    do
#      for WInitValue in 0.1
#      do
#        for sizeHiddenLayerOne in 100 300
#        do
#          echo "python3 main.py ${batchSizes} ${learningRates} ${activationParameter} ${WInitValue} 0.01 1 ${sizeHiddenLayerOne}"
#          python3 main.py $batchSizes $learningRates $activationParameter $WInitValue 0.01 1 $sizeHiddenLayerOne | tee oneHidden_${batchSizes}_${learningRates}_${activationParameter}_${WInitValue}_${sizeHiddenLayerOne}.txt
#        done
#      done
#    done
#  done
#done

for batchSizes in 50 500 5000
do
  for learningRates in 0.05 0.01
  do
    for activationParameter in 0.1 0.01 0.001
    do
      for WInitValue in 0.1
      do
        for sizeHiddenLayerOne in 100 300
        do
          for sizeHiddenLayerTwo in 50 100 300
          do
            echo "python3 main.py ${batchSizes} ${learningRates} ${activationParameter} ${WInitValue} 0.01 2 ${sizeHiddenLayerOne} ${sizeHiddenLayerTwo}"
            python3 main.py $batchSizes $learningRates $activationParameter $WInitValue 0.01 2 $sizeHiddenLayerOne $sizeHiddenLayerTwo | tee twoHidden_${batchSizes}_${learningRates}_${activationParameter}_${WInitValue}_${sizeHiddenLayerOne}_${sizeHiddenLayerTwo}.txt
          done
        done
      done
    done
  done
done
