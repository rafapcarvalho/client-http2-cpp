#include <iostream>
#include <sstream>
//#include <curl/curl.h>
#include <jsoncpp/json/json.h> // Para parsear a resposta em formato JSON
#include "/opt/rh/httpd24/root/usr/include/curl/curl.h"

// Função de retorno de chamada para concatenar os dados recebidos na resposta à string de resposta existente
size_t writeFunction(char* ptr, size_t size, size_t nmemb, std::string* response) {
    size_t realsize = size * nmemb;
    response->append(ptr, realsize);
    return realsize;
}


int main() {

    // Inicicialando o curl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Criando um hadle curl
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Erro ao inicializar o curl\n";
        return 1;
    }

    // Configurar a solicitação HTTP/2
    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.5.89:8010/oauth2/token");
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE);
    //curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
    //curl_easy_setopt(curl, CURLOPT_SSLCERT, "/home/rafa/Sixbell/http2/c++/client/config/pcf.pem");
    //curl_easy_setopt(curl, CURLOPT_SSLKEY, "/home/rafa/Sixbell/http2/c++/client/config/pcf.key");
   // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "/home/rafa/Sixbell/http2/c++/client/config/pcf.pem");
    //curl_easy_setopt(curl, CURLOPT_MAX_RECV_SPEED_LARGE, 8178L); // Limite de velocidade de recebimento em bytes por segundo
    //curl_easy_setopt(curl, CURLOPT_SSL_CIPHER_LIST, "TLSv1");
    // Salvando o sslkey.log para verificação com wireshark
    curl_easy_setopt(curl, CURLOPT_SSLKEY, "sslkeynrf.log");
    //curl_easy_setopt(curl, CURLOPT_SSLKEYLOGFILE, "sslkey.log");

    std::stringstream params;
    params  << "grant_type=authorization_code"
            << "&nfInstanceId=ab79328b-e3f8-4945-b67a-63225afd01d1"
            << "&nfType=PCF"
            << "&targetNfType=PCF"
            << "&scope=create+delete"
            << "&targetNfInstanceId=ab79328b-e3f8-4945-b67a-63225afd01d1"
            << "&RequestPlmn={\"mmc\":\"215\",\"mnc\":\"30\"}"
            << "&TargetPlmn={\"mmc\":\"216\",\"mnc\":\"30\"}";
    std::string paramsStr = params.str();
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, paramsStr.c_str());

    // Configurando a solicitação para usar o método POST e o Content-Type application/x-www-form-urlencoded
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    headers = curl_slist_append(headers, "Content-Encoding: gzip");
    headers = curl_slist_append(headers, "Accept-Encoding: gzip,json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Definindo a função de retorno de chamada para concatenar os dados da resposta à string de resposta
    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Habilitando o modo verbose do curl
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

    // Habilitando visualização de informações da versão do curl
    std::cout << "-----------------------------------------------------------------------\n";
    std::cout << "versão:  " << curl_version() << "\n";
    std::cout << "-----------------------------------------------------------------------\n";

    // Realizar a solicitação
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Error making request: " << curl_easy_strerror(res) << "\n";
        return 1;
    }



    // Obter a resposta da solicitação
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    if (httpCode != 200) {
        std::cerr << "Error requesting token: HTTP code " << httpCode << "\n";
        return 1;
    }

    //Parse da resposta como JSON
    Json::Value root;
    Json::CharReaderBuilder builder;
    std::string errors;
    std::istringstream responseStream(response);
    if (!parseFromStream(builder, responseStream, &root, &errors)) {
        std::cerr << "Error parsing response as JSON: " << errors << "\n";
        return 1;
    }

    // Imprimir o corpo da mensagem JSON
    std::cout << root.toStyledString() << "\n";

    // Limpando curl e responses
    //delete[] response;
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}
