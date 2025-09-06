#!/bin/bash

echo "🎮 在线五子棋对战系统 - 快速启动脚本"
echo "========================================"

# 检查依赖
echo "📋 检查系统依赖..."
if ! command -v g++ &> /dev/null; then
    echo "❌ g++ 未安装，请先安装: sudo apt install g++"
    exit 1
fi

if ! command -v mysql &> /dev/null; then
    echo "❌ MySQL 未安装，请先安装: sudo apt install mysql-server"
    exit 1
fi

echo "✅ 系统依赖检查完成"

# 安装开发依赖
echo "📦 安装开发依赖..."
sudo apt update
sudo apt install -y libmysqlclient-dev libjsoncpp-dev libwebsocketpp-dev

# 编译项目
echo "🔨 编译项目..."
cd source
make clean
make

if [ $? -eq 0 ]; then
    echo "✅ 编译成功！"
else
    echo "❌ 编译失败，请检查错误信息"
    exit 1
fi

# 启动服务
echo "🚀 启动服务器..."
echo "服务器将在 http://localhost:8085 启动"
echo "测试账号: xiaoming / 123456"
echo "按 Ctrl+C 停止服务器"
echo "========================================"

./gobang
