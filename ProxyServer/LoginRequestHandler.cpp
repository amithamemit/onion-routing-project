#include "LoginRequestHandler.hpp"

LoginRequestHandler::LoginRequestHandler(LoginManager* LoginManager, RequestHandlerFactory* requestHandlerFactory, RSA_Communication* rsa){
    this->m_loginManager = LoginManager;
    this->m_requestHandlerFactory = requestHandlerFactory;
    this->m_rsa = rsa;
}

LoginRequestHandler::~LoginRequestHandler(){

}

bool LoginRequestHandler::isRequestRelevant(int code) const{
    return code == loginCode || code == signupCode;
}

/*
method that handle in user request.
this method can handle:
1. Login request
2. Signup request
if the user have access to the proxy server, the method create to the user new handler to the next request
Input: request info.
Output: request result.
*/
RequestResult LoginRequestHandler::handleRequest(RequestInfo requestInfo) const{
    RequestResult requestResult = {};
    int requestCode = (int)requestInfo.buffer[0];
    
    if(this->isRequestRelevant(requestCode)){
        switch (requestCode){
            case loginCode:{
                // getting login request info
                LoginInfo loginInfo = JsonPacketDeserialization::getLoginInfo(requestInfo.buffer, this->m_rsa);
                // getting login result after login proccess in the database server
                LoginResult loginResult = this->m_loginManager->login(loginInfo.username, loginInfo.password);
                // handle the login request result
                this->handleLoginResultCode(loginResult.code);

                requestResult.buffer = JsonPacketSerialization::createLoginResult(loginResult, this->m_rsa);
                requestResult.newHandler = (IRequestHandler*)this;
                break;
            }

            case signupCode:{
                SignupInfo signupInfo = JsonPacketDeserialization::getSignupInfo(requestInfo.buffer, this->m_rsa);
                SignupResult signupResult = this->m_loginManager->signup(signupInfo.username, signupInfo.password);
                this->handleSignupResultCode(signupResult.code);

                requestResult.buffer = JsonPacketSerialization::createSignupResult(signupResult, this->m_rsa);
                requestResult.newHandler = (IRequestHandler*)this;
                break;
            }
            
            default:
                break;
        }
    }

    return requestResult;
}


void LoginRequestHandler::handleLoginResultCode(int code) const{
    switch (code){
        case accessApprovedCode:{
            std::cout << Helper::getCurrentTime() << "user logged in" << std::endl;
            break;
        }
        case wrongPasswordCode:{
            std::cout << Helper::getCurrentTime() << "wrong password" << std::endl;
            break;
        }
        case wrongUsernameCode:{
            std::cout << Helper::getCurrentTime() << "wrong username" << std::endl;
            break;
        }
        case userAlreadyLoggedInCode:{
            std::cout << Helper::getCurrentTime() << "user already logged in" << std::endl;
            break;
        }
        
        default:{
            std::cout << Helper::getCurrentTime() << "error in login proccess. error code: " << code << std::endl;
            break;
        }
    }
}

void LoginRequestHandler::handleSignupResultCode(int code) const{
    switch (code){
        case signupSucceed:{
            std::cout << Helper::getCurrentTime() << "user signed up succeed" << std::endl;
            break;
        }
        case usernameExists:{
            std::cout << "username exist" << std::endl;
            break;
        }
        default:{
            break;
        }
    }
}