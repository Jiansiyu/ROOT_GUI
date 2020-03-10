# develop plan for the week
* change the canvas style,remapping before draw
* add the summary tab
* optmize the data structure 
* change root 6 to root 5 
* add another tab which display all the apv in one canvas used for diagnose perpose
* draw performance optimize (fill and draw)
* ssp suport


# todo 
* check the raw level, compare the cluster size, whether there is mistake in the mapping
* display by GEM layer
* title on each histograms ( Module GEM MPD APV cards)
* for zero mode, add the title staff, make the x-axis just the strip that have to solve confuse, should  not be hard coded
* zero mode, one summary tab
* add the pedestal multi-file support
* average ADC/ channe;
* check the peak in the cluster charge


# New TDataframe strcuture 

### Decoder plan 
- [ ] First step, decode the raw data and save to root file 
- [ ] load the root file into TDataframe
- [ ] Concurrent processs all event 

#### Raw Display plan

- [ ] Generate the Histogram in the subfolder, 
- [ ] in the return scripts, it only return a trigger signal 
- [ ] in the respond scripts code, it use gROOT to look for the histogram


#### Display code plan 

- [ ] a global code that used for trigger the signal/slot that can handle all the display requirement 



