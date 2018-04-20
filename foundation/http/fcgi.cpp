#include "fcgi.h"

#include <fcgi_stdio.h>
#include <fcgiapp.h>

#include <cstring>
#include <cstdlib>

#include "log/log.h"
#include "httpresponse.h"

void FCgi::Handle()
{
    FCGX_Stream *in, *out, *err;
    FCGX_ParamArray envp;

    while (FCGX_Accept(&in, &out, &err, &envp) >= 0)
    {
        FCGX_SetStreamOut(out);
        // LOGGING(LOG_INFO, "FCGX_Accept(): accept data");

        auto requestMethod = FCGX_GetParam("REQUEST_METHOD", envp);
        // LOGGING(LOG_INFO, "HTTP method = [%s]", request_method);

        //GET
        if(strcmp(requestMethod, "GET") == 0)
        {
            httpGet_.Handle(envp);
        }

        // POST
        else if(strcmp(requestMethod, "POST") == 0)
        {
            httpPost_.Handle(envp, in);
        }

        else
        {
            continue;
        }
    }

    FCGX_Finish();

}