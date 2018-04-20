#include "posthandle.h"
#include "http/httppost.h"
#include "log/log.h"
#include "http/httpresponse.h"

#include <cstring>

inline static void PostHandleTest(const char* jsonStr)
{
    // LOGGING(LOG_INFO, "post info  %s", jsonStr);

    char outBuffer[256] = { 0 };
    std::snprintf(outBuffer, sizeof(outBuffer), "{\"uid\":\"1234567890\"}");
    FCGX_Response(outBuffer, strlen(outBuffer));
}

void HttpPostHandleRegister()
{
    HttpPostDispatcherInstance().Register("/post.cgi", PostHandleTest);
}