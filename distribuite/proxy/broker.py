import zmq
import sys
import math
import numpy

class Broker:

    context = zmq.Context()
    router = context.socket(zmq.ROUTER)
    #poller = zmq.Poller()
    p = 0


    def __init__(self, n):

        self.op = {"WorkDone":self.serverResponse, "serverFREE":self.serverFree}
        self.router.bind("tcp://*:5000")

        #elementos para realizar la operacion
        self.n = n
        self.bestK = None
        self.obtenido = {}
        #self.colaK = [1, self.n//2, self.n]

        #self.colaK = [1, numpy.random.randint(2,self.n/2),numpy.random.randint(self.n/2,self.n + 1)]
        self.colaK =  [1,int(self.n/8), int(self.n/4 + 1)]
        self.cantKCalculate = 0



    def serverResponse(self, data):

        print("el servidor acabo de evaluar un k")
        print(data[1])
        print(data)
        #guardar el resultado en una estructura de datos, evaluar estructura de datos
        kObtenido = int(data[2].decode())
        ssdobtenido = float(data[3].decode())
        if kObtenido in self.obtenido:
            print("el k ya habia sido calculado")
        else:
            self.obtenido[kObtenido] = ssdobtenido
            print("obtenido k: " , kObtenido, "su ssd:", ssdobtenido)
            self.cantKCalculate += 1

    def serverFree(self, data):

        print("un servidor esta libre se le va asignar un k para que trabaje")
        print(data[1])

        #validar si no tengo que conseguir mas k
            #enviar un mensaje diciendole al sever que termino para que no mande mas trabajo
        #sacar k que necesito pedir de alguna estructura de datos
        #msj = None
        if self.cantKCalculate <= math.sqrt(self.n):

            if len(self.colaK): #hay elementos para enviar
                ktocalc = self.colaK.pop(0)
                msj = [data[0], b'KMEANS', str(ktocalc).encode()]#, b"probando"]
                self.router.send_multipart(msj)
                #self.p += 1#tengo un k adicional
            else:#espere que no  hay trabajo

                msj = [data[0], b'WAIT', b"0"]#, b"probando"]
                self.router.send_multipart(msj)
                #self.p += 1#tengo un k adicional
        else:
            print("ha finalizado el proceso no puedo enviar mas")
            msj = [data[0], b'Finish', b"0"]#, b"probando"]
            self.router.send_multipart(msj)
            #self.p += 1



    def run(self):
        print("Running the server Broker....")

        while True:#cambiar esto hasta que el numero que K que haya pedido sea raiz de n


            #print("revisando si un server ha solicitado")

            if self.router.poll(100):
                print("-----------un servidor ha hecho una solicitud--------------")
                msj = self.router.recv_multipart()
                #print("lo que me envia el server:", msj[1])
                self.op[msj[1].decode()](msj)


            #validar los k que tengo si son suficientes
            #validar el k apropiado hasta este momento
            #agregar a una cola que K's voy a pedir
            if len(list(self.obtenido.keys())) >= 3:
                print("calculando elbow")
                a,b,c = self.elbow2()
                print("k a buscar", a,b,c)
                try:
                    self.colaK.append(numpy.random.randint(a,b+1))
                    self.colaK.append(numpy.random.randint(b, c+1))
                    #self.colaK.append(numpy.random.randint(1, 3000))
                    #self.colaK.append(numpy.random.randint(1, 3000))
                except Exception as e:
                    print("hubo un erro y no se peuden agregar k a la cola")


                #self.colaK.append(numpy.random.randint(l,m+1))
                #self.colaK.append(numpy.random.randint(m, r+1))
                #distribuciones y agregar a la cola de k
            print("el mejor k hasta el momento:" , self.bestK)



    def dist(self, x, y):
        return math.sqrt(x*x + y*y)

    def calculoTheta(self, x1, y1, x2, y2) :
        var = (x1*x2+y2*y2)/(self.dist(x1, y1)*self.dist(x2, y2))
        print("el valor a calcular en el acos", var)
        if var > 1:
            var = 1

        if var < -1:
            var = -1
        res = math.acos(var)
        print("el valor del theta calculado es:", res)
        return res

    def elbow2(self):
        listaOrdenada = list(self.obtenido.keys())#los value represetan los y
        listaOrdenada.sort()#tomo las llaves que representan los x
        l = 0
        r = len(listaOrdenada) - 1
        k = (l+r)>>1#dividir entre dos
        theta = self.calculoTheta(listaOrdenada[l]-listaOrdenada[k],
                                    self.obtenido[listaOrdenada[l]] - self.obtenido[listaOrdenada[k]],
                                    listaOrdenada[r]-listaOrdenada[k],
                                    self.obtenido[listaOrdenada[r]] - self.obtenido[listaOrdenada[k]])

        flag = True
        while flag:

            flag = False
            midI = math.ceil((k+l)/2)#techo
            midD = math.floor((k+r)/2)
            thetaD = 4
            thetaI = 4
            orientation = 0

            if midI < k:
                thetaI = self.calculoTheta(listaOrdenada[l]-listaOrdenada[midI],
                                            self.obtenido[listaOrdenada[l]] - self.obtenido[listaOrdenada[midI]],
                                            listaOrdenada[k]-listaOrdenada[midI],
                                            self.obtenido[listaOrdenada[k]] - self.obtenido[listaOrdenada[midI]])

            if midD > k:
                thetaD = self.calculoTheta(listaOrdenada[k]-listaOrdenada[midD],
                                            self.obtenido[listaOrdenada[k]] - self.obtenido[listaOrdenada[midD]],
                                            listaOrdenada[r]-listaOrdenada[midD],
                                            self.obtenido[listaOrdenada[r]] - self.obtenido[listaOrdenada[midD]])
            #validar primero si los id son validos
            if (thetaD < theta) or (thetaI < theta):
                #tanteo las thetas xD
                print("posiciones")
                print(l)
                print(k)
                print(r)
                if thetaD < thetaI:
                    print("derecha")
                    print("mid", midD)
                    flag = True
                    theta = thetaD
                    l = k
                    k = midD
                    self.bestK = listaOrdenada[k]
                    orientation = 0
                else:
                    print("izquierda")
                    print("mid", midI)
                    flag = True
                    theta = thetaI
                    r = k
                    k = midI
                    self.bestK = listaOrdenada[k]
                    orientation = 1
                print("posiciones actualizadas")
                print(l)
                print(k)
                print(r)

        """if orientation:
            return listaOrdenada[k], listaOrdenada[r]
        else:
            return listaOrdenada[l], listaOrdenada[k]"""
        print(listaOrdenada)
        return listaOrdenada[l], listaOrdenada[k], listaOrdenada[r]



    def elbow(self):
        listaOrdenada = list(self.obtenido.keys())#los value represetan los y
        listaOrdenada.sort()#tomo las llaves que representan los x

        l = 0
        r = len(listaOrdenada) - 1
        k = (l+r)>>1#dividir entre dos
        self.bestK = k
        # En la posicion 0 esta el 'x' y en la posicion 1 esta el 'y'
        # calculamos el theta inicial
        #theta = calculoTheta(listaOrdenada[l][0]-listaOrdenada[k][0], listaOrdenada[l][1]-listaOrdenada[k][1],listaOrdenada[r][0]-listaOrdenada[k][0], listaOrdenada[r][1]-listaOrdenada[k][1])

        theta = self.calculoTheta(listaOrdenada[l]-listaOrdenada[k],
                                    self.obtenido[listaOrdenada[l]] - self.obtenido[listaOrdenada[k]],
                                    listaOrdenada[r]-listaOrdenada[k],
                                    self.obtenido[listaOrdenada[r]] - self.obtenido[listaOrdenada[k]])
        print("valor de thetha", theta)
        flag = True
        while(flag) :
            flag = False
            #mid = (k+r)>>1#piso
            mid = math.floor((k+r)/2)
            print("el valor de mid", mid)
            print("el valor de r", r)
            print("el valor de k", k)
            print("el valor de l", l)
            print(listaOrdenada)
            print(list(self.obtenido.items()))
            #auxmid = 0
            #k mid r
            # calculamos el theta temp por el lado derecho

            temp = self.calculoTheta(listaOrdenada[k]-listaOrdenada[mid],
                                        self.obtenido[listaOrdenada[k]] - self.obtenido[listaOrdenada[mid]],
                                        listaOrdenada[r]-listaOrdenada[mid],
                                        self.obtenido[listaOrdenada[r]] - self.obtenido[listaOrdenada[mid]])
            # Comprobamos si el theta temp es menor que el tetha actual
            if(theta > temp) :
                flag = True
                theta = temp
                l = k
                k = mid
                self.bestK = k
            mid = math.ceil((k+l)/2)#techo

            # calculamos el theta temp por el lado izquierdo
            #temp = calculoTheta(listaOrdenada[l][0]-listaOrdenada[mid][0], listaOrdenada[l][1]-listaOrdenada[mid][1],
                #listaOrdenada[k][0]-listaOrdenada[mid][0], listaOrdenada[k][1]-listaOrdenada[mid][1])

            temp = self.calculoTheta(listaOrdenada[l]-listaOrdenada[mid],
                                        self.obtenido[listaOrdenada[l]] - self.obtenido[listaOrdenada[mid]],
                                        listaOrdenada[k]-listaOrdenada[mid],
                                        self.obtenido[listaOrdenada[k]] - self.obtenido[listaOrdenada[mid]])
            # comprobamos si el theta es menor
            if(theta > temp) :
                flag = True
                theta = temp
                r = k
                k = mid
                self.bestK = k
        #l2,k5,r9
        return l,k,r
if __name__ == '__main__':

    cantPoints = int(sys.argv[1])
    print("cantidad de puntos:", cantPoints)
    b = Broker(cantPoints)
    b.run()
