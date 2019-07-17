import zmq


class Broker:

    context = zmq.Context()
    router = context.socket(zmq.ROUTER)
    poller = zmq.Poller()
    p = 0


    def __init__(self):

        self.op = {"WorkDone":self.serverResponse, "serverFREE":self.serverFree}
        self.router.bind("tcp://*:5000")
        #self.poller.register(self.router, zmq.POLLIN)
        #print(self.poller.poll())

        self.bestK = None
        self.k = {}

        #print(self.router == zmq.POLLIN)


    def serverResponse(self, data):

        print("el servidor acabo de evaluar un k")
        print(data[1])
        print(data)
        #guardar el resultado en una estructura de datos, evaluar estructura de datos

    def serverFree(self, data):

        print("un servidor esta libre se le va asignar un k para que trabaje")
        print(data[1])

        #validar si no tengo que conseguir mas k
            #enviar un mensaje diciendole al sever que termino para que no mande mas trabajo
        #sacar k que necesito pedir de alguna estructura de datos

        if(self.p < 1000):
            msj = [data[0], b'KMEANS', str(self.p).encode()]#, b"probando"]
            self.router.send_multipart(msj)
            self.p += 1
        else:
            print("ha finalizado el proceso no puedo enviar mas")
            msj = [data[0], b'Finish', str(self.p).encode()]#, b"probando"]
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

if __name__ == '__main__':


    b = Broker()
    b.run()
