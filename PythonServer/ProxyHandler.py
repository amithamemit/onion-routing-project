from JsonPacketSerialization import JsonPacketSerialization
from Codes import Codes
import json

class ProxyHandler: #handler to speak with proxy server
    def __init__(self, datbase):
        self.__database = datbase
        self.__codes = Codes()

    def handle_request(self, request_json_string_with_code):
        # getting request code
        request_code = ord(request_json_string_with_code[0])
        # remove the code from the request, to get the json string.
        request_json_string = request_json_string_with_code[1:]
        # convert the json string to json object
        request_json = json.loads(request_json_string)
        # create a buffer variable that have the client response
        buffer = None
        # handle the request
        if request_code == self.__codes.login_request:
            user_id, login_code = self.__database.login(request_json["username"], request_json["password"])
            buffer = JsonPacketSerialization.get_login_result(login_code, user_id)
        elif request_code == self.__codes.signup_request:
            signup_code = self.__database.signup(request_json["username"], request_json["password"])
            buffer = JsonPacketSerialization.get_signup_result(signup_code)
        else:
            pass
        return buffer
        
        