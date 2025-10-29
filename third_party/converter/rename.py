import os

res = []

for file_path in os.listdir(os.getcwd()):
    if os.path.isfile(os.path.join(os.getcwd(), file_path)):
        res.append(file_path)


for i in res:
    if (i == 'a.py'): continue
    new = str(int(i[6:8])) + '.png'
    os.rename(i, new)