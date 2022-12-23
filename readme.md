# client-http2-cpp

Este cliente é um teste de comunicação HTTP2.

Usando o método POST contra o servidor NRF do projeto free5GC, para testar a autenticação via OAuth2.
E posteriormente com outros métodos para testar todos as URLs do NRF.

## Cliente http2 com OAuth2

Para compilar o código é necessário no mínimo c++11

E ter instalado as bibliotecas httpd24, jsoncpp-devel

Para instalar basta seguir os passos abaixo:

```bash
yum install centos-release-scl-rh
yum install httpd24-libcurl
yum install httpd24-libcurl-devel
yum install jsoncpp-devel
```

A versão de **libcurl** usada para o teste foi: **libcurl/7.61.1**

## Comando para compilar com g++

```bash
g++ -std=c++14 -lcurl -I/opt/rh/httpd24/root/usr/include/curl -L /opt/rh/httpd24/root/usr/lib64 -Wl,-rpath=/opt/rh/httpd24/root/usr/lib64/ -ljsoncpp -o client14 main.cpp
```

## Comando para compilar com CMake

```bash
cmake .
make
```


## Execução do binário e resposta

```zsh
~/http2/c++/client>$  ./client14 
-----------------------------------------------------------------------
versão:  libcurl/7.61.1 NSS/3.36 zlib/1.2.7 libssh2/1.8.0 nghttp2/1.7.1
-----------------------------------------------------------------------
< content-type: application/json; charset=utf-8
< content-length: 238
< date: Thu, 22 Dec 2022 20:49:18 GMT
<
* Connection #0 to host 192.168.5.89 left intact
{
   "access_token" : "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiIiLCJzdWIiOiIiLCJhdWQiOiIiLCJzY29wZSI6IiIsImV4cCI6MTY3MTc0MzE1OCwiaWF0IjoxNjcxNzQyMTU4fQ.98hbi7UxevwTxk38wEYnYmDY73hOPdaPav8BdsV8XbM",
   "expires_in" : 1000,
   "token_type" : "Bearer"
}

~/http2/c++/client>$
```

## Para analises de traces

É necessário ter a variável de ambiente **SSLKEYLOGFILE** criada.

- Linux : 
  - export SSLKEYLOGFILE=filesslkey.log

- Configurar o arquivo gerado no wireshark através do menu: 
  - Edit ->  Preferences -> Protocols -> TLS:
    - No parâmetro (Pre)-Master_secret log filename: Adicionar o arquivo filesslkey.log
  - Logo basta filtrar por http2 

