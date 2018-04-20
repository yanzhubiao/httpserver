#include "httpget.h"
#include "urldecode.h"
#include "log/log.h"

#include <cstdlib>
#include <cstring>

#include "httpresponse.h"

inline static void CreateKV(char* queryString, char*& key, char*& value)
{
    key = queryString;

    const char seg = '=';
    char* pos = std::strchr(queryString, seg);

    if(pos == nullptr)
    {
        return;
    }
    *pos = '\0';
    value = pos+1;
}

inline static void CreateQuery(char* queryString, ParamVector& params)
{
    params.clear();
    const char* seg = "&";
    char* subStr = std::strtok (queryString, seg);
    while (subStr != nullptr)
    {
        KV kv;
        CreateKV(subStr, kv.key, kv.value);
        params.push_back(kv);
        subStr = std::strtok(nullptr, seg);
    }
}

// http://localhost/a.cgi?name=yzb  ==> /a.cgi 
// or http://localhost/a.cgi ==> /a.cgi
// or http://localhost/abc ==> nullptr
inline static char* GetRequestUri(FCGX_ParamArray& envp)
{
    static const char* endStr = ".cgi";
    static int lenth = strlen(endStr);

	auto requestUri = FCGX_GetParam("REQUEST_URI", envp);
    auto firstpoint = strstr(requestUri, endStr);
    if (!firstpoint)
        return nullptr;

    *(firstpoint + lenth) = '\0';
    return requestUri;
}

void HttpGet::Handle(FCGX_ParamArray& envp)
{
    auto requestUri = GetRequestUri(envp);
    if(!requestUri)
    {
        FCGX_ResponseNull();
        return;
    }

    auto queryString = FCGX_GetParam("QUERY_STRING", envp);
    if(queryString == nullptr)
        return;
    // LOGGING(LOG_INFO, "QUERY_STRING = [%s]", queryString );

    auto pDecodeQueryString = urlDecode(queryString);
    if(pDecodeQueryString == nullptr)
        return;

    // LOGGING(LOG_INFO, "After Urldecode DECODE_QUERY_STRING = [%s]", pDecodeQueryString);
    
    CreateQuery(pDecodeQueryString, queryParams_);

    auto ret = HttpGetDispatcherInstance().Dispatch(requestUri, queryParams_);
    if(!ret)
        FCGX_ResponseNull();

    if(pDecodeQueryString != nullptr)
        free(pDecodeQueryString);
}

HttpGetDispatcher& HttpGetDispatcherInstance()
{
    static HttpGetDispatcher instance;
    return instance;
}
