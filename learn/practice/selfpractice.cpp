// //练习1，Unique_ptr的基本操作
// #include <iostream>
// #include <memory>
// int a;
// class MyClass {
//     public:
//     std::unique_ptr<int> data=std::make_unique<int>(a=100);
//     MyClass() {
//         std::cout << "MyClass constructor called with value: " << *data << std::endl;
//     }
//     ~MyClass() {
//         std::cout << "MyClass destructor called" << std::endl;
//     }
// };
// int main(){
//     MyClass myclass;
//     std::unique_ptr<int>data2=std::move(myclass.data); 
//     std::cout << "data2的值: " << *data2 << std::endl;
//     return 0;
// }

//练习2，shared_ptr的引用计数
// #include <iostream>
// #include <memory>
// int main(){
//     std::shared_ptr<int>data=std::make_shared<int>(100);
//     std::shared_ptr<int>data2=data;
//     std::cout << "data的引用计数: " << data.use_count() << std::endl;
//     //当一个新的 shared_ptr 通过拷贝构造或拷贝赋值方式共享所有权时，引用计数 +1
//     std::cout << "data2的引用计数: " << data2.use_count() << std::endl;
//     //释放data2
//     data2.reset();
//     std::cout << "data的引用计数: " << data.use_count() << std::endl;
//     std::cout << "data2的引用计数: " << data2.use_count() << std::endl;
//     return 0;
// }


//练习3，智能指针与容器
// #include <iostream>
// #include <memory>
// #include <vector>
// class Resource{
//     public:
//     Resource(int id):id(id){
//         std::cout << "Resource " << id << " created" << std::endl;
//     }
//     ~Resource(){
//         std::cout << "Resource " << id << " destroyed" << std::endl;
//     }
//     int id;
// };
// int main(){
//     std::vector<std::unique_ptr<Resource>>resources;
//     resources.push_back(std::make_unique<Resource>(1));
//     resources.push_back(std::make_unique<Resource>(2));
//     resources.push_back(std::make_unique<Resource>(3));
//     return 0;
// }
// class Resource {
// public:
//     Resource() { std::cout << "Resource创建" << std::endl; }
//     ~Resource() { std::cout << "Resource销毁" << std::endl; }
// };

// void exception_safety_exercise() {
//     try {
//         std::unique_ptr<Resource> res(new Resource());
//         throw std::runtime_error("测试异常");//runtime_error是标准库中的异常类,用于表示运行时错误
//     } catch (const std::exception& e) {//std::exception是所有标准异常的基类,const 表示e的值不会被修改,
//     //&表示e是一个引用,e.what()返回异常的描述信息,设置成引用是为了避免拷贝构造函数
//         std::cout << "捕获到异常: " << e.what() << std::endl;
//     }
// };
// int main(){
//     exception_safety_exercise();
//     return 0;
// }


// //练习4：智能指针与容器
// #include <iostream>
// #include <memory>
// #include <vector>
// #include <string>
// #include <algorithm>

// class Player {
// private:
//     std::string name;  // 成员变量声明在类内部

// public:
//     // 构造函数
//     Player(const std::string& playerName) : name(playerName) {}

//     // 获取名字的方法
//     std::string getName() const { return name; }
// };

// // 查找玩家函数
// std::shared_ptr<Player> findPlayer(
//     const std::vector<std::shared_ptr<Player>>& players,
//     const std::string& name) {
//     for (const auto& player : players) {
//         if (player->getName() == name) {
//             return player;
//         }
//     }
//     return nullptr;
// }

// // 删除玩家函数
// void removePlayer(
//     std::vector<std::shared_ptr<Player>>& players,
//     const std::string& name) {
//     players.erase(
//         std::remove_if(players.begin(), players.end(),
//             [&name](const std::shared_ptr<Player>& p) {
//                 return p->getName() == name;
//             }),
//         players.end());
// }

// int main() {
//     // 创建玩家列表
//     std::vector<std::shared_ptr<Player>> players;
//     players.push_back(std::make_shared<Player>("张三"));
//     players.push_back(std::make_shared<Player>("李四"));
//     players.push_back(std::make_shared<Player>("王五"));

//     // 查找玩家
//     std::shared_ptr<Player> player = findPlayer(players, "张三");
//     if (player) {
//         std::cout << "找到玩家: " << player->getName() << std::endl;
//     }

//     // 删除玩家
//     removePlayer(players, "李四");

//     // 打印剩余玩家
//     for (const auto& player : players) {
//         std::cout << "玩家: " << player->getName() << std::endl;
//     }

//     return 0;
// }

//练习五：自定义删除器
#include <iostream>
#include <memory>
#include <vector>
#include <cstdio>  // 添加FILE类型支持
//文件资源类，管理文件资源
class FileResource {
public:
    // 构造函数：打开文件
    FileResource(const std::string& filename) {
        file = fopen(filename.c_str(), "w");
        if (!file) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
    }

    // 写入文件内容
    void write(const std::string& content) {
        if (file) {
            fputs(content.c_str(), file);
        }
    }

    // 获取文件指针（供删除器使用）
    FILE* getFile() const { return file; }

private:
    FILE* file;
};

// 自定义删除器，定义如何释放资源
struct FileDeleter {
    void operator()(FileResource* resource) {
        if (resource && resource->getFile()) {
            fclose(resource->getFile());
        }
        delete resource;
    }
};

void custom_deleter_exercise() {
    try {
        // 创建unique_ptr，使用自定义删除器
        std::unique_ptr<FileResource, FileDeleter> file(
            new FileResource("test.txt")
        );
        
        // 写入内容
        file->write("Hello, World!");
        
        // 文件会在unique_ptr销毁时自动关闭
        // 因为FileDeleter会处理文件的关闭
    } catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << std::endl;
    }
}

int main() {
    custom_deleter_exercise();
    return 0;
}