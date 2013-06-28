# -*- coding: utf-8 -*-
"""
Created on Thu Jun 27 10:14:01 2013

@author: ben
"""


fname="markup"

with open(fname,"r") as f:
    text=f.read().split("\n")
    
       
lines=[]    
    
for line in text:
    line=line.strip()    
    if line:
       lines.append(line)
       #print(line)
    


#print(lines)

level=0;
s=""
tab="    "
for line in lines:    
    if "Start" in line or "{" in line:
        level+=1
        s += tab*(level-1)+line +"\n"
        continue
    elif "End" in line or "}" in line:
        level-=1       
        s += tab*level+line  +"\n"
        continue
    
    s += tab*level+line  +"\n"

  
with open(fname, "w") as f:
    f.write(s)