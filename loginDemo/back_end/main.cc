#include <drogon/HttpResponse.h>
#include <drogon/drogon.h>
#include <exception>

int main() {
    //Set HTTP listener address and port
    //drogon::app().addListener("0.0.0.0",80);
    //Load config file
    drogon::app().loadConfigFile("../config.json");

    /**
     * 全局异常处理
     */
    drogon::app().setExceptionHandler([](
        const std::exception &e,
        const drogon::HttpRequestPtr &req,
        std::function<void (drogon::HttpResponsePtr &)> &&callback) {

        LOG_ERROR << e.what();

        Json::Value json;
        json["error"] = e.what();
        auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
        callback(resp);
    });

    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}
