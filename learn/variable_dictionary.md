# C++变量名词典 - 看懂每个变量

## 🎯 变量命名规律

### 前缀含义
- `_` 开头：私有成员变量（类内部使用）
- 无前缀：局部变量（函数内部使用）

### 后缀含义
- `_ptr`：指针类型
- `_str`：字符串类型  
- `_id`：ID标识符

---

## 📚 核心变量详解

### 类成员变量（gobang_server类）

| 变量名 | 完整含义 | 简单理解 | 实际作用 |
|--------|----------|----------|----------|
| `_web_root` | Web Root Directory | 网站根目录 | 存放HTML/CSS/JS文件的文件夹路径 |
| `_wssrv` | WebSocket Server | WebSocket服务器 | 处理所有网络连接和消息 |
| `_ut` | User Table | 用户表操作器 | 专门操作数据库中的用户信息 |
| `_om` | Online Manager | 在线管理器 | 记录哪些用户正在线上 |
| `_rm` | Room Manager | 房间管理器 | 管理所有游戏房间 |
| `_mm` | Match Maker | 匹配制造者 | 为玩家寻找对手 |
| `_sm` | Session Manager | 会话管理器 | 管理用户登录状态 |

### 函数参数和局部变量

| 变量名 | 完整含义 | 简单理解 | 实际作用 |
|--------|----------|----------|----------|
| `conn` | Connection | 连接 | 与客户端的网络连接 |
| `req` | Request | 请求 | 客户端发来的请求信息 |
| `resp` | Response | 响应 | 服务器返回的响应信息 |
| `req_body` | Request Body | 请求正文 | POST请求中的数据内容 |
| `resp_body` | Response Body | 响应正文 | 返回给客户端的数据内容 |
| `login_info` | Login Information | 登录信息 | 包含用户名密码的JSON对象 |
| `user_info` | User Information | 用户信息 | 包含用户详细信息的JSON对象 |
| `cookie_str` | Cookie String | Cookie字符串 | 从HTTP头部获取的Cookie内容 |
| `ssid_str` | Session ID String | 会话ID字符串 | 从Cookie中提取的Session ID |
| `ret` | Return | 返回值 | 函数调用的返回结果（成功/失败） |

### 智能指针类型

| 类型名 | 完整含义 | 简单理解 | 实际作用 |
|--------|----------|----------|----------|
| `session_ptr` | Session Pointer | 会话指针 | 指向会话对象的智能指针 |
| `connection_ptr` | Connection Pointer | 连接指针 | 指向连接对象的智能指针 |
| `room_ptr` | Room Pointer | 房间指针 | 指向房间对象的智能指针 |
| `message_ptr` | Message Pointer | 消息指针 | 指向消息对象的智能指针 |

### ID标识符

| 变量名 | 完整含义 | 简单理解 | 实际作用 |
|--------|----------|----------|----------|
| `uid` | User ID | 用户ID | 用户的唯一标识号码 |
| `ssid` | Session ID | 会话ID | 会话的唯一标识号码 |
| `rid` | Room ID | 房间ID | 房间的唯一标识号码 |
| `hdl` | Handle | 句柄 | WebSocket连接的标识符 |

---

## 🔍 实际例子理解

### 例子1：用户登录过程中的变量
```cpp
void login(wsserver_t::connection_ptr &conn) {
    // conn = 与张三浏览器的连接
    
    std::string req_body = conn->get_request_body();
    // req_body = "{\"username\":\"zhang\", \"password\":\"123456\"}"
    
    Json::Value login_info;
    // login_info = 空的JSON容器，准备装登录信息
    
    bool ret = json_util::unserialize(req_body, login_info);
    // ret = true（解析成功）
    // login_info = {"username":"zhang", "password":"123456"}
    
    uint64_t uid = login_info["id"].asUInt64();
    // uid = 1001（张三的用户ID）
    
    session_ptr ssp = _sm.create_session(uid, LOGIN);
    // ssp = 指向张三会话对象的指针
    
    std::string cookie_ssid = "SSID=" + std::to_string(ssp->ssid());
    // cookie_ssid = "SSID=12345"（张三的会话ID）
}
```

### 例子2：HTTP路由中的变量
```cpp
void http_callback(websocketpp::connection_hdl hdl) {
    // hdl = 连接句柄（类似"3号窗口"的号码牌）
    
    wsserver_t::connection_ptr conn = _wssrv.get_con_from_hdl(hdl);
    // conn = 根据号码牌找到具体的连接对象
    
    websocketpp::http::parser::request req = conn->get_request();
    // req = 解析后的HTTP请求信息
    
    std::string method = req.get_method();
    // method = "POST"（HTTP方法）
    
    std::string uri = req.get_uri();
    // uri = "/login"（请求路径）
}
```

---

## 🧠 记忆技巧

### 1. 联想记忆法
- `conn` → Connection → "看嗯"（看到连接）
- `req` → Request → "锐窝"（请求）
- `resp` → Response → "锐斯波"（响应）
- `ssp` → Session Shared Pointer → "Session指针"

### 2. 功能记忆法
- 以`_`开头的：类的"私人物品"
- 以`_ptr`结尾的：指向某个对象的"箭头"
- 以`_str`结尾的：文字内容
- 以`_id`结尾的：身份证号码

### 3. 类比记忆法
- `session` = 酒店房卡（证明你已经登录）
- `connection` = 电话线（你和服务器的通信渠道）
- `manager` = 管理员（专门管理某类事务）
- `handler` = 处理器（专门处理某种任务）

---

## 💡 快速查找技巧

当你看到陌生变量时，问自己：

1. **在哪里定义的？**
   - 类成员：看类的private/public部分
   - 局部变量：看函数开头的定义

2. **什么类型？**
   - `std::string`：文字内容
   - `Json::Value`：JSON数据
   - `bool`：真/假
   - `uint64_t`：大整数

3. **从哪里来？**
   - `= conn->get_xxx()`：从连接对象获取
   - `= req.get_xxx()`：从请求对象获取
   - `= _xx.xxx()`：从某个管理器获取

4. **去哪里用？**
   - 作为参数传给其他函数
   - 用于条件判断（if语句）
   - 用于返回响应

---

## 🎯 练习建议

1. **每次看到新变量，先查这个词典**
2. **在代码旁边写中文注释**
3. **用自己的话重新命名变量**
4. **画图表示变量之间的关系**

**这样是不是清楚多了？您还有哪些变量不理解吗？**
