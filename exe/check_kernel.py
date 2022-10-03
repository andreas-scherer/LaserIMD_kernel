# -*- coding: utf-8 -*-
"""
Created on Sun May 29 18:42:07 2022

@author: andi
"""

import matplotlib.pyplot as plt
import numpy as np
import deerlab as dl

kernel='TPP_9_3_GHz2'

file = open(kernel+"_x.bin","rb")
data=file.read()
file.close()

x=np.frombuffer(data, dtype=np.float64)


file = open(kernel+"_y.bin","rb")
data=file.read()
file.close()

k=np.frombuffer(data, dtype=np.float64)




plt.plot(x,k)