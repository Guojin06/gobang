# 五子棋项目实践学习指南

## 1. 项目架构理解（1-2天）
### 1.1 整体架构
- 客户端-服务器架构
- WebSocket通信
- 数据库存储
- 会话管理

### 1.2 核心模块
1. 会话管理（Session）
2. 房间系统（Room）
3. 数据库操作（Database）
4. 在线状态（Online）
5. 工具类（Util）

## 2. 技术栈学习（2-3天）
### 2.1 WebSocket
```cpp
// WebSocket连接示例
websocketpp::server<websocketpp::config::asio> server;

server.set_access_channels(websocketpp::log::alevel::all);
server.clear_access_channels(websocketpp::log::alevel::frame_payload);

server.init_asio();
```

### 2.2 MySQL
```cpp
// 数据库连接示例
MYSQL* conn = mysql_init(NULL);
mysql_real_connect(conn, "localhost", "user", "password", "database", 0, NULL, 0);
```

### 2.3 JSON处理
```cpp
// JSON序列化示例
json j;
j["name"] = "player1";
j["score"] = 100;
string json_str = j.dump();
```

## 3. 项目模块学习（每个模块2-3天）
### 3.1 会话管理
- WebSocket连接建立
- 消息处理
- 会话状态维护

### 3.2 房间系统
- 房间创建和销毁
- 玩家匹配
- 游戏状态同步

### 3.3 数据库操作
- 用户数据存储
- 游戏记录管理
- 数据查询优化

## 4. 实战练习（每天）
### 4.1 每日任务
1. 阅读并理解一个模块的代码
2. 编写测试用例
3. 实现一个小功能
4. 记录学习笔记

### 4.2 代码示例
```cpp
// 房间创建示例
class GameRoom {
private:
    int roomId;
    vector<shared_ptr<Player>> players;
    
public:
    GameRoom(int id) : roomId(id) {}
    
    void addPlayer(shared_ptr<Player> player) {
        players.push_back(player);
    }
};
```

## 5. 面试准备（持续进行）
### 5.1 技术要点
1. C++基础语法
2. 网络编程
3. 数据库操作
4. 设计模式
5. 项目架构

### 5.2 项目亮点
1. 双通道广播机制
2. 智能指针资源管理
3. 弱依赖架构设计
4. 完整的日志系统

## 学习计划
### 第一周
- 周一：项目架构学习
- 周二：WebSocket基础
- 周三：数据库操作
- 周四：会话管理模块
- 周五：房间系统模块
- 周末：复习和练习

### 第二周
- 周一：在线状态管理
- 周二：工具类实现
- 周三：系统集成
- 周四：测试和调试
- 周五：性能优化
- 周末：项目完善

## 面试准备清单
1. 项目架构图
2. 核心功能说明
3. 技术难点解决方案
4. 性能优化措施
5. 项目亮点总结

## 每日学习建议
1. 早上：阅读文档和代码
2. 下午：编写和测试代码
3. 晚上：总结和复习
4. 周末：项目实践和面试准备

记住：
- 多写代码，多调试
- 及时记录问题和解决方案
- 保持代码整洁和规范
- 注重项目文档的维护 