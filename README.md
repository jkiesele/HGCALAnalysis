```
cmsrel CMSSW_12_0_0_pre2
cd CMSSW_12_0_0_pre2/src
git clone https://github.com/jkiesele/HGCALAnalysis
scram b -j12
```

Check options in https://github.com/jkiesele/HGCALAnalysis/blob/master/TruthAna/presteps/step1.py.

Script to run all: https://github.com/jkiesele/HGCALAnalysis/blob/master/TruthAna/presteps/runall.sh
(adapt to your needs)

Make plots with https://github.com/jkiesele/HGCALAnalysis/blob/master/TruthAna/analysis/plots.py. (may need additional python packages)

