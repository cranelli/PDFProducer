# Run Over Samples
# Example python scripts/scheduler.py
import os
from argparse import ArgumentParser

#WGamGam
#base = r'/afs/cern.ch/work/c/cranelli/public/WGamGam/Acceptances'
#base = r'/afs/cern.ch/work/c/cranelli/public/WGamGam/ReFilterFinalNtuple'

#ZGamGam
base = r'/afs/cern.ch/work/c/cranelli/public/ZGamGam/ReFilterFinalNtuple'
#base = r'/afs/cern.ch/work/c/cranelli/public/ZGamGam/Fiducial/Dressed'

#WGamGam
#input_name = "CommonFiducial_NLO_wMT_Dress500MeV_Skim_PUWeights"
#input_name = "NLO_LepGammaGammaFinalElandMuUnblindAll_2015_10_27_ScaleFactors"

#ZGamGam
input_name = 'NLO_LepLepGammaGammaFinalElElandMuMuUnblindAll_2015_10_01_ScaleFactors'
#input_name= 'CommonFiducial_NLO_Skim_2015_09_22_PUWeights'

output_name = input_name +'_PDFReweights'

jobs_mc = [
    # WGamGam
    #(base, 'job_NLO_WAA_ISR'),
    #(base, 'job_NLO_WAA_FSR'),

    #(base, 'job_NLO_WAA_ISRwithSF'),
    #(base, 'job_NLO_WAA_FSRWithSF'),

    # ZGamGam
    (base, 'llaa_nlo_ggNtuple'),
]

treename='ggNtuplizer/EventTree'
#exeName='PDFProducer.exe'

#Set up Executable, and run jobs

#Make Output Directory
os.system('mkdir ' + base + "/"+ output_name)

for base, job in jobs_mc:
    filesDir =  base+'/'+input_name+'/'+job 
    outputDir =  base+'/'+output_name+'/'+job
    command = 'python scripts/filter.py --filesDir '+ filesDir + ' --outputDir ' + outputDir +' --treeName '+ treename
    print command
    os.system(command)


