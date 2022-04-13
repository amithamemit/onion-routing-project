import json
from sys import byteorder

class JsonPacketSerialization:
    """
    Method that get number and return the number in 4 bytes array.
    Input: number.
    Output: 4 bytes array.
    """
    @staticmethod
    def int_to_4_bytes_array(num):
        return bytearray(int.to_bytes(num, 4, byteorder='big'))

    """
    Method that get a request in json format and return buffer in bytes to send to the server.
    Input: request in json format.
    Output: buffer to send to the server
    """
    @staticmethod
    def get_buffer_from_json(request_json, code):
        # convert the json object to string
        json_string = json.dumps(request_json).replace("'", '"')
        # getting the length of the json string
        json_length = len(json_string)
        bytes_list = JsonPacketSerialization.int_to_4_bytes_array(json_length)
        buffer = bytearray()

        buffer.append(code)
        # adding the bytes
        for byte in bytes_list:
            buffer.append(byte)
        # adding the json string
        for ch in json_string:
            buffer.append(ord(ch))
        
        buffer = bytes(buffer)

        return buffer

        