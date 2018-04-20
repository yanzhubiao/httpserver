#include "httpresponse.h"
#include "log/log.h"

#include <cstring>

class HttpResponse
{
public:
    static HttpResponse& instance();
public:
    void SetStreamOut(FCGX_Stream* out) {out_ = out;}
    FCGX_Stream* GetStreamOut() {return out_;}
private:
    FCGX_Stream* out_;
};

HttpResponse& HttpResponse::instance()
{
    static HttpResponse instance;
    return instance;
}

void FCGX_SetStreamOut(FCGX_Stream* out)
{
    HttpResponse::instance().SetStreamOut(out);
}

void FCGX_Response(const char *data, int contentLength)
{
    if (!data)
        return;

    auto out = HttpResponse::instance().GetStreamOut();
    if(!out)
        return;

    FCGX_FPrintF(out, "Content-Length: %d\r\n\r\n", contentLength);
    if (FCGX_PutStr(data, contentLength, out) != contentLength)
    {
        LOGGING(LOG_ERROR, "FCGX_PutStr() error !");
        return;
    }
}

void FCGX_ResponseNull()
{
    static const char* defaultString = "null";
    FCGX_Response(defaultString, strlen(defaultString));
}