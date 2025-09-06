# 第3小时综合技术检测

## 🎯 测试说明

**涵盖技术栈：**
- Session会话管理机制
- WebSocket实时通信原理  
- 智能指针实际应用
- 代码理解与分析能力
- C++现代特性运用

**难度设计：**
- 题目1-3：基础概念理解 ⭐
- 题目4-6：技术应用分析 ⭐⭐  
- 题目7-8：代码实现能力 ⭐⭐⭐
- 题目9-10：综合场景应用 ⭐⭐⭐⭐

**答题要求：**
- 请在每题后直接写出您的答案
- 不确定的地方请标注"不确定"
- 我会在您完成所有题目后进行详细分析

---

## 📝 题目开始

### 题目1：基础概念理解 ⭐

下面哪个说法是**错误**的？

A. `session_manager _sm` 中，`_sm`是Session管理器的实例  
B. `LOGIN`是`ss_statu`枚举中的值，数值为1  
C. `session_ptr`等价于`std::shared_ptr<session>`  
D. WebSocket连接建立后就永远不会断开

**您的答案：**
D
---

### 题目2：智能指针机制 ⭐

以下代码中，Session对象的引用计数最终是多少？

```cpp
session_ptr ssp = _sm.create_session(uid, LOGIN);  // 创建Session
_session.insert(std::make_pair(ssid, ssp));        // 插入容器
return ssp;                                        // 返回给调用者
// 函数结束，局部变量ssp销毁
```

A. 1  
B. 2  
C. 3  
D. 0

**您的答案：**
B
---

### 题目3：状态机理解 ⭐

Session的状态转换中，哪种情况**不存在**？

A. UNLOGIN → LOGIN (用户登录成功)  
B. LOGIN → UNLOGIN (用户主动退出)  
C. LOGIN → LOGIN (重复登录)  
D. UNLOGIN → UNLOGIN (保持未登录)

**您的答案：**
C
---

### 题目4：技术选择分析 ⭐⭐

项目选择`std::unordered_map`而不是`std::map`来存储Session的主要原因是：

A. unordered_map占用内存更少  
B. unordered_map支持更多数据类型  
C. unordered_map提供O(1)平均查找时间，而map是O(log n)  
D. unordered_map线程安全，而map不是

**您的答案：**
c
---

### 题目5：WebSocket vs HTTP ⭐⭐

以下关于WebSocket和HTTP的对比，哪个是**正确**的？

A. HTTP支持双向通信，WebSocket只支持单向通信  
B. WebSocket需要每次重新建立连接，HTTP保持长连接  
C. HTTP是有状态协议，WebSocket是无状态协议  
D. WebSocket建立连接后可以双向通信，HTTP是请求-响应模式

**您的答案：**
d
---

### 题目6：定时器状态转换 ⭐⭐

在Session定时器的四种状态转换中，需要**异步重新添加Session**的情况有几种？

A. 1种  
B. 2种  
C. 3种  
D. 4种

**您的答案：**
B
---

### 题目7：代码实现分析 ⭐⭐⭐

以下代码有什么潜在问题？请指出并说明原因。

```cpp
void handle_user_action(uint64_t ssid) {
    session_ptr ssp = session_mgr->get_session_by_ssid(ssid);
    
    if (ssp->is_login()) {  // 直接使用ssp
        // 处理用户操作
        process_user_action(ssp->get_user());
    }
}
```

**您的答案：**
不会
---

### 题目8：Lambda表达式应用 ⭐⭐⭐

将以下`std::bind`写法改写为等价的Lambda表达式：

```cpp
_server->set_timer(5000, std::bind(&session_manager::remove_session, this, ssid));
```

**您的答案：**
_server->set_timer(5000,[](this,ssid){
    this->remove_session;
})
---

### 题目9：综合场景应用 ⭐⭐⭐⭐

用户A正在游戏房间中，突然网络断开，但很快重新连接。请分析：

1. 断网时Session会发生什么？
2. 重连时应该如何处理？
3. 如何避免用户游戏进度丢失？

**您的答案：**
1.session不会被删除，
2.
3.
---

### 题目10：系统设计思考 ⭐⭐⭐⭐

假设游戏服务器需要支持10万并发用户，当前的Session管理设计可能遇到什么性能瓶颈？请提出3个优化方案。

**您的答案：**
并发性不足，

---

## 🎯 答题完成提交

请确认您已完成所有10道题目，然后告诉我"答题完成"，我将为您进行详细的答案分析和技能评估！

**记住：答题过程中遇到不确定的地方，请诚实标注，这有助于我为您提供更精准的学习建议！** 💪

