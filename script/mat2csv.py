import os
import scipy
from scipy import io
import pandas as pd

a=False
filepath=input()
if filepath.find("\"")!=-1:
    filepath=filepath.split("\"")[1]
try:
    name=filepath.split("\\")[-1].split(".")[0]
except:
    name=filepath.split("/")[-1].split(".")[0]
    a=True
matfile=scipy.io.loadmat(filepath)
datafile=list(matfile.values())[-1]
dfdata=pd.DataFrame(data=datafile)
try:
    path=filepath.split("\\")
except:
    path=filepath.split("/")
path=path[:-1]
p=""
if a:
    for i in path:
        p=p+i+"/"
else:
    for i in path:
        p=p+i+"\\"
datapath=p+name+"_r.csv"
dfdata.to_csv(datapath,index=True)
dfdata=dfdata.T
datapath=p+name+"_c.csv"
dfdata.to_csv(datapath,index=False)