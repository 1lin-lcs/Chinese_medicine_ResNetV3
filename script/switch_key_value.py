import os
import json

json_file=r"E:\文件\Documents\Python\pytorch_learning\Chinese_Medicine.json"


w=open("./Chinese_Medicine_for_test.json","w")
j=open(json_file,"r")
js=json.load(j)

a={}

for i in js:
    a[str(js[i])]=i

json.dump(a,w,indent=4)
w.close()
j.close()