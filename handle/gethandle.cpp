#include "gethandle.h"
#include "log/log.h"
#include "http/httpget.h"

#include "http/httpresponse.h"

#include <cstring>
#include <cstdio>


inline static void GetHandleTest(ParamVector& param)
{
    // LOGGING(LOG_INFO, "GetHandleTest");

    char outBuffer[256] = { 0 };
    std::snprintf(outBuffer, sizeof(outBuffer), "{\"uid\":\"123456\"}");
    FCGX_Response(outBuffer, strlen(outBuffer));
}

void HttpGetHandleRegister()
{
    HttpGetDispatcherInstance().Register("/get.cgi", GetHandleTest);
}
