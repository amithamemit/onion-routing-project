#pragma once
#include <iostream>
#include "IRequestHandler.hpp"
#include "LoginManager.hpp"
#include "RequestHandlerFactory.hpp"
#include "JsonPacketDeserialization.hpp"
#include "JsonPacketSerialization.hpp"
#include "RSA_Communication.hpp"

class RequestHandlerFactory;
class LoginManager;

class LoginRequestHandler : public IRequestHandler{
    public:
        LoginRequestHandler(LoginManager*, RequestHandlerFactory*, RSA_Communication*);
        ~LoginRequestHandler();

        bool isRequestRelevant(int) const;
        RequestResult handleRequest(RequestInfo) const;

    private:
        LoginManager* m_loginManager;
        RequestHandlerFactory* m_requestHandlerFactory;
        RSA_Communication* m_rsa;

        void handleLoginResultCode(int) const;
        void handleSignupResultCode(int) const;

};