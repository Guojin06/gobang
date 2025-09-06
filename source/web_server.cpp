#include "server.hpp"

#define HOST "127.0.0.1"    // MySQL主机
#define PORT 3306           // MySQL端口
#define USER "root"         // MySQL用户名  
#define PASS "Gh12345."     // MySQL密码
#define DBNAME "gobang"     // 数据库名
#define WEB_PORT 8085       // Web服务器端口

int main() {
    DLOG("=== 五子棋在线对战平台启动中 ===");
    DLOG("MySQL配置: %s:%d 数据库:%s", HOST, PORT, DBNAME);
    DLOG("Web服务端口: %d", WEB_PORT);
    DLOG("");
    DLOG("启动完成后访问地址:");
    DLOG("本地访问: http://localhost:%d", WEB_PORT);
    DLOG("云服务器访问: http://您的公网IP:%d", WEB_PORT);
    DLOG("请确保云服务器已开放端口 %d", WEB_PORT);
    DLOG("================================");
    
    try {
        // 创建服务器实例
        gobang_server server(HOST, USER, PASS, DBNAME, PORT);
        
        // 启动Web服务器（这里会阻塞）
        DLOG("服务器正在启动...");
        server.start(WEB_PORT);
        
    } catch (const std::exception& e) {
        ELOG("服务器启动失败: %s", e.what());
        return -1;
    }
    
    return 0;
}
