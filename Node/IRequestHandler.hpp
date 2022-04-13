#pragma once
#include <iostream>
#include "DataStructures.hpp"

struct RequestResult;
struct RequestInfo;

class IRequestHandler{ //virtual class that other handlers inherit from
    public:
        virtual bool isRequestRelevant(int) const = 0;
        virtual RequestResult handleRequest(RequestInfo) const = 0;
};