import os
import pandas as pd

root_dir=r"E:\文件\Documents\Python\pytorch_learning\中草药"
all_dir=os.listdir(root_dir)
#print(all_dir)

datasets_dict={}

for dir in all_dir:
    file_path=os.path.join(root_dir,dir)
    files=os.listdir(file_path)
    datasets_dict[dir]=files

#print(datasets_dict)

datasets_df=pd.DataFrame.from_dict(datasets_dict,orient='index')

#print(datasets_df)
#datasets_df.to_excel("dataset_excel.xlsx",sheet_name="datasets",na_rep="NaN")