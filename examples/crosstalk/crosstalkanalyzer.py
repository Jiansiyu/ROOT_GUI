class stripmap:
    def __init__(self, maptype=None):
        self._mapType='INFN'
        if maptype is "UVa":
            self._mapType='UVa'
        else:
            print("Unknow map type")
        self.map={}
        self.inverse_map={}
        
    def GetMap(self):
        array=[]
        for stripNb in range(0,128,1):
            RstripNb=32*(stripNb%4)+8*(int)(stripNb/4)-31*(int)(stripNb/16)
            array.append(RstripNb)
            self.map[stripNb]=RstripNb
            self.inverse_map[RstripNb]=stripNb
        return array,self.map,self.inverse_map

    def strip_map(self,stripNb,RstripNb):
        return stripNb, 32*(stripNb%4)+8*(int)(stripNb/4)-31*(int)(stripNb/16)

    def CrossTalkScan(self,cluster_size=2):
        GEM_signal_array=[]
        for stripNb in range(0,128,1):
            if stripNb + cluster_size <= 128:
                temp=[]
                for stripid in range(cluster_size):
                    temp.append(stripid+stripNb)
                GEM_signal_array.append(temp)
        # project to APV chip, and inside the GEM chip generate cross talk
        for gem_signal in GEM_signal_array:
            
            apv_signal=self._GEM2APV_map(gem_signal)
            print gem_signal
            print apv_signal
            apvcrosstalk=self._generateCrossTalk(apv_signal)
            print apvcrosstalk
            self._crosstalkIndentification(apvcrosstalk)
    
    def _APV2GEM_map(self,array=[]):
        re_array=[]
        for i in array:
            re_array.append(self.map[i])
        return re_array
    
    def _GEM2APV_map(self,array):
        re_array=[]
        for i in array:
            re_array.append(self.inverse_map[i])
        return re_array
    
    def _generateCrossTalk(self,apvSignalNB):
        array=[]
        for apv_strip in apvSignalNB:
            if apv_strip > 0:
                array.append(apv_strip-1)
            #array.append(apv_strip)
            if apv_strip < 127:
                array.append(apv_strip+1)
        return array

    def _crosstalkIndentification(self,apvcrosstalkarray):
        gemsignalarray=self._APV2GEM_map(apvcrosstalkarray)
        gemsignalarray.sort()
        if len(gemsignalarray) > 1:
            reference=gemsignalarray[0:]
            print reference
            print
            
            
        
if __name__ == "__main__":
    test=stripmap();
    a,b,c=test.GetMap()
    test.CrossTalkScan()
    print b