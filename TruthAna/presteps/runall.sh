#!/bin/bash

function run {
file=$1
cmsRun $CMSSW_BASE/src/HGCALAnalysis/TruthAna/presteps/step1.py outputFile="${file}_1.root" pid=$2 finecalo=$3 maxEvents=50 seed=$4 marco=True npart=$5
cmsRun $CMSSW_BASE/src/HGCALAnalysis/TruthAna/presteps/step2.py inputFiles=file:"${file}_1.root" outputFile="${file}_2.root" 
cmsRun $CMSSW_BASE/src/HGCALAnalysis/TruthAna/python/runit.py inputFiles=file:"${file}_2.root" outputFile="${file}_ntup.root" 
}

#test

run gamma_test 22 False 1 1 
exit

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




