#ifndef HTTPGET_H
#define HTTPGET_H

#include <fcgi_stdio.h>
#include <fcgiapp.h>

#include <vector>

#include "dispatcher.h"

struct KV
{
    KV()
    {
        key = nullptr;
        value = nullptr;
    }
    char* key;
    char* value;
};

using ParamVector = std::vector<KV>;

class HttpGet 
{
public:
    HttpGet() = default;
    ~HttpGet() = default;

public:
    void Handle(FCGX_ParamArray& envp);

private:
    ParamVector queryParams_;

};

using HttpGetDispatcher = Dispatcher<ParamVector>;
HttpGetDispatcher& HttpGetDispatcherInstance();

#endif
