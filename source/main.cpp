#include "server.hpp"

#define HOST "127.0.0.1"
#define PORT 3306        // MySQL端口
#define USER "root"
#define PASS "Gh12345."
#define DBNAME "gobang"
#define WEB_PORT 8085    // Web服务器端口

int main() {
    // 创建服务器实例
    gobang_server server(HOST, USER, PASS, DBNAME, PORT);
    
    // 启动Web服务器
    DLOG("五子棋服务器启动中...");
    DLOG("访问地址: http://localhost:%d", WEB_PORT);
    DLOG("如果在云服务器，请访问: http://您的公网IP:%d", WEB_PORT);
    
    server.start(WEB_PORT);
    
    return 0;
}
