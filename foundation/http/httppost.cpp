#include "httppost.h"
#include "log/log.h"
#include "urldecode.h"
#include "httpresponse.h"

#include <cstdlib>
#include <cstring>

inline static bool GetContentLenth(const FCGX_ParamArray& envp, int& contentLength)
{
    static const int  kContentLengthMax = 2048;
    contentLength = 0;
    auto pContentLength = FCGX_GetParam("CONTENT_LENGTH", envp);

    if (pContentLength != nullptr)
        contentLength = strtol(pContentLength, nullptr, 10);
        
    // LOGGING(LOG_INFO, "Receiving Message :length = %d", contentLength);

    if(contentLength > kContentLengthMax-1 || contentLength < 0)
    {
        LOGGING(LOG_ERROR, "Receiving Message length error :length = %d", contentLength);
        return false;
    }
    
    return true;
}

inline static char* GetRecvContent(FCGX_Stream* in, int contentLength)
{
    char* pRecvBuf = (char*)malloc(contentLength + 1);
    if (nullptr == pRecvBuf)
        return nullptr;

    memset(pRecvBuf,0,contentLength + 1);

    int readn = FCGX_GetStr(pRecvBuf, contentLength, in);
    if(readn != contentLength)
    {
        LOGGING(LOG_ERROR, "FCGX_GetStr() read len=[%d], contentLength=[%d]", readn, contentLength);
        free(pRecvBuf);
        pRecvBuf = nullptr;
        return nullptr;
    }

    // if (pRecvBuf)
        // LOGGING(LOG_INFO, "Receiving Message : %s", pRecvBuf);

    return pRecvBuf;
}


void HttpPost::Handle(FCGX_ParamArray& envp, FCGX_Stream* in)
{
    int contentLength = 0;
    if(!GetContentLenth(envp, contentLength))
        return;

    auto pRecvBuf = GetRecvContent(in, contentLength);
    if(pRecvBuf == nullptr)
        return;
    
    char* pDecodeRecvBuf = urlDecode(pRecvBuf);
    if(pDecodeRecvBuf == nullptr)
        return;

    // LOGGING(LOG_INFO, "After Urldecode Receiving Message : %s", pDecodeRecvBuf);

	auto requestUri = FCGX_GetParam("REQUEST_URI", envp);
	// LOGGING(LOG_INFO, "POST REQUEST_URI=[%s]", requestUri);

    auto ret = HttpPostDispatcherInstance().Dispatch(requestUri, pDecodeRecvBuf);
    if(!ret)
        FCGX_ResponseNull();

    if(pRecvBuf)
        free(pRecvBuf);

    if(pDecodeRecvBuf)
        free(pDecodeRecvBuf);
}

HttpPostDispatcher& HttpPostDispatcherInstance()
{
    static HttpPostDispatcher instance;
    return instance;
}
