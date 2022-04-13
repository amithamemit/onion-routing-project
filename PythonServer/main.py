from Server import Server
from Database import Database
def main():
    server = Server("127.0.0.1", 5000) #creating the server
    server.run()


if __name__ == "__main__":
    main()