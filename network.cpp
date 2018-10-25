#include "network.h"

Network::Network()
{

}

Network::~Network(){
    if (_connect){
        delete _connect;
    }
}
