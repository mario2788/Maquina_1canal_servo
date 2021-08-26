import numpy as np
import matplotlib.pyplot as plt
import scipy.optimize as op

f  = lambda x:  np.sin(x)/( 1 - np.cos(x) ) + 1
xc = op.bisect(f,2,6,xtol=1.e-6,maxiter=100)  
print( xc )

x = np.linspace(0,2*np.pi , 100)
# x = np.linspace(-1,20,1000)

plt.figure(figsize=(9,4))
plt.plot(x,f(x))
plt.show()