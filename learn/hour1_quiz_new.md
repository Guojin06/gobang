# 第1小时学习检测 - 由简到难梯度测试

## 📋 测试说明
- **基础题（1-3题）**：考查基本概念理解
- **应用题（4-6题）**：考查语法应用能力  
- **分析题（7-8题）**：考查代码分析能力
- **综合题（9-10题）**：考查系统性理解

**请完成所有题目后，告诉我您的答案，我再进行检查和点评！**

---

## 🟢 基础题（1-3题）- 概念理解

### 1. 【单选】项目架构识别
`gobang_server`类中，负责"数据库操作"的成员变量是？
A. `_wssrv`
B. `_ut` 
C. `_om`
D. `_rm`
答：B

### 2. 【判断】初始化列表基础
下面说法正确吗？
"C++的初始化列表语法是：构造函数参数后面用冒号，然后是成员变量的初始化"
A. 正确
B. 错误
答：对

### 3. 【单选】HTTP方法识别
用户登录请求应该使用什么HTTP方法？
A. GET
B. POST
C. PUT  
D. DELETE
答：A
---

## 🟡 应用题（4-6题）- 语法应用

### 4. 【填空】初始化列表实践
请完成以下构造函数的初始化列表：
```cpp
gobang_server(const std::string &host) : 
    _web_root("./wwwroot/"),
    _ut(_host______, user, pass, dbname, port),
    _rm(&_ut, __&om_____),
    _sm(&__wssrv_____)
{
    // 构造函数体
}
```

### 5. 【选择】std::bind应用理解
以下代码的作用是什么？
```cpp
_wssrv.set_http_handler(std::bind(&gobang_server::http_callback, this, std::placeholders::_1));
```
A. 定义一个HTTP处理函数
B. 将成员函数绑定为HTTP请求的回调函数
C. 创建一个新的HTTP服务器
D. 设置HTTP请求的参数
答：B
### 6. 【选择】引用传递优势
为什么函数参数使用`const std::string &reason`而不是`std::string reason`？
A. 为了能修改reason的值
B. 为了避免复制字符串，提高效率
C. 为了让reason为空
D. 为了支持多线程
答：B
---

## 🟠 分析题（7-8题）- 代码分析

### 7. 【多选】HTTP路由分析
根据`http_callback`函数，以下哪些请求会被正确路由？
```cpp
if (method == "POST" && uri == "/reg") {
    return reg(conn);
}else if (method == "POST" && uri == "/login") {
    return login(conn);
}else if (method == "GET" && uri == "/info") {
    return info(conn);
}else {
    return file_handler(conn);
}
```
A. `POST /reg` → 用户注册
B. `GET /login` → 用户登录  
C. `POST /login` → 用户登录
D. `GET /info` → 获取用户信息
E. `GET /index.html` → 静态文件服务
答：ABD
### 8. 【分析】会话创建流程
用户登录成功后，会话创建的步骤是？请按正确顺序排列：
A. 设置Cookie头部
B. 创建session对象
C. 获取用户ID
D. 设置session过期时间
E. 返回成功响应

正确顺序：__B___ → __C___ → __A___ → __D___ → __E___

---

## 🔴 综合题（9-10题）- 系统理解

### 9. 【编程】错误处理补全
以下是简化的登录函数，请找出所有可能的错误情况，并说明应该如何处理：
```cpp
void login(wsserver_t::connection_ptr &conn) {
    std::string req_body = conn->get_request_body();
    Json::Value login_info;
    json_util::unserialize(req_body, login_info);
    
    _ut.login(login_info);
    
    uint64_t uid = login_info["id"].asUInt64();
    session_ptr ssp = _sm.create_session(uid, LOGIN);
    
    return http_resp(conn, true, websocketpp::http::status_code::ok, "登录成功");
}
```

请列出至少3个错误处理点：
- 错误点1：_缺少反序列化失败的处理，如果失败，返回400________________
- 错误点2：_缺少对数据库是否有此数据的验证，若失败返回400________________  
- 错误点3：_有可能创建会话失败，还缺少时间限制________________

### 10. 【设计】架构扩展思考
假设要为这个五子棋项目添加"观战功能"，观众可以观看正在进行的对局。
请思考：
1. 需要修改哪些模块？
2. 需要添加什么新的数据结构？
3. WebSocket路由需要如何扩展？

答案要点：
- 修改模块：__房间管理模块（可容纳多人），__在线管理器模块_____________
- 新增数据结构：__栈_______________
- 路由扩展：__扩展观战路由_______________

---



## 🎯 完成提示

完成所有题目后，请告诉我：
1. 您的所有答案
2. 觉得哪些题目比较困难
3. 对哪些知识点还不够清楚

我会根据您的答题情况：
- ✅ 详细分析每道题的考点
- 🔍 针对错题进行专项讲解  
- 📚 为薄弱环节提供补充学习资料
- 🚀 制定第2小时的个性化学习重点
