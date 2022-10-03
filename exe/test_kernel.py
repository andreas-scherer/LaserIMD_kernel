# -*- coding: utf-8 -*-
"""
Created on Fri May 27 16:53:25 2022

@author: andi
"""

import numpy as np
import deerlab as dl
import matplotlib.pyplot as plt


def laserimdkernel_new(t,r, kernel):
    
    
    file = open(kernel+"_x.bin","rb")
    data=file.read()
    file.close()

    x=np.frombuffer(data, dtype=np.float64)
    
    
    file = open(kernel+"_y.bin","rb")
    data=file.read()
    file.close()

    k=np.frombuffer(data, dtype=np.float64)

    if t[-1]/r[0] > x[-1]:
        print('The range of Your  kernel list is too short for this combination of time and distance axis')
        

   
    K=np.zeros((len(t), len(r)))
    
    
    for l in range(len(r)):
            
        t0=t/(r[l]**3)
        
        y0=np.interp(t0,x,k)
        K[:,l]=y0
        
    return K
            
            




t=np.linspace(0,5,100)
r=np.linspace(1.5,5,100)


K=laserimdkernel_new(t,r,'TPP_Q')



P=dl.dd_gauss(r, [3,0.2])
P /= np.trapz(P,r)

plt.figure()
plt.plot(r,P)

V=K@P

plt.figure()
plt.plot(t,np.real(V))
plt.plot(t,np.imag(V))