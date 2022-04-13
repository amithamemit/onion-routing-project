#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include "IRequestHandler.hpp"
#include "LoginManager.hpp"
#include "RequestHandlerFactory.hpp"
#include "JsonPacketDeserialization.hpp"
#include "JsonPacketSerialization.hpp"

class RequestHandlerFactory;

class CreatePathHandler: public IRequestHandler{
    public:
        CreatePathHandler(LoggedUser, RequestHandlerFactory*, std::vector<RelayServer>*);
        ~CreatePathHandler();
        bool isRequestRelevant(int) const;
        RequestResult handleRequest(RequestInfo) const;
    private:
        std::vector<RelayServer> CreatePath(int, int);
        RelayServer getGuardNode(int);
        void refreshRelayServers();

        std::vector<RelayServer>* m_relayServers;
        LoggedUser m_loggedUser;
        std::mutex m_relayServersMutex;
        RequestHandlerFactory* m_requestHandlerFactory;
};