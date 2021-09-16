"""
INSTALACIONES:
    pip3 install numpy
    pip3 install matplotlib
    sudo apt-get install python3-tk
    sudo apt-get install python-scipy
"""
import numpy as np
import matplotlib.pyplot as plt
import scipy.optimize as op

# *******************************************************************
#***********    Configuracion de parametros    **********************
# *******************************************************************
# Angulos del movimiento:
ang_inicial = 10
ang_final   = 90
# Delta de tiempo para una interrupcion ms:
time_intr = 50
# Tiempo del ciclo en ms:
time_ciclo = 2000 
# limites de posicion del servo en microsegundos y grados:   
mic_izq = 1000
mic_der = 2000
ang_izq = 0
ang_der = 180
# *******************************************************************
# *******************************************************************
# *******************************************************************


# Solucion de la ecuacion no lineal 
f  = lambda x:  np.sin(x)/( 1 - np.cos(x) ) + 1 
tc = op.bisect( f, 2, 6, xtol=1.e-6, maxiter=100 ) 
# print( "Solucion para t:{}".format(tc) ) 


r = 1/2
xc = r*( tc - np.sin(tc) )
yc = 1

puntos =  int (time_ciclo / time_intr )
t = np.linspace( np.pi, tc, puntos )
x1 = r*( t - np.sin(t) )
y1 = r*( 1 - np.cos(t) )

x2 = -r*( t - np.sin(t) ) + xc + r*( tc -np.sin(tc) )
y2 = -r*( 1 - np.cos(t) ) + yc + r*np.cos( tc )



fig, ax = plt.subplots()
line1, = ax.plot( x1, y1, label='Cicloide 1' )
line2, = ax.plot( x2, y2, label='Cicloide 2' )



# swap el vector y2:
index_end = y2.size 
temp = np.linspace( 1, index_end, index_end)
index_ini = 0
while index_end > 0 :
    temp[index_ini] = y2[index_end -1]
    index_end = index_end - 1
    index_ini = index_ini + 1
y2 = temp

# construccion de un unico vector:
y3 = np.concatenate( [y1,y2] )
# Obtencion del delta:
ang_delta = ang_inicial - ang_final
# conversion de grados a microsegundo:
m = (mic_der - mic_izq)/(ang_der - ang_izq)
t_ms = lambda ang: m*(ang - ang_der) + mic_der

# construccion del vector:
vector_text = "{"
for y in y3:
    # imprimir ',' si no es el ultimo elemento
    if y != y3[-1] :
        vector_text = vector_text + "{}, ".format( int(t_ms( ang_final + ang_delta*y )))
    else:
        vector_text = vector_text + "{}".format( int(t_ms( ang_final + ang_delta*y )))

vector_text = vector_text + "}"

print("Vector calculado:{}".format(vector_text) )
print("Total de elementos por vector:{}".format(y3.size) )
# ploteo de la graficas:
ax.legend()
plt.show()