# -*- coding: utf-8 -*-
"""
Created on Mon Oct  3 22:14:37 2022

@author: Andreas Scherer
"""

import numpy as np
import deerlab as dl
import matplotlib.pyplot as plt

def laserimd_kernel(t,r, kernel, lam=1, B=None):
    
    
    file = open(kernel+"_x.bin","rb")
    data=file.read()
    file.close()

    x=np.frombuffer(data, dtype=np.float64)
    
    
    file = open(kernel+"_y.bin","rb")
    data=file.read()
    file.close()

    k=np.frombuffer(data, dtype=np.float64)

    if t[-1]/r[0]**3 > x[-1]:
        print('The range of Your  kernel list is too short for this combination of time and distance axis')
        

   
    K=np.zeros((len(t), len(r)))
    
    
    for l in range(len(r)):
            
        t0=t/(r[l]**3)
        
        y0=np.interp(t0,x,k)
        K[:,l]=y0
        
        
    K = lam*K + (1-lam)
    
    
    if B is not None:
        
        K=K*B[:,np.newaxis]
        
        
    dr = np.zeros(len(r))
    dr[0] = r[1] - r[0]
    dr[-1] = r[-1] - r[-2]
    dr[1:-1] = r[2:] - r[0:-2]
    dr = dr/2
    K = K*dr
    
        
    return K



if __name__=='__main__':
    
    r=np.linspace(1.5,7,1024)
    t=np.linspace(0,6,1024)
    
    mu=3
    sigma=0.1
    
    distr=dl.dd_gauss(r, [mu,sigma])
    
    lam=0.5
    c=10
    
    B=dl.bg_hom3d(t, c, lam)
    
    K=laserimd_kernel(t,r, 'TPP_9_3_GHz',lam=lam, B=B)
    
    V=K@distr
    
    fig,ax= plt.subplots(2,1)
    
    ax[1].plot(t, V)
    ax[0].plot(r,distr)
    

