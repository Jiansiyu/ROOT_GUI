# GEM Detector Analyzer Package

## Module Layout

### GEM Analysis module

*  GEM Data file Parser
*  GEM Config
*  Zero Subtraction 
*  Common Mode
*  map read
*  pedestal io
*  cross talk removal 
*  detector position correction
*  cluster searching
*  cluster matching
*  GEM detector porformance analysis 
*  GEM time fit
*  tree search 
*  tracking finding and fitting

#### GEM analysis folder and class

##### GEM decoder
*  GEM Raw Parser(evio, simulation root etc.)
*  GEM Pedestal analyzer
*  GEM Common Mode analyzer
*  GEM Zero Subtraction analyzer
*  GEM detector crosstalk analyzer
*  GEM file IO(evio, root etc)
*  GEM Data exchang

##### GEM Analysis
*  


##### massage, log and error report
* massage infor 
* log io
* bug report

##### GEM common 
*  GEM UID Generator(DetectorID, strip ID, detectorID etc.)
*  GEM Pedestal IO
*  GEM Config IO

### GUI config
* GUI Class 
* GUI Subframe class
* GUI Config
* GUI Infor

### multithread 
* Thread pool class

### particle identification class
* machine learning io

### examples

### scripts
* python
* bash 


## GEM data structure consideration

### GEM Raw level

### GEM event

## GEM Analysis Configuration file 

* GEM detector mapping
* GEM detector sigma cut
* GEM detector size and strips
* GEM detector position correction matrix or correction data file
* GEM detector cut(enable flag)


## Problems


## contact 

jiansiyu@gmail.com
