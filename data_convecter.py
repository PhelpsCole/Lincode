import sys
import re

mode = 0

if len(sys.argv) < 2:
    print("Input filename")
else:
    if len(sys.argv) >= 3:
        mode = sys.argv[2]
    spectMap = {}
    with open(sys.argv[1]) as f:
        for line in f:
            if mode == 0:
                elem = re.split('\]:|, \[', line.rstrip())[1]
            else:
                elem = re.split('\]:{|} \[', line.rstrip())[1]
            #print(line)
            #print(elem)
            if elem in spectMap:
                spectMap[elem] += 1
            else:
                spectMap[elem] = 1
        for k, v in spectMap.items():
            elem = re.split(';|:|\[|\]|{', k.rstrip())
            elem = " & ".join([x for x in elem if x!=''][1::2])
            #print(v, elem, sep=" &")
            print(k, v, sep=" ")
