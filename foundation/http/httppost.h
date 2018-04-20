#ifndef HTTPPOST_H
#define HTTPPOST_H

#include <fcgi_stdio.h>
#include <fcgiapp.h>

#include "dispatcher.h"

class HttpPost
{
public:
    HttpPost() = default;
    ~HttpPost() = default;

public:
    void Handle(FCGX_ParamArray& envp, FCGX_Stream* in);

};

using HttpPostDispatcher = Dispatcher<char*>;
HttpPostDispatcher& HttpPostDispatcherInstance();

#endif