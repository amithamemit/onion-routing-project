# EmekIsrael-1402-OnionRouting

this project is an implementation of the tor network, which is a project created by the US Navy in order to make the messages sent by the network more secure.
the majority of the project is written in C++, the database server is written in Python.
this network allows the user to send encrypted messages and communicate with the destination without showing his ip adrress.
the network contains 4 important entities:
1 - The proxy server:
The proxy is required to communicate with the client, give him the route and organize the network.
2 - The relays:
The relays are responsible of passing the messages and encrypting/ decrypting them by the direction they are going,
if they go from the client to the destination they are decrypted and on the way back they are encrypted.
3 - The database server:
The database server contains the information about the users, relays and the public key of the relays.
4 - The client:
The client gets the route from the proxy and then builds the route with the relays, then he encrypts the message by layers of encryption and sends it to the destination, when the message returns from the relays encrypted with layers the client decrypts it.
Using this network a person who listens to the netwrok's traffic will never be able to see the client's adress and the message sent (without encryption) at the same time.
(back to top)
Built With
C++
Python
Linux
SQL
RSA
(back to top)
Getting Started
To get a local copy up and running follow these simple example steps.
Installation

git clone https://gitlab.com/Aviad_G/emekisrael-1402-onionrouting.git


Run the project

run the database server by going to the terminal of the directory /PythonServer and type "python3 main.py"

then launch the proxy server by going to the /ProxyServer directory and type "./main"

the create at least 3 local relays running by going to the /Node directory and type ./node followed by a port number.
for example to create a relay with the port number of 2057 you need to type "./node 2057"

then run the example server in the /DemoServer directory and type ./server

at the end launch the client in the /ProxyClient directory and type in the terminal "./Client

follow the instructions specified in the client program 


Important to note

Hopefully in the future the relays, proxy, database server and the demo server will run on rented hardware and the 
only file needed will be the client.


(back to top)
Usage

this projects allows to send encrypted messages which will be encrypted and not readble at client side and
decrypted at server end also in the end the message will be sent to the server ny a relay with different ip adrress
from the user, thus preventing strangers to know what messages the client sends and where they are headed.


(back to top)
Features

Thread pool
Proxy organizes route by amount of users connected to relays and user request
Database server
    SSH to prevent unknown entities accessing the database.


See the open issues for a full list of proposed features (and known issues).
(back to top)
License
Distributed under the MIT License. See LICENSE.txt for more information.
(back to top)
Contact

email - amitharit55@gmail.com
gitlab - @amithamemit2

email - aviadgolde@gmail.com
gitlab - @Aviad_G

Project Link: https://gitlab.com/Aviad_G/emekisrael-1402-onionrouting
(back to top)
Acknowledgments

The official Tor project - https://www.torproject.org/


(back to top)
