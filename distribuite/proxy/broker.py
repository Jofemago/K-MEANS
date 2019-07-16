import zmq


class Broker:

    context = zmq.Context()
    router = context.socket(zmq.ROUTER)
    poller = zmq.Poller()
    p = 0


    def __init__(self):

        self.op = {"WorkDone":self.serverResponse, "IFREE":self.serverFree}
        self.router.bind("tcp://*:5000")
        #self.poller.register(self.router, zmq.POLLIN)
        #print(self.poller.poll())

        self.bestK = None
        self.k = {}

        #print(self.router == zmq.POLLIN)


    def serverResponse(self, data):

        print("el servidor acabo de evaluar un k")
        print(data[0])

    def serverFree(self, data):

        print("un servidor esta libre se le va asignar un k para que trabaje")
        print(data[0])
        #k = 20
        msj = [data[0],str(self.p).encode(), b"probando"]
        self.router.send_multipart(msj)
        self.p += 1




    def run(self):
        print("Running the server Broker....")
        while True:


            print("revisando si un server ha solicitado")

            if self.router.poll(100):
                print("-----------un servidor ha hecho una solicitud--------------")
                msj = self.router.recv_multipart()
                #print("lo que me envia el server:", msj[1])
                self.op[msj[1].decode()](msj)

if __name__ == '__main__':


    b = Broker()
    b.run()
