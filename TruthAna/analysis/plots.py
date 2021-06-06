#!/usr/bin/env python3


import uproot3 as uproot
import matplotlib.pyplot as plt
import numpy as np
import awkward0
from argparse import ArgumentParser
import os

parser = ArgumentParser('')
parser.add_argument("inputFile")
parser.add_argument("outputDir")
args=parser.parse_args()

def makeTitle():
    global filename
    titleadddict={
    'fc_nm': 'fineCalo, naive bc removed',
    'fc_nms': 'fineCalo, naive switched off',
    'fc': 'fineCalo, naive remains on',
    'naive': 'naive boundary crossing'
    }
    
    filename = filename.split('/')[-1]
    particle = filename.split('_')[0]
    titlename = particle
    if 'fc_nm_' in filename:
        titlename += ", " +titleadddict['fc_nm']
    elif 'fc_nms_' in filename:
        titlename += ", " + titleadddict['fc_nms']
    elif 'fc_' in filename:
        titlename += ", " + titleadddict['fc']
    else:
        titlename += ", " + titleadddict['naive']
        
    plt.title(titlename)
        

os.system('mkdir -p '+args.outputDir)
outputDir = args.outputDir+'/'

tree = uproot.open(args.inputFile)["demo/tree"]
filename=args.inputFile

sc_energy = tree["sc_energy"].array()
st_energy = tree["st_energy"].array()

cp_energy = tree["cp_energy"].array()

n_sc = np.array(tree["n_sc"].array(), dtype='float')
n_sc_ass = np.array(tree["n_sc_ass"].array(), dtype='float')

sc_cp_ass = tree["sc_cp_ass"].array()
sc_no_ass = (sc_cp_ass*0.)
sc_no_ass[sc_cp_ass<0] = 1
nsc_no_ass = sc_no_ass.sum()






plt.hist((nsc_no_ass/n_sc).flatten(), bins=101)
plt.xlabel(r"Fraction of SC not associated to CP")
plt.yscale('log')
makeTitle()
plt.savefig(outputDir+"sc_cp_ass.pdf")
plt.close()

no_ass_energy = sc_energy[sc_no_ass>0].sum()

plt.hist((no_ass_energy/cp_energy).flatten(), bins=10)
plt.xlabel(r"Fraction of energy not associated to CP")
plt.yscale('log')
makeTitle()
plt.savefig(outputDir+"sc_cp_noass_en.pdf")
plt.close()


sc_ensum = sc_energy.sum()
st_ensum = st_energy.sum()
cp_ensum = cp_energy.sum()

plt.hist(cp_energy.flatten())
plt.xlabel(r"$E_{cp} [GeV]$")
plt.yscale('log')
makeTitle()
plt.savefig(outputDir+"cp_energy.pdf")
plt.close()



#now what do we plot
st_sc_ratio = st_energy / sc_energy
plt.hist(st_sc_ratio.flatten())
plt.xlabel(r"$E_{st0} / E_{sc}$")
plt.yscale('log')
makeTitle()
plt.savefig(outputDir+"st_sc_e_ratio.pdf")
plt.close()

plt.hist(n_sc)
plt.xlabel(r"$N_{sc}/N_{cp}$")
plt.yscale('log')
makeTitle()
plt.savefig(outputDir+"n_sc_n_cp_ratio.pdf")
plt.close()



ratio_sc = (sc_ensum / cp_ensum).flatten()
ratio_st = (st_ensum / cp_ensum).flatten()



plt.hist((cp_ensum - st_ensum).flatten(), bins=11)
plt.xlabel(r"$E_{CP} - \Sigma  E_{st0}$ [GeV]")
plt.yscale('log')
makeTitle()
plt.savefig(outputDir+"sum_e_st_e_cp_diff.pdf")
plt.close()

plt.hist((cp_ensum - sc_ensum).flatten(), bins=11)
plt.xlabel(r"$E_{CP} - \Sigma  E_{st0}$ [GeV]")
plt.yscale('log')
makeTitle()
plt.savefig(outputDir+"sum_e_sc_e_cp_diff.pdf")
plt.close()


plt.hist2d(cp_ensum.flatten(), (cp_ensum - st_ensum).flatten(), bins=51)
plt.ylabel(r"$E_{CP} - \Sigma  E_{st0}$ [GeV]")
plt.xlabel(r"$E_{CP}$ [GeV]")
makeTitle()
plt.savefig(outputDir+"sum_e_st_e_cp_diff2d.pdf")
plt.close()

plt.hist2d(cp_ensum.flatten(), (cp_ensum - sc_ensum).flatten(), bins=51)
plt.ylabel(r"$E_{CP} - \Sigma  E_{sc}$ [GeV]")
plt.xlabel(r"$E_{CP}$ [GeV]")
makeTitle()
plt.savefig(outputDir+"sum_e_sc_e_cp_diff2d.pdf")
plt.close()



plt.hist2d(cp_ensum.flatten(), (st_ensum/cp_ensum).flatten(), bins=51)
plt.ylabel(r"$\Sigma  E_{st0} / E_{CP}$")
plt.xlabel(r"$E_{CP}$ [GeV]")
makeTitle()
plt.savefig(outputDir+"sum_e_st_e_cp_ratio2d.pdf")
plt.close()


plt.hist(ratio_sc)
plt.xlabel(r"$\Sigma E_{sc} / E_{cp}$")
plt.yscale('log')
makeTitle()
plt.savefig(outputDir+"sum_e_sc_e_cp_ratio.pdf")
plt.close()

plt.hist(ratio_st)
plt.xlabel(r"$\Sigma E_{st0} / E_{cp}$")
plt.yscale('log')
makeTitle()
plt.savefig(outputDir+"sum_e_st_e_cp_ratio.pdf")
plt.close()




plt.hist(n_sc_ass/n_sc)
plt.xlabel(r"$N_{sc}(CP\ ass.)/N_{sc}$")
plt.yscale('log')
makeTitle()
plt.savefig(outputDir+"n_sc_ass.pdf")
plt.close()


