#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 练习1：基本智能指针使用
void basic_smart_pointer() {
    // 创建一个unique_ptr - C++11方式
    std::unique_ptr<int> num(new int(42));  // 直接使用new
    std::cout << "num的值: " << *num << std::endl;

    // 创建一个shared_ptr - C++11方式
    std::shared_ptr<std::string> str(new std::string("Hello"));  // 直接使用new
    std::cout << "str的值: " << *str << std::endl;

    // 创建另一个shared_ptr指向同一个对象
    std::shared_ptr<std::string> str2 = str;
    std::cout << "str2的值: " << *str2 << std::endl;
    std::cout << "引用计数: " << str.use_count() << std::endl;
}

// 练习2：模拟项目中的连接管理
class Connection {
private:
    std::string id;
    bool isActive;

public:
    // 初始化列表语法说明：
    // 1. 在构造函数参数列表后面使用冒号(:)
    // 2. 每个成员变量用逗号分隔
    // 3. 格式为：成员变量名(初始值)
    // 4. 初始化列表在构造函数体执行前完成初始化
    Connection(const std::string& connId) : id(connId), isActive(true) {
        std::cout << "创建连接: " << id << std::endl;
    }

    ~Connection() {
        std::cout << "关闭连接: " << id << std::endl;
    }

    void sendMessage(const std::string& msg) {
        if (isActive) {
            std::cout << "发送消息到 " << id << ": " << msg << std::endl;
        }
    }
};

// 练习3：使用智能指针管理连接
void connection_management() {
    // 使用unique_ptr管理连接 - C++11方式
    std::unique_ptr<Connection> conn1(new Connection("conn1"));
    conn1->sendMessage("Hello");

    // 使用shared_ptr管理连接 - C++11方式
    std::shared_ptr<Connection> conn2(new Connection("conn2"));
    conn2->sendMessage("Hi");

    // 创建另一个shared_ptr指向同一个连接
    std::shared_ptr<Connection> conn3 = conn2;
    conn3->sendMessage("Hey");
}

// 练习4：模拟房间系统中的玩家管理
class Player {
private:
    std::string name;
    int score;

public:
    // 初始化列表示例：
    // name(playerName) - 使用参数初始化name成员
    // score(0) - 将score初始化为0
    Player(const std::string& playerName) : name(playerName), score(0) {
        std::cout << "玩家 " << name << " 加入游戏" << std::endl;
    }

    void updateScore(int points) {
        score += points;
        std::cout << name << " 的分数更新为: " << score << std::endl;
    }
};

void player_management() {
    // 使用vector存储玩家智能指针
    std::vector<std::shared_ptr<Player>> players;
    
    // 添加玩家 - C++11方式
    players.push_back(std::shared_ptr<Player>(new Player("玩家1")));
    players.push_back(std::shared_ptr<Player>(new Player("玩家2")));

    // 更新玩家分数
    players[0]->updateScore(10);
    players[1]->updateScore(5);
}

int main() {
    std::cout << "=== 练习1：基本智能指针使用 ===" << std::endl;
    basic_smart_pointer();

    std::cout << "\n=== 练习2：连接管理 ===" << std::endl;
    connection_management();

    std::cout << "\n=== 练习3：玩家管理 ===" << std::endl;
    player_management();

    return 0;
} 