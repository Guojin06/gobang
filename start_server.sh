#!/bin/bash

echo "=== 五子棋在线对战平台启动脚本 ==="
echo ""

# 检查当前目录
if [ ! -f "source/server.hpp" ]; then
    echo "❌ 错误：请在项目根目录执行此脚本"
    exit 1
fi

# 进入源码目录
cd source

echo "📦 正在编译服务器..."
g++ -g -std=c++11 web_server.cpp -o web_server -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lstdc++ -ljsoncpp

if [ $? -eq 0 ]; then
    echo "✅ 编译成功！"
else
    echo "❌ 编译失败，请检查依赖库是否安装"
    echo ""
    echo "安装依赖命令："
    echo "sudo apt update"
    echo "sudo apt install libmysqlclient-dev libjsoncpp-dev"
    exit 1
fi

echo ""
echo "🚀 启动服务器..."
echo "📡 服务端口: 8085"
echo "🌐 访问地址: http://您的公网IP:8085"
echo "⚠️  请确保云服务器已开放端口 8085"
echo ""
echo "按 Ctrl+C 停止服务器"
echo "================================"

# 启动服务器
./web_server
