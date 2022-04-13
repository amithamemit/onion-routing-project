#include "CreatePathHandler.hpp"

CreatePathHandler::CreatePathHandler(LoggedUser loggedUser, RequestHandlerFactory* requestHandlerFactory, std::vector<RelayServer>* relayServers){
    this->m_loggedUser = loggedUser;
    this->m_requestHandlerFactory = requestHandlerFactory;
    this->m_relayServers = relayServers;
}

CreatePathHandler::~CreatePathHandler(){

}

bool CreatePathHandler::isRequestRelevant(int requestCode) const{
    return true;
}

RequestResult CreatePathHandler::handleRequest(RequestInfo requestInfo) const{
    RequestResult requestResult = {};


    return requestResult;
}


/*
Method that get the best n relays for the new path of the user.
The n relays chosen by algorithem which check which relays have the fewers connected users.
The first relay is need to be the guard node, so the method check in the database which relay is the guard node of the current user.
Input: n which present the amount of relays that the user want.
Output: Relay server list which present the path of the user.
*/
std::vector<RelayServer> CreatePathHandler::CreatePath(int n, int userId){
    std::vector<RelayServer> path = std::vector<RelayServer>(); // vector of the new path for the current user
    // first, need to add the guard node for the path.
    RelayServer guardNode = this->getGuardNode(userId);
    path.push_back(guardNode);

    // loop that add relays for the path
    for(int i = 0; i < n; i++){
        // getting the next relay in the realys vector that is sort by the number of connected users.
        this->m_relayServersMutex.lock();
        RelayServer nextRelay = (*this->m_relayServers)[i];
        this->m_relayServersMutex.unlock();

        path.push_back(nextRelay);
    }
    
    return path;
}


RelayServer CreatePathHandler::getGuardNode(int userId){
    RelayServer relayServer = {};
    //need to ask python DB server for guard node

    return relayServer;
}

void CreatePathHandler::refreshRelayServers()
{
    
}
