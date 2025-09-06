# 第1小时学习检测 - 项目架构与C++基础语法

## 题目列表（共10题）

### 1. 【架构理解】多选题
在`gobang_server`类中，包含了哪些核心组件？
A. `wsserver_t _wssrv` - WebSocket服务器  
B. `user_table _ut` - 数据库操作  
C. `online_manager _om` - 在线用户管理  
D. `room_manager _rm` - 房间管理  
E. `matcher _mm` - 匹配系统  
F. `session_manager _sm` - 会话管理  

**答案：ABCDEF（全选）**

### 2. 【初始化列表】填空题
请完成以下构造函数的初始化列表：
```cpp
gobang_server(const std::string &host, const std::string &user):
    _web_root("./wwwroot/"),
    _ut(_______, _______, pass, dbname, port),
    _rm(&_ut, _______),
    _sm(&_______)
```

**答案：host, user, &_om, _wssrv**

### 3. 【路由理解】选择题
HTTP请求`POST /login`会被路由到哪个函数？
A. `file_handler()`
B. `reg()`
C. `login()`
D. `info()`

**答案：C**

### 4. 【WebSocket回调】判断题
`wsopen_callback`函数负责处理WebSocket连接建立时的逻辑。（）

**答案：正确**

### 5. 【std::bind语法】选择题
以下std::bind语法中，`std::placeholders::_1`的作用是：
A. 绑定第一个参数的值
B. 为第一个参数位置预留占位符
C. 调用第一个函数
D. 返回第一个结果

**答案：B**

### 6. 【引用vs指针】选择题
函数参数`wsserver_t::connection_ptr &conn`中，`&`的作用是：
A. 取地址操作符
B. 引用传递，避免复制connection对象
C. 指针解引用
D. 逻辑与操作符

**答案：B**

### 7. 【JSON处理】填空题
在项目中，将JSON字符串转换为Json::Value对象使用的函数是：
`json_util::_______`

**答案：unserialize**

### 8. 【会话管理】选择题
用户登录成功后，session ID通过什么方式返回给客户端？
A. JSON响应体
B. HTTP状态码
C. Cookie头部
D. WebSocket消息

**答案：C**

### 9. 【智能指针】判断题
`session_ptr`是`std::shared_ptr<session>`的类型别名。（）

**答案：正确**

### 10. 【业务流程】排序题
请将用户完整游戏流程按正确顺序排列：
A. 进入游戏大厅
B. 用户注册
C. 开始对局
D. 匹配对手
E. 用户登录
F. 创建房间

**答案：B → E → A → D → F → C**

---

## 评分标准
- 全对（9-10题）：优秀，架构理解很到位
- 良好（7-8题）：不错，个别细节需加强
- 一般（5-6题）：基本理解，需要重点复习
- 需要重学（<5题）：建议重新学习第1小时内容

---

## 错题分析指导

### 如果第1题错误：
需要重新理解项目的模块化设计，每个组件都有明确职责分工。

### 如果第2、5题错误：
需要加强C++语法：初始化列表和std::bind的理解。

### 如果第3、4、8题错误：
需要重新梳理HTTP和WebSocket的路由机制。

### 如果第6题错误：
需要复习C++引用的概念和使用场景。

### 如果第7题错误：
需要了解JSON序列化/反序列化的基本概念。

### 如果第9题错误：
需要学习C++类型别名（using）和智能指针的基础知识。

### 如果第10题错误：
需要重新理解整个业务流程的逻辑顺序。
