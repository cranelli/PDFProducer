
LHAPDF_PATH = /cvmfs/cms.cern.ch/slc6_amd64_gcc472/external/lhapdf6/6.1.4
ROOT_PATH = /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.07_python2.7/x86_64-slc6-gcc46-opt/root

ROOTLIBS= $(shell root-config --libs)

all:
	g++ PDFProducer.cpp -L $(LHAPDF_PATH)/lib $(ROOTLIBS) -l LHAPDF -I $(LHAPDF_PATH)/include/ -I $(ROOT_PATH)/include/ -o PDFProducer.exe

clean:
	rm PDFProducer.exe