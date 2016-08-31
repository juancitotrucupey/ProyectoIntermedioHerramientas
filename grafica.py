#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Aug 29 11:17:52 2016

@author: francisco
"""

import numpy as np
import matplotlib.pyplot as plt
import numpy.ma as ma


###import datos
atc,ats=np.loadtxt('datos2.dat',unpack=True)

###reformando datos


##quitando ceros en ataque y mascara
mask1=ats!=0
atc1=atc[mask1]
ats1=ats[ats!=0]
###quitando datos para hallar pendiente
ats2=ats1[(ats1)>0.00045]
mask2=(ats1)>0.00045
atc2=atc1[mask2]




##plotss
plt.loglog(atc,ats,'ro',markersize=13)
m,b=np.polyfit(np.log(atc2),np.log(ats2),1)
x=np.arange(50)
plt.plot(np.exp(x),np.exp(m*x+b),'k')
plt.axis([0,100,0.0001,1])
plt.text(4,0.02,'$y=-2.38x-0.7$',fontsize=15)
plt.xlabel('Fuerza de ataque')
plt.ylabel('Unidades de ataque con fuersa de ataque s')


plt.title('Grafica Log-Log numero de unidades de ataque Vs Fuerza de ataque ')





