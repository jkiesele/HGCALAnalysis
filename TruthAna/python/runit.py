import FWCore.ParameterSet.Config as cms

process = cms.Process("Truth")

process.load("FWCore.MessageService.MessageLogger_cfi")


from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('python')
options.setDefault('inputFiles', 'file:step2.root')
options.setDefault('outputFile', 'file:out.root')
options.parseArguments()


process.TFileService = cms.Service("TFileService", fileName = cms.string(options.__getattr__("outputFile", noTags=True)))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
            options.inputFiles
                )
                            )

process.demo = cms.EDAnalyzer('TruthAna',
                              SimClusters  = cms.InputTag("mix", "MergedCaloTruth"),
                              CaloParticles = cms.InputTag("mix", "MergedCaloTruth"),
                              )

process.p = cms.Path(process.demo)