#!/bin/bash

function run {
file=$1
cmsRun step1.py outputFile="${file}_1.root" pid=$2 finecalo=$3 maxEvents=100 seed=$4 marco=False npart=$5
cmsRun step2.py inputFiles=file:"${file}_1.root" outputFile="${file}_2.root" 
cmsRun runit.py inputFiles=file:"${file}_2.root" outputFile="${file}_ntup.root" 
}

#run gamma 22 False &
#run gamma_fc 22 True &
#
#run cpion 211 False &
#run cpion_fc 211 True &
#
#run cpion_ext -211 False &
#run cpion_fc_ext -211 True &

#for i in `seq 1 20`
#do
#run gamma_$i 22 False $i 1 &
#run cpion_$i 211 False $i 1 &
#done
#wait

#finecalo
#for i in `seq 1 20`
#do
#run gamma_fc_$i 22 True $i 1 &
#run cpion_fc_$i 211 True $i 1 &
#done
#wait

#finecalo nms (already done)

#compile again

#finecalo nm
for i in `seq 1 20`
do
run gamma_fc_nm_$i 22 True $i 1 &
run cpion_fc_nm_$i 211 True $i 1 &
done
wait


#no finecalo nm
for i in `seq 1 20`
do
run gamma_nm_$i 22 False $i 1 &
run cpion_nm_$i 211 False $i 1 &
done
wait




