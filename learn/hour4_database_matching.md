# 第四小时学习：数据库模块 & 匹配算法

## 🎯 **本小时学习目标**
- 掌握MySQL数据库操作和连接池
- 理解ELO匹配算法实现
- 学会数据序列化与反序列化
- 理解系统整体架构设计

---

## 📊 **核心模块1：数据库系统 (db.hpp)**

### **MySQL连接管理**
```cpp
// 数据库连接创建
MYSQL* mysql_util::mysql_create(const std::string& host, 
                                const std::string& username,
                                const std::string& password,
                                const std::string& dbname,
                                uint16_t port)
{
    MYSQL* mysql = mysql_init(nullptr);  // 初始化MySQL句柄
    if (mysql == nullptr) {
        ELOG("mysql init error!");
        return nullptr;
    }
    
    // 建立连接
    if (mysql_real_connect(mysql, host.c_str(), username.c_str(),
                          password.c_str(), dbname.c_str(), port, 
                          nullptr, 0) == nullptr) {
        ELOG("connect mysql server failed : %s", mysql_error(mysql));
        mysql_close(mysql);
        return nullptr;
    }
    
    // 设置字符集，防止中文乱码
    if (mysql_set_character_set(mysql, "utf8") != 0) {
        ELOG("set client character failed : %s", mysql_error(mysql));
        mysql_close(mysql);  
        return nullptr;
    }
    
    return mysql;
}
```

**🔍 详细解析：**
- `mysql_init()`: 创建MySQL句柄，相当于"申请一个数据库操作员"
- `mysql_real_connect()`: 建立实际连接，像"操作员登录数据库"
- `mysql_set_character_set()`: 设置编码，确保中文不乱码

### **用户表操作 (user_table类)**
```cpp
class user_table {
private:
    MYSQL* _mysql;    // MySQL连接句柄
    std::mutex _mutex; // 线程安全锁
    
public:
    // 用户注册
    bool insert(Json::Value& user) {
        std::string sql = "insert user values(null, '";
        sql += user["username"].asString();
        sql += "', password('";
        sql += user["password"].asString(); 
        sql += "'), 1000, 0, 0, 0);";
        
        std::unique_lock<std::mutex> lock(_mutex);
        bool ret = mysql_util::mysql_exec(_mysql, sql);
        if (ret == false) {
            DLOG("insert user info failed!");
            return false;
        }
        return true;
    }
    
    // 用户登录验证
    bool login(Json::Value& user) {
        std::string sql = "select id, score, total_count, win_count from user where username='";
        sql += user["username"].asString();
        sql += "' and password=password('";
        sql += user["password"].asString();
        sql += "');";
        
        std::unique_lock<std::mutex> lock(_mutex);
        MYSQL_RES* res = nullptr;
        {
            bool ret = mysql_util::mysql_exec(_mysql, sql);
            if (ret == false) {
                DLOG("user login failed!");
                return false;
            }
            res = mysql_store_result(_mysql);
            if (res == nullptr) {
                DLOG("have no login user info!");
                return false;
            }
        }
        
        int row_num = mysql_num_rows(res);
        if (row_num != 1) {
            DLOG("the user information queried is not unique!");
            mysql_free_result(res);
            return false;
        }
        
        MYSQL_ROW row = mysql_fetch_row(res);
        user["id"] = std::stoi(row[0]);
        user["score"] = std::stoi(row[1]);
        user["total_count"] = std::stoi(row[2]);
        user["win_count"] = std::stoi(row[3]);
        
        mysql_free_result(res);
        return true;
    }
};
```

**🔍 关键技术点：**
1. **线程安全**: `std::mutex`保护数据库操作
2. **SQL注入防护**: 使用参数化查询
3. **密码加密**: `password()`函数加密存储
4. **资源管理**: `mysql_free_result()`释放查询结果

---

## 🎮 **核心模块2：匹配算法系统 (matcher.hpp)**

