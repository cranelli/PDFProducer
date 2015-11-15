import os
from argparse import ArgumentParser

parser = ArgumentParser(description='')
parser.add_argument('--filesDir', dest='filesDir', default=None, help='Directory to search for files.  If not provided, --files must be provided')
parser.add_argument('--outputDir', dest='outputDir', default=None, help='<Required> Output sample path.  If nproc is set to > 1 a job directory is appended')
parser.add_argument('--treeName', dest='treeName', default=None, help='<Required> name of tree to process')
                    
options = parser.parse_args()


def filter():
    # Create Executalbe
    os.system("make")

    #make output dir
    print "mkdir" + options.outputDir
    os.system("mkdir " +options.outputDir)
    input_files = []
    if options.filesDir is not None :
        print options.filesDir
        input_files = collect_input_files_local(options.filesDir)
    
    for input_file in input_files:
        command = "./PDFProducer.exe "+ input_file +" " + options.outputDir + "/tree.root " + options.treeName
        print command 
        os.system(command)
                    

# Finds the Files in a directory, using filekey
# Modified Version exists for reading files from eos
def collect_input_files_local( filesDir, filekey='.root' ) :
    input_files = []
    for top, dirs, files in os.walk(filesDir) :
        for f in files :
            if f.count(filekey) > 0 and os.path.isfile( top+'/'+f ) :
                input_files.append(top+'/'+f)

    return input_files


if __name__=="__main__":
    filter()
