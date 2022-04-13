import sqlite3
import os
import socket
from Codes import Codes

PROXY_IP_ADDRESS = "172.0.0.1"
PROXY_PORT = 8080

class Database:
    def __init__(self):
        # check if there is database. if not create a new one.
        if not os.path.isfile('{}/TOR-DB.db'.format(os.getcwd())):
            print("Creating database...")
            self.__create_empty_database()

        print("Connecting to database...")
        self.__database_connection = sqlite3.connect('{}/TOR-DB.db'.format(os.getcwd()))
        self.__codes = Codes()

    def __dell__(self):
        # close connection to sqlite database.
        self.__database_connection.close()
        pass


    """
    Method that check if user can login.
    Input: username and password.
    Output: code after the login proccess, and user id (if the user exists).
    """
    def login(self, username, password):
        result = None
        # sql query to check if user exist
        query = "SELECT user_id FROM users WHERE username = '{}' AND password = '{}'".format(username, password)
        try:
            cursor = self.__database_connection.cursor()
            # run query and getting the results
            cursor.execute(query)
            rows = cursor.fetchall()
            # if there is no match for the username and the password
            if not rows:
                result = (0, self.__codes.wrong_username)
            # if the username and password match
            else:
                user_id = rows[0][0]
                result = (user_id, self.__codes.login_succeed)
        except sqlite3.Error as e:
            result = (0, self.__codes.unexpected_error)
            

        # close cursor
        cursor.close()
        return result


    """
    Method that signup new user.
    Input: username and password.
    Output: code after the signup proccess
    """
    def signup(self, username, password):
        result = None
        
        # sql query to signup new user
        query = "INSERT INTO users (username, password) VALUES ('{}', '{}')".format(username, password)
        try:
            cursor = self.__database_connection.cursor()
            cursor.execute(query)
            result = self.__codes.signup_succeed
        except sqlite3.Error as e:
            result = self.__codes.username_exists
            
        # save database changed to the database file
        self.__database_connection.commit()
        cursor.close()
        return result

    
    """
    Method that return the user guard node by user id.
    Input: user id.
    Output: the guard node of the user.
    """
    def get_guard_node(self, user_id):
        ip_address, port = 0, 0
        # sql query to get the guard node

        # ****** need to change the query ****
        query = "SELECT ip_address, port FROM nodes WHERE node_id = (SELECT node_id FROM user_guard_node WHERE user_id = {})".format(user_id) 
        try:
            cursor = self.__database_connection.cursor()
            cursor.execute(query)
            rows = cursor.fetchall()

            ip_address, port = rows[0][0], rows[0][1]

        except sqlite3.Error as e:
            print("Can't find guard node associated with user.")

        cursor.close()
        return ip_address, port

    
    """
    Method that set guard node of user.
    Input: user id, node id.
    Output: if the guard node have changed.
    """
    def set_guard_node(self, user_id, node_id):
        result = False

        # sql query to set the guard node
        query = "UPDATE user_guard_node SET node_id = {} WHERE user_id = {}" .format(node_id, user_id)
        try:
            cursor = self.__database_connection.cursor()
            cursor.execute(query)
            result = True
        except sqlite3.Error as e:
            print("Couldn't update guard node.")
            result = False
            
        # save database changed to the database file
        self.__database_connection.commit()
        cursor.close()
        return result
        

    """
    Method to check if ip of client is allowed.
    Input: ip address.
    Output: if the address is allowed.
    """
    def check_if_address_allowed(self, ip_address):
        try:
            socket.inet_pton(socket.AF_INET, ip_address)
        except AttributeError:  # no inet_pton here, sorry
            try:
                socket.inet_aton(ip_address)
            except socket.error:
                return False
            return ip_address.count('.') == 3
        except socket.error:  # not a valid address
            return False
        return True

    
    """
    Method that add new ip address to block.
    Input: ip address.
    Output: if the address is added to the "black list".
    """
    def add_new_address_to_block(self, ip_address):
        result = False

        # sql query to add new ip address to block
        query = "INSERT INTO black_list (ip_address) VALUES('{}')".format(ip_address)
        try:
            cursor = self.__database_connection.cursor()
            cursor.execute(query)
            result = True
        except sqlite3.Error as e:
            print("Couldn't add new addres to block.")
            result = False
            
        # save database changed to the database file
        self.__database_connection.commit()
        cursor.close()
        return result


    """
    Method to check if ip address is not in the black list.
    input: ip address.
    output: if the ip is valid to access or not.
    """
    def check_if_ip_adress_not_blocked(self, user_id):
        return True

    """
    Method to get ip address and port of the proxy server, will be used by relays
    input: None
    output: IP Address, Port
    """
    def getProxyServer(self):
        return PROXY_IP_ADDRESS, PROXY_PORT

    """
    Method to check if relay exists in the database
    input: IP address and port of relay
    output: if exists (bool)
    """
    def doesRelayExist(self, ip_address, port):
        # sql query to get node with the same ip address and port
        query = "SELECT * FROM nodes WHERE ip_address = '{}' AND port = {}".format(ip_address, port) 
        try:
            cursor = self.__database_connection.cursor()
            cursor.execute(query)
            rows = cursor.fetchall()
            
            if rows: #if list is empty in python it is considered false
                cursor.close()
                return True

        except sqlite3.Error as e:
            print("Can't find node.")

        cursor.close()
        return False


    """
    Method to add a new node to the network.
    Input: node ip address, and node port.
    Output: if the new node is added.
    """
    def add_new_node(self, ip_address, port):
        result = False

        # sql query to add new node
        query = "INSERT INTO nodes (ip_address, port) VALUES('{}', {})".format(ip_address, port)
        try:
            cursor = self.__database_connection.cursor()
            cursor.execute(query)
            result = True
        except sqlite3.Error as e:
            print("Couldn't add new node.")
            result = False
            
        # save database changed to the database file
        self.__database_connection.commit()
        cursor.close()
        return result

    
    """
    Method that remove node from the network.
    Input: node ip adress, and node port.
    Output: if the node is removed.
    """
    def remove_node(self, ip_adress, port):
        result = False

        # sql query to remove node
        query = "DELETE FROM node WHERE ip_address = '{}' AND port = {}".format(ip_adress, port)
        try:
            cursor = self.__database_connection.cursor()
            cursor.execute(query)
            result = True
        except sqlite3.Error as e:
            print("Couldn't remove node.")
            result = False
            
        # save database changed to the database file
        self.__database_connection.commit()
        cursor.close()
        return result


    """
    Method to get all the nodes in the network.
    Input: None.
    Output: list of nodes in the next fomat: (ip_address, port).
    """
    def get_all_nodes(self):
        return []


    """
    Method that get node location.
    Input: node ip address, and node port.
    Output: the location of the node.
    """
    def get_node_location(self, ip_address, port):
        location = ""
        # sql query to get node location
        query = "SELECT location FROM nodes WHERE ip_address = '{}' AND port = {}".format(ip_address, port) 
        try:
            cursor = self.__database_connection.cursor()
            cursor.execute(query)
            rows = cursor.fetchall()
            
            location = rows[0][0]

        except sqlite3.Error as e:
            print("Can't find node location.")

        cursor.close()
        return location

    
    """
    Method that return the amount of time in days that the user don't change the guard node.
    Input: user id.
    Output: amount of times that the user don't change the guard node.
    """
    def get_guard_node_time(self, user_id):
        return 0


    """
    Method that create all the database.
    Input: None.
    Output: None.    
    """
    def __create_empty_database(self):
        # connect to the database
        connection = sqlite3.connect('{}/TOR-DB.db'.format(os.getcwd()))
        # getting cursor to the database
        cursor = connection.cursor()

        # create all database tables
        querys = []
        
        # create users table
        querys.append(""" 
                CREATE TABLE IF NOT EXISTS
                users(user_id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, username TEXT NOT NULL UNIQUE, password TEXT NOT NULL)  
                """)
        # create nodes table
        querys.append("""
                CREATE TABLE IF NOT EXISTS
                nodes(node_id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, ip_address TEXT NOT NULL, port INTEGER NOT NULL, location TEXT NOT NULL, public_key TEXT NOT NULL)
                """)
        # create guard node table
        querys.append("""
                CREATE TABLE IF NOT EXISTS
                user_guard_node(node_id INTEGER NOT NULL, user_id INTEGER NUT NULL UNIQUE)
                """)
        querys.append("""
                CREATE TABLE IF NOT EXISTS
                black_list(ip_address TEXT NOT NULL UNIQUE);
                """)
        
        for query in querys:
            cursor.execute(query)
        
        connection.commit()

        #close connections
        cursor.close()
        connection.close()

