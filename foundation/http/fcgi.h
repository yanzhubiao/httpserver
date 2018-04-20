#ifndef FCGI_H
#define FCGI_H

#include "httpget.h"
#include "httppost.h"

class FCgi
{
public:
    FCgi(){}
    ~FCgi(){}

public:
    void Handle();

private:
    HttpGet   httpGet_;
    HttpPost  httpPost_;

};



#endif