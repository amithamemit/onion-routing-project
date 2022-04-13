import json

class JsonPacketSerialization:
    def __init__(self):
        pass
    
    """
    this function creates a json buffer fron user input
    input: code of the login messsage and the user id
    output: json buffer
    """
    @staticmethod
    def get_login_result(code, user_id):
        response_json = {
            "code": code,
            "user_id": user_id
        }
        return JsonPacketSerialization.__createResponseBuffer(response_json)

    """
    this function creates a json buffer fron user input
    input: code of the signup messsage 
    output: json buffer
    """
    @staticmethod 
    def get_signup_result(code):
        response_json = {
            "code": code
        }
        return JsonPacketSerialization.__createResponseBuffer(response_json)
    
    """
    Method that create a buffer by a json object.
    The method create a buffer with the message length and the json string in bytes format.
    Input: json object.
    Output: bytes buffer.
    """
    @staticmethod
    def __createResponseBuffer(response_json):
        # convert the json object to string
        json_string = json.dumps(response_json).replace("'", '"')

        # getting the length of the json string
        json_length = len(json_string)
        
        # getting the json string length in byte format
        json_length_bytes_string = JsonPacketSerialization.__int_to_4_bytes_string(json_length)

        # create a buffer in byte format that have the message length and the json string
        buffer = json_length_bytes_string + json_string
        return buffer


    """
    Method that convert int to 4 bytes array.
    Input: number to convert.
    Output: 4 byte array.
    """
    @staticmethod
    def __int_to_4_bytes_string(num):
        result_bytes = []
        result = ""

        # create array of bytes
        for i in range(0, 4):
            result_bytes.append(num >> (i * 8) & 0xff)
        result_bytes.reverse()
        
        # convert the bytes array to char array
        for i in range(0, 4):
            result += chr(result_bytes[i])
        
        return result