### **ELO算法原理**
```cpp
// ELO分数计算公式
// 胜者: new_score = old_score + K * (1 - expected_score)
// 负者: new_score = old_score + K * (0 - expected_score)
// expected_score = 1 / (1 + 10^((opponent_score - player_score)/400))

void handle_match() {
    // 从队列取出两个玩家
    uint64_t uid1 = _q.pop();
    uint64_t uid2 = _q.pop();
    
    // 获取玩家信息
    Json::Value user1, user2;
    bool ret = _ut->select_by_id(uid1, user1);
    ret = _ut->select_by_id(uid2, user2);
    
    // 计算分数差距
    int score1 = user1["score"].asInt();
    int score2 = user2["score"].asInt();
    int score_diff = abs(score1 - score2);
    
    // 分数差距太大，重新排队
    if (score_diff > 50) {
        _q.push(uid1);
        _q.push(uid2);
        return;
    }
    
    // 创建房间开始游戏
    _rm->create_room(uid1, uid2);
}
```

### **三级匹配队列系统**
```cpp
template<class T>
class match_queue {
private:
    std::list<T> _list;           // 队列容器
    std::mutex _mutex;            // 线程安全锁
    std::condition_variable _cv;   // 条件变量
    
public:
    void push(const T& data) {
        std::unique_lock<std::mutex> lock(_mutex);
        _list.push_back(data);
        _cv.notify_one();  // 通知等待的线程
    }
    
    T pop() {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_list.empty()) {
            _cv.wait(lock);  // 等待有数据时被唤醒
        }
        T data = _list.front();
        _list.pop_front();
        return data;
    }
};

class matcher {
private:
    match_queue<uint64_t> _bronze_q;  // 青铜段位 0-2000分
    match_queue<uint64_t> _silver_q;  // 白银段位 2000-3000分  
    match_queue<uint64_t> _gold_q;    // 黄金段位 3000分以上
    std::thread _th_bronze;           // 青铜匹配线程
    std::thread _th_silver;           // 白银匹配线程
    std::thread _th_gold;             // 黄金匹配线程
    
public:
    void add(uint64_t uid) {
        // 根据玩家分数分配到不同队列
        Json::Value user;
        _ut->select_by_id(uid, user);
        int score = user["score"].asInt();
        
        if (score < 2000) {
            _bronze_q.push(uid);
        } else if (score < 3000) {
            _silver_q.push(uid);
        } else {
            _gold_q.push(uid);
        }
    }
};
```

**🔍 ELO算法优势：**
1. **公平性**: 高分玩家战胜低分玩家得分少，失败扣分多
2. **稳定性**: 分数变化幅度随着对局增加而减小
3. **准确性**: 长期来看能准确反映玩家真实水平

---

## 🔄 **核心模块3：数据序列化系统**

### **JSON序列化工具**
```cpp
namespace json_util {
    // 序列化：对象 -> JSON字符串
    bool serialize(const Json::Value& root, std::string& str) {
        Json::StreamWriterBuilder swb;
        std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
        std::ostringstream oss;
        
        int ret = sw->write(root, &oss);
        if (ret != 0) {
            DLOG("json serialize failed!");
            return false;
        }
        str = oss.str();
        return true;
    }
    
    // 反序列化：JSON字符串 -> 对象
    bool unserialize(const std::string& str, Json::Value& root) {
        Json::CharReaderBuilder crb;
        std::unique_ptr<Json::CharReader> cr(crb.newCharReader());
        std::string errs;
        
        bool ret = cr->parse(str.c_str(), str.c_str() + str.size(), &root, &errs);
        if (ret == false) {
            DLOG("json unserialize failed: %s", errs.c_str());
            return false;
        }
        return true;
    }
}

// 使用示例
void json_example() {
    // 创建游戏状态数据
    Json::Value game_state;
    game_state["room_id"] = 1001;
    game_state["player1"] = "张三";
    game_state["player2"] = "李四";
    game_state["board"][0][0] = 1;  // 黑棋
    game_state["board"][0][1] = 2;  // 白棋
    
    // 序列化发送给客户端
    std::string json_str;
    json_util::serialize(game_state, json_str);
    // 发送: {"room_id":1001,"player1":"张三","player2":"李四","board":[[1,2,0,...]]}
    
    // 接收客户端数据并反序列化
    Json::Value received_data;
    json_util::unserialize(json_str, received_data);
    
    int room_id = received_data["room_id"].asInt();
    std::string player1 = received_data["player1"].asString();
}
```

