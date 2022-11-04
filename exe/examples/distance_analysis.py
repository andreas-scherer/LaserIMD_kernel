# -*- coding: utf-8 -*-
"""
Created on Sun Oct 23 14:27:34 2022

@author: Andreas Scherer
"""

import numpy as np
import matplotlib.pyplot as plt
import deerlab as dl


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






filename='data\TPP_AA_5_NO_X_band.txt'
data = np.loadtxt(filename)
t=data[:,0]
V=data[:,1]

#Preprocessing:

t0=0.2176
t=t-t0
V = V[t>=0]
t = t[t>=0]
cutoff = 1.4
V = V[t<cutoff]
t = t[t<cutoff]


# Generate the Kernel
r=np.linspace(1.5,5,1024)

K=laserimd_kernel(t,r, 'TPP_9_28_GHz')


def laserimdmodel(p):
    lam,k = p
    B = dl.bg_exp(t,k)
    Kb=lam*K+(1-lam)
    Kb=Kb*B[:,np.newaxis]
    
    
    return Kb


#define the limits for the fir routine
par0 = [0.5, dl.bg_exp.start[0] ] # Start values
lb   = [ 0, dl.bg_exp.lower[0]] # lower bounds
ub   = [ 1, dl.bg_exp.upper[0]] # upper bounds
lbl = np.zeros_like(r) # Non-negativity constraint of P
ubl = [] # Unconstrained upper boundary
    
fit = dl.snlls(V,laserimdmodel,par0,lb,ub,lbl,ubl)




fig,ax=plt.subplots(1,2,figsize=[7,6])

ax[0].plot(t,V,'.',color='grey',alpha=0.5)
ax[0].plot(t,fit.model,'tab:blue')
ax[0].grid(alpha=0.3)
ax[0].set_xlabel('Time (µs)')
ax[0].set_ylabel('V')
ax[0].legend(('Data','Vfit'))


ax[1].plot(r,fit.lin,'tab:blue',label='Distance')
ax[1].set_xlabel('Distance (nm)')
ax[1].set_ylabel('P (nm⁻¹)')
ax[1].legend()
ax[1].grid(alpha=0.3)
plt.tight_layout()
