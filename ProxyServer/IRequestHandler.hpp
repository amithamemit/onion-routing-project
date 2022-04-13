#pragma once
#include <iostream>
#include "DataStructures.hpp"

struct RequestResult;
struct RequestInfo;

class IRequestHandler{
    public:
        virtual bool isRequestRelevant(int) const = 0;
        virtual RequestResult handleRequest(RequestInfo) const = 0;
};