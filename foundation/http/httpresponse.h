#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <fcgi_stdio.h>
#include <fcgiapp.h>

void FCGX_SetStreamOut(FCGX_Stream* out);

void FCGX_Response(const char *data, int contentLength);
void FCGX_ResponseNull();

#endif