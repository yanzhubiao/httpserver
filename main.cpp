#include "http/fcgi.h"

#include "handle/gethandle.h"
#include "handle/posthandle.h"

char g_logfile[256] = "hydra.log";

static void HttpHandleRegister()
{
    HttpGetHandleRegister();
    HttpPostHandleRegister();
}

int main(void)
{
    HttpHandleRegister();

    FCgi fcgi;
    fcgi.Handle();

    return 0;
}
