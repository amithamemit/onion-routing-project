import socket
from Database import Database
from ProxyHandler import ProxyHandler

class Server:
    def __init__(self, host, port):
        self.__host = host
        self.__port = port
        self.__server_socket = socket.socket()
        self.__server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.__database_connection = Database()
        self.__connections = {}

    def __del__(self):
        print("Closing server socket...")
        self.__server_socket.close()

    def run(self):
        self.__bindAnsListen()

    def shutdown(self):
        del self

    """
    this function makes the server wait and listen for messages
    """
    def __bindAnsListen(self):
        try:
            self.__server_socket.bind((self.__host, self.__port))  # bind host address and port together

            # configure how many client the server can listen simultaneously
            self.__server_socket.listen(2)
        except Exception as e:
            print("Can't run the server")
            print("Error: {}".format(e))
            return;
        
        print("Listen on port {}...".format(self.__port))

        # waiting for new connections
        while (True):
            # accept new connection
            conn, address = self.__server_socket.accept()
            self.__connections[conn] = ProxyHandler(self.__database_connection)
            # handle new client
            self.__handle_new_client(conn, address)

    """
    this function is called when a new client connects and handles the client requests
    input: connection with client and client address
    output: none
    """
    def __handle_new_client(self, conn, address):
        print("Connection from: {}".format(str(address)))
        
        # loop that get requests from client and handle them.
        while True:
            try:
                # getting message from client
                data = str(conn.recv(1024).decode())
                if not data:
                    # if data is not received break
                    break

                # handle client request and get response
                response_buffer = self.__connections[conn].handle_request(data)

                # send data to the client
                conn.send(response_buffer.encode('utf-8'))
            
            except Exception as e:
                print(e)
                print("Error with client connection")
                break

        print("closing {} socket...".format(str(address)))
        conn.close()  # close the connection
