from tkinter import *
from functools import partial
import socket
import json
from JsonPacketSerialization import JsonPacketSerialization

import Crypto
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_v1_5
from Crypto.Cipher import PKCS1_OAEP

key = RSA.generate(2048)

private_key = key.exportKey('PEM').decode('utf-8')
public_key = key.publickey().exportKey('PEM').decode('utf-8')

private_key_file = open('private.pem', "w")
private_key_file.write(private_key)
private_key_file.close()

public_key_file = open('public.pem', "w")
public_key_file.write(public_key)
public_key_file.close()

HOST = "127.0.0.1"
PORT = 8080
	

def validateLogin(username, password):
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.connect((HOST, PORT))
		# first need to transfer keys
		# send client public key to the server
		request_json = {'publicKey': public_key}
		# create a buffer to send to the proxy. the buffer have the public key of the user in json format
		buffer = JsonPacketSerialization.get_buffer_from_json(request_json, 0)
		# send the buffer to the server
		s.sendall(buffer)
		# recieve the response from the proxy server. the response have the proxy server public key
		data = s.recv(1024)

		# getting the json message from the server
		data_len = int.from_bytes(data[0:4], byteorder="big")
		msg = ""
		for i in range(data_len):
			msg += chr(data[i+4])
		json_response = json.loads(msg)

		
		print('Proxy public key:\n', json_response['publicKey'])

		# login to proxy
		request_json = {"username": username.get(), "password": password.get()}
		buffer = JsonPacketSerialization.get_buffer_from_json(request_json, 0)

		proxy_public_key = RSA.importKey(json_response['publicKey'])
		cipher = PKCS1_v1_5.new(proxy_public_key)
		buffer = cipher.encrypt(buffer)

		s.sendall(buffer)
		data = s.recv(1024)

		my_private_key = RSA.importKey(private_key)
		rsa_private_key = PKCS1_OAEP.new(my_private_key)
		data = rsa_private_key.decrypt(data)

		data_len = int.from_bytes(data[0:4], byteorder="big")
		msg = ""
		for i in range(data_len):
			msg += chr(data[i+4])
		json_response = json.loads(msg)
		

	#print("username entered :", username.get())
	#print("password entered :", password.get())
	return

#window
tkWindow = Tk()
tkWindow.geometry('400x150')
tkWindow.title('Tkinter Login Form')

#username label and text entry box
usernameLabel = Label(tkWindow, text="Username").grid(row=0, column=0)
username = StringVar()
usernameEntry = Entry(tkWindow, textvariable=username).grid(row=0, column=1)

#password label and password entry box
passwordLabel = Label(tkWindow,text="Password").grid(row=1, column=0)
password = StringVar()
passwordEntry = Entry(tkWindow, textvariable=password, show='*').grid(row=1, column=1)  

validateLogin = partial(validateLogin, username, password)

#login button
loginButton = Button(tkWindow, text="Login", command=validateLogin).grid(row=4, column=0)  

tkWindow.mainloop()