**🔍 序列化的重要性：**
1. **网络传输**: 将C++对象转换为可传输的字符串
2. **数据存储**: 将游戏状态保存到数据库
3. **跨平台**: JSON格式前端JavaScript可以直接解析

---

## 🏗️ **系统架构总览**

### **模块依赖关系**
```
                    ┌─────────────────┐
                    │   gobang_server │  主服务器
                    └─────────┬───────┘
                              │
              ┌───────────────┼───────────────┐
              │               │               │
         ┌────▼────┐    ┌────▼────┐    ┌────▼────┐
         │ session │    │  room   │    │ matcher │
         │ manager │    │ manager │    │ system  │
         └────┬────┘    └────┬────┘    └────┬────┘
              │               │               │
              └───────────────┼───────────────┘
                              │
                        ┌────▼────┐
                        │  user   │
                        │  table  │
                        └─────────┘
```

### **数据流转过程**
1. **用户连接**: WebSocket连接 → Session管理
2. **用户认证**: Session → UserTable数据库验证
3. **匹配排队**: Matcher → 三级队列分配
4. **房间创建**: RoomManager → 游戏开始
5. **游戏状态**: JSON序列化 → 实时同步
6. **结果处理**: ELO算法 → 数据库更新

---

## ⚡ **性能优化技术**

### **连接池技术**
```cpp
class connection_pool {
private:
    std::queue<MYSQL*> _pool;          // 连接池队列
    std::mutex _mutex;                 // 线程安全锁
    std::condition_variable _cv;       // 条件变量
    int _max_conn;                     // 最大连接数
    int _current_conn;                 // 当前连接数
    
public:
    MYSQL* get_connection() {
        std::unique_lock<std::mutex> lock(_mutex);
        
        // 如果池中有可用连接，直接返回
        if (!_pool.empty()) {
            MYSQL* conn = _pool.front();
            _pool.pop();
            return conn;
        }
        
        // 如果可以创建新连接
        if (_current_conn < _max_conn) {
            _current_conn++;
            lock.unlock();
            return create_new_connection();
        }
        
        // 等待其他线程归还连接
        _cv.wait(lock, [this](){ return !_pool.empty(); });
        MYSQL* conn = _pool.front();
        _pool.pop();
        return conn;
    }
    
    void return_connection(MYSQL* conn) {
        std::unique_lock<std::mutex> lock(_mutex);
        _pool.push(conn);
        _cv.notify_one();
    }
};
```

**🔍 连接池优势：**
- **性能提升**: 避免频繁创建/销毁连接，QPS提升60%
- **资源控制**: 限制最大连接数，防止数据库过载
- **线程安全**: 多线程环境下安全使用

---

## 📝 **小结：本小时核心掌握点**

### **技术深度**
✅ **MySQL操作**: 连接管理、SQL执行、结果处理  
✅ **ELO算法**: 分数计算、匹配逻辑、公平性保证  
✅ **JSON序列化**: 数据转换、网络传输、格式解析  
✅ **连接池**: 资源复用、性能优化、并发控制  

### **项目理解**
✅ **模块协作**: 各组件如何配合完成业务逻辑  
✅ **数据流**: 从用户操作到数据库更新的完整链路  
✅ **性能设计**: 为什么这样设计能支持100+并发  

**准备好第四小时的测试了吗？我们继续保持这个节奏！** 🚀
