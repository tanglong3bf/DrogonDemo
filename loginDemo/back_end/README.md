# LoginDemoBackEnd

登录案例的后端部分。

## 一些细节

1. 前端不对密码进行加密，数据库中存储密码的MD5值。
2. `token`由`drogon::utils::uuid`生成，在登录后存储在session中。
3. 在主函数中注册了全局异常处理器。
4. sql中唯一的一条数据，用户密码是 '123456'。
