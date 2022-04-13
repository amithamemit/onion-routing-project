#pragma once
#include <iostream>
#include "DataStructures.hpp"
#include "Helper.hpp"
#include "json.hpp"
#include "RSA_Communication.hpp"

using nlohmann::json;

#define BYTES_LEN 4

class JsonPacketSerialization{
    public:
        static unsigned char* createLoginRequest(LoginRequest, RSA_Communication*);
        static unsigned char* createSignupRequest(SignupRequest, RSA_Communication*);
        static unsigned char* createPathRequest(PathRequest, RSA_Communication*);
        static unsigned char* createAvaliableLastRelaysRequest(RSA_Communication*);
        static unsigned char* createNextNodeResult(nextNode nextRelay, RSA_Communication*);
        static unsigned char* getJsonBufferRequest(json, int code);

        
};