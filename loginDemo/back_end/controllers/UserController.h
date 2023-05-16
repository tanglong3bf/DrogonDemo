#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

// 一般来讲，登录和注册时的字段可能不同
// 需要将UserDTO拆分为UserLogin和UserRegister并分别填充不同的字段
// 本案例暂不考虑
struct UserDTO {
    std::string username;
    std::string password;
};

class UserController : public drogon::HttpController<UserController>
{
  public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserController::login, "/user/login", Post);
    ADD_METHOD_TO(UserController::registerUser, "/user/register", Post);
    ADD_METHOD_TO(UserController::logout, "/user/logout", Post);

    METHOD_LIST_END
    void login(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, UserDTO user) const;
    void registerUser(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, UserDTO user) const;
    void logout(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) const;
};
