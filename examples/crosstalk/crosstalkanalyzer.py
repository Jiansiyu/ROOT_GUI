class stripmap:
    def __init__(self, maptype=None):
        self._mapType='INFN'
        if maptype is "UVa":
            self._mapType='UVa'
        else:
            print("Unknow map type")
    def GetMap(self):
        array=[]
        for stripNb in range(0,128,1):
            RstripNb=32*(stripNb%4)+8*(int)(stripNb/4)-31*(int)(stripNb/16)
            array.append(RstripNb)
            print('{0:2d},'.format(RstripNb))
        return array

