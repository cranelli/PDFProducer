#!/bin/bash
LHAPDF_PATH=/cvmfs/cms.cern.ch/slc6_amd64_gcc472/external/lhapdf6/6.1.5-epnlpl/
ROOT_PATH=/afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.07_python2.7/x86_64-slc6-gcc46-opt/root/

export LD_LIBRARY_PATH=$LHAPDF_PATH/lib:$ROOT_PATH/lib:$LD_LIBRARY_PATH

#export LHAPDF_DATA_PATH=/afs/cern.ch/user/c/cranelli/public/WGamGam/PDFReweighting/PDFProducer/PDFSets
export LHAPDF_DATA_PATH=$LHAPDF_PATH/share/LHAPDF #:
