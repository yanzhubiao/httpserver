# httpserver
nginx + fcgi + spawn-fcgi

对fcgi的概念理解：
see: http://www.cnblogs.com/skynet/p/4173450.html

需求：处理http请求，处理方法有get、post

环境配置：
平台:ubuntu
软件需求：gcc、c++11、nginx、fcgi、spawn-fcgi、cmake
see: http://chriswu.me/blog/writing-hello-world-in-fcgi-with-c-plus-plus/
nginx配置路径 /etc/nginx/nginx.conf
配置内容:
user www-data;
worker_processes auto;
pid /run/nginx.pid;

events {
        worker_connections 768;
        # multi_accept on;
}

http {
server {
                location ~ \.cgi$
                {
                fastcgi_pass 127.0.0.1:6666;
                fastcgi_index index.cgi;
                fastcgi_param SCRIPT_FILENAME fcgi$fastcgi_script_name;
                include fastcgi_params;
                }
       }
}

设计架构：

main
|->handle
                 |->gethandle
                 |->posthandle
|->foundation
                 |->http
                             |->httpget
                             |->httppost
                             |->despatcher
                             |->urldecode
                             |->-lfcgi
                 |->json
                 |->log

代码注释：
log: 仅支持ubuntu,  考虑用easylog代替，支持跨平台
json: cjson see: https://github.com/DaveGamble/cJSON 
http: httpget、httppost
despatcher:   高效简洁的事件发布器，通过std::unordered_multimap实现
urldecode：解码，里面有一个malloc操作需要优化。考虑用libcurl:curl_easy_unescape代替，
                   see: https://curl.haxx.se/libcurl/c/curl_easy_unescape.html
lfcgi: libfcgi库，仅支持ubuntu，可以考虑引入对应的库支持window

编译工具： cmake
平台支持：
ubuntu：         支持
window：        暂不支持，如果以后基于该架构的应用增多，有调试需求，可以支持。

开发心得：
（1）原来的代码中，有大量的字符串拷贝分割，通过std::strtok、std::strchr分割字符串，获得request_uri并通过despatcher分发。
（2）原来的代码中，有大量的字符串比较操作，通过std::unordered_multimap中的字符串hash处理，避免字符串的比较操作。
（3）在c++11中，引入一些简洁、高效的容器，通过学习使用，简化代码，一个高效的字符串事件发布器，实现代码仅20-30行。

附加：
see: http://blog.csdn.net/zhouzhaoxiong1227/article/details/72236922
查看标准源版本  apt-cache madison nginx
ubuntu添加个人源  https://zhidao.baidu.com/question/243134320363485284.html 
ubuntu版本 lsb_release -d

