import math
# Esto es solo una plantilla
# Obiamente llamar esta función cuando ya tenga almenos tres puntos y siempre pasar la lista ordenada.
# la lista tiene puntos x, y; donde x es el k y el 'y' es el f(k)

def dist(x, y):
    return sqrt(x*x + y*y)

def calculoTheta(x1, y1, x2, y2) :
    return acos( (x1*x2+y2*y2)/(dist(x1, y1)*dist(x2, y2)) )

def codo(listaOrdenada) :

    l = 0
    r = len(listaOrdenada) - 1
    k = (l+r)>>1#dividir entre dos

    # En la posicion 0 esta el 'x' y en la posicion 1 esta el 'y'
    # calculamos el theta inicial
    theta = calculoTheta(listaOrdenada[l][0]-listaOrdenada[k][0], listaOrdenada[l][1]-listaOrdenada[k][1],
            listaOrdenada[r][0]-listaOrdenada[k][0], listaOrdenada[r][1]-listaOrdenada[k][1])
    flag = True
    while(flag) :
        flag = False
        mid = (k+r)>>1#piso
        #k mid r
        # calculamos el theta temp por el lado derecho
        temp = calculoTheta(listaOrdenada[k][0]-listaOrdenada[mid][0], listaOrdenada[k][1]-listaOrdenada[mid][1],
            listaOrdenada[r][0]-listaOrdenada[mid][0], listaOrdenada[r][1]-listaOrdenada[mid][1])

        # Comprobamos si el theta temp es menor que el tetha actual
        if(theta > temp) :
            flag = True
            theta = temp
            l = k
            k = mid
        mid = math.ceil((k+l)/2)#techo

        # calculamos el theta temp por el lado izquierdo
        temp = calculoTheta(listaOrdenada[l][0]-listaOrdenada[mid][0], listaOrdenada[l][1]-listaOrdenada[mid][1],
            listaOrdenada[k][0]-listaOrdenada[mid][0], listaOrdenada[k][1]-listaOrdenada[mid][1])

        # comprobamos si el theta es menor
        if(theta > temp) :
            flag = True
            theta = temp
            r = k
            k = mid

    #l2,k5,r9
    return k
