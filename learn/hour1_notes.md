# 第1小时学习笔记 - 项目架构与C++核心语法

## 🎯 学习目标达成
✅ 理解项目整体架构  
✅ 掌握C++类的初始化列表语法  
✅ 理解函数指针与std::bind  
✅ 掌握引用传递的使用场景  
✅ 理解HTTP路由机制  

---

## 📊 项目架构总览

### 核心组件关系图
```
gobang_server (主控制器)
├── wsserver_t _wssrv          (WebSocket服务器)
├── user_table _ut             (数据库操作)
├── online_manager _om         (在线用户管理)
├── room_manager _rm           (房间管理)
├── matcher _mm                (匹配系统)
└── session_manager _sm        (会话管理)
```

### 请求处理流程
```
HTTP请求 → http_callback() → 路由分发
├── POST /reg → reg() (注册)
├── POST /login → login() (登录)
├── GET /info → info() (用户信息)
└── 其他 → file_handler() (静态文件)

WebSocket连接 → wsopen_callback() → 路由分发
├── /hall → wsopen_game_hall() (大厅连接)
└── /room → wsopen_game_room() (房间连接)
```

---

## 🔑 核心C++语法详解

### 1. 初始化列表语法
```cpp
// 语法格式
ClassName(参数) : member1(value1), member2(value2) {
    // 构造函数体
}

// 项目实例
gobang_server(const std::string &host) : 
    _web_root("./wwwroot/"),              // 直接初始化
    _ut(host, user, pass, dbname, port),  // 传参初始化
    _rm(&_ut, &_om),                      // 指针初始化
    _sm(&_wssrv) {                        // 对象指针初始化
    // 函数体
}
```

**要点：**
- 效率更高：直接初始化，避免先构造再赋值
- 必须使用场景：const成员、引用成员、无默认构造函数的成员
- 执行顺序：按成员声明顺序，不是初始化列表顺序

### 2. std::bind函数绑定
```cpp
// 基本语法
std::bind(函数地址, 对象指针, 占位符...);

// 项目应用
_wssrv.set_http_handler(
    std::bind(&gobang_server::http_callback,    // 成员函数地址
              this,                             // 当前对象
              std::placeholders::_1)            // 第1个参数占位符
);
```

**原理：**
- 将成员函数绑定到具体对象
- 占位符表示运行时传入的参数位置
- 等价于：`this->http_callback(参数1)`

### 3. 引用传递优化
```cpp
// 值传递 vs 引用传递
void func1(std::string str);           // 复制，慢
void func2(std::string& str);          // 引用，快，可修改
void func3(const std::string& str);    // 常量引用，快，只读

// 项目应用
void http_resp(wsserver_t::connection_ptr &conn,  // 避免复制大对象
               const std::string &reason) {       // 只读且高效
```

**选择原则：**
- 大对象：使用引用避免复制开销
- 只读：使用const引用
- 需要修改：使用普通引用

---

## 🌐 HTTP处理机制深度解析

### 用户登录完整流程
```cpp
POST /login → login()函数
├── 1. JSON反序列化：字符串 → Json::Value对象
├── 2. 参数校验：检查用户名密码是否完整
├── 3. 数据库验证：调用_ut.login()验证用户
├── 4. 创建会话：生成session_ptr，设置过期时间
├── 5. 设置Cookie：通过HTTP头部返回SSID
└── 6. JSON响应：返回成功或失败信息
```

### 关键代码段解析
```cpp
// JSON反序列化（第93-99行）
std::string req_body = conn->get_request_body();
Json::Value login_info;
bool ret = json_util::unserialize(req_body, login_info);

// 会话创建（第112-118行）
uint64_t uid = login_info["id"].asUInt64();
session_ptr ssp = _sm.create_session(uid, LOGIN);
_sm.set_session_expire_time(ssp->ssid(), SESSION_TIMEOUT);

// Cookie设置（第120-121行）
std::string cookie_ssid = "SSID=" + std::to_string(ssp->ssid());
conn->append_header("Set-Cookie", cookie_ssid);
```

---

## 🧠 智能指针应用

### 类型定义
```cpp
using session_ptr = std::shared_ptr<session>;  // 会话智能指针
using connection_ptr = wsserver_t::connection_ptr;  // 连接智能指针
```

### 使用场景
- **shared_ptr**：多个地方需要访问同一资源（如session、connection）
- **自动管理**：引用计数为0时自动释放内存
- **线程安全**：引用计数操作是原子的

---

## 🔍 路由机制分析

### HTTP路由表
| 方法 | 路径 | 处理函数 | 功能 |
|------|------|----------|------|
| POST | /reg | reg() | 用户注册 |
| POST | /login | login() | 用户登录 |
| GET | /info | info() | 获取用户信息 |
| * | * | file_handler() | 静态文件服务 |

### WebSocket路由表
| 路径 | 连接处理 | 消息处理 | 功能 |
|------|----------|----------|------|
| /hall | wsopen_game_hall() | wsmsg_game_hall() | 游戏大厅 |
| /room | wsopen_game_room() | wsmsg_game_room() | 游戏房间 |

---

## 💡 重要概念总结

1. **模块化设计**：每个类负责单一职责，松耦合
2. **回调机制**：通过std::bind将成员函数注册为回调
3. **资源管理**：智能指针自动管理内存生命周期
4. **效率优化**：引用传递避免不必要的复制
5. **错误处理**：完整的错误检查和响应机制

---

## 🎯 下一步学习计划

1. **第2小时**：深入学习matcher.hpp（匹配系统）和多线程编程
2. **重点关注**：线程安全、条件变量、阻塞队列
3. **实践目标**：理解玩家匹配的完整流程

---

## ❓ 思考题
1. 为什么要使用初始化列表而不是在构造函数体中赋值？
2. std::bind的优势是什么？有什么替代方案？
3. 什么情况下应该使用引用传递？
4. HTTP和WebSocket的主要区别是什么？

*答案将在下一小时的学习中揭晓！*
