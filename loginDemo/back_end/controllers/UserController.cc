#include "UserController.h"
#include <drogon/orm/Criteria.h>
#include <drogon/orm/Exception.h>
#include <drogon/utils/Utilities.h>
#include <string>
#include <trantor/utils/Logger.h>
#include "../models/User.h"
#include <drogon/orm/Mapper.h>

// Add definition of your processing function here

using namespace drogon::orm;
using namespace drogon_model::login_demo_db;

void UserController::login(
    const HttpRequestPtr &req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const UserDTO userDTO) const {
    
    Json::Value json;
    auto dbClient = app().getDbClient();
    auto userMapper = Mapper<User>(dbClient);
    try {
        User user = userMapper.findOne({User::Cols::_username, CompareOperator::EQ, userDTO.username});
        // 前端传过来的是明文，这里加密成md5
        auto encodePassword = drogon::utils::getMd5(userDTO.password);
        if (encodePassword == user.getValueOfPassword()) {
            json["message"] = "登录成功";

            // 获取之前生成的token
            auto oldToken = req->getSession()->get<std::string>("token");
            if (oldToken != "") {
                json["token"] = oldToken;
            } else {
                auto token = drogon::utils::getUuid();
                json["token"] = token;
                req->getSession()->insert("token", token);
            }
            LOG_INFO << "用户 [" << userDTO.username << "] 登录成功";
        } else {
            json["error"] = "用户名或密码错误";
            LOG_WARN << "用户 [" << userDTO.username << "] 登录失败";
        }
    } catch (drogon::orm::DrogonDbException &e) {
        json["error"] = "用户名或密码错误";
        LOG_WARN << "有闸总尝试使用用户名 [" << userDTO.username << "] 进行登录";
        LOG_WARN << "闸总ip: " << req->getPeerAddr().toIp();
    }
    auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void UserController::registerUser(
    const HttpRequestPtr &req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const UserDTO userDTO) const {

    Json::Value json;
    auto dbClient = drogon::app().getDbClient();
    auto userMapper = Mapper<User>(dbClient);
    try {
        auto password = drogon::utils::getMd5(userDTO.password);
        User user;
        user.setUsername(userDTO.username);
        user.setPassword(password);
        userMapper.insert(user);
        json["message"] = "注册成功";
    } catch (drogon::orm::DrogonDbException &e) {
        json["error"] = "重复的用户名";
        LOG_WARN << e.base().what();
    }
    auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void UserController::logout(
    const HttpRequestPtr &req,
    std::function<void (const HttpResponsePtr &)> &&callback) const {

    req->getSession()->erase("token");

    Json::Value json;
    json["message"] = "退出成功";
    auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

namespace drogon
{

/**
 * 从请求体中取出数据并封装为UserDTO结构体
 * 这里抛出的异常交给全局异常处理器进行处理
 * 全局异常处理器在main函数中进行注册
 */
template <>
inline UserDTO fromRequest(const HttpRequest &req)
{
    auto json = req.getJsonObject();
    UserDTO user;
    if (json) {
        if (json->isMember("username")) {
            auto username = json->get("username", "").asString();
            if (username.length() < 8) {
                throw std::runtime_error("用户名长度过短");
            }
            for (const auto &c : username) {
                if (c != '_' && (c < '0' || c > '9' && c < 'A' || c > 'Z' && c < 'a' || c > 'z')) {
                    throw std::runtime_error("用户名仅可使用大小写英文字母、阿拉伯数字、英文下划线");
                }
            }
            user.username = username;
        } else {
            throw std::runtime_error("缺少必备字段：username");
        }
        if (json->isMember("password")) {
            auto password = json->get("password", "").asString();
            if (password.length() < 6) {
                throw std::runtime_error("密码长度过短");
            }
            for (const auto &c : password) {
                if (c != '_' && (c < '0' || c > '9' && c < 'A' || c > 'Z' && c < 'a' || c > 'z')) {
                    throw std::runtime_error("密码仅可使用大小写英文字母、阿拉伯数字、英文下划线");
                }
            }
            user.password = password;
        } else {
            throw std::runtime_error("缺少必备字段：password");
        }
    } else {
        throw std::runtime_error("请求体格式错误，请使用json格式");
    }
    return user;
}
}
