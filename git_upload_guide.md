# 📤 Git上传指南

## 🚀 第一次上传到GitHub

### 1. 初始化Git仓库
```bash
cd /root/my_project
git init
```

### 2. 添加远程仓库
```bash
git remote add origin https://github.com/Guojin06/gobang.git
```

### 3. 添加所有文件
```bash
git add .
```

### 4. 创建第一次提交
```bash
git commit -m "🎮 Initial commit: Online Gobang Game v1.0

✨ Features:
- Real-time WebSocket communication
- ELO rating matching system  
- Room management and user sessions
- MySQL connection pooling
- Thread-safe concurrent architecture

🔧 Tech Stack:
- C++11/14, WebSocket++, MySQL, JsonCpp
- Multi-threading, Smart pointers, STL containers

🎯 Performance:
- Support 100+ concurrent users
- <100ms response time
- 60% QPS improvement with connection pool"
```

### 5. 推送到GitHub
```bash
git branch -M main
git push -u origin main
```

## 🔄 后续更新流程

### 日常开发提交
```bash
# 1. 查看修改状态
git status

# 2. 添加修改的文件
git add <file_name>
# 或添加所有修改
git add .

# 3. 提交修改
git commit -m "描述你的修改"

# 4. 推送到远程
git push
```

## 📝 提交信息规范

### 提交类型
- `🎮 feat:` 新功能
- `🐛 fix:` 修复bug  
- `📝 docs:` 文档更新
- `🎨 style:` 代码格式调整
- `♻️ refactor:` 代码重构
- `⚡ perf:` 性能优化
- `✅ test:` 测试相关
- `🔧 chore:` 构建工具、辅助工具

### 示例提交信息
```bash
git commit -m "🎮 feat: Add AI opponent mode"
git commit -m "🐛 fix: Fix WebSocket connection memory leak"  
git commit -m "📝 docs: Update installation guide"
git commit -m "⚡ perf: Optimize database query performance"
```

## 🌟 创建Release版本

### 创建标签
```bash
# 创建版本标签
git tag -a v1.0.0 -m "Release version 1.0.0"

# 推送标签到远程
git push origin v1.0.0
```

### 在GitHub创建Release
1. 进入GitHub项目页面
2. 点击"Releases" -> "Create a new release"
3. 选择刚才创建的标签
4. 填写Release说明
5. 上传编译好的二进制文件（可选）

## 🔧 常用Git命令

### 查看状态和历史
```bash
git status              # 查看工作区状态
git log --oneline       # 查看提交历史
git diff               # 查看修改差异
git branch -a          # 查看所有分支
```

### 分支操作
```bash
git checkout -b feature/new-feature  # 创建并切换到新分支
git checkout main                    # 切换到主分支
git merge feature/new-feature        # 合并分支
git branch -d feature/new-feature    # 删除分支
```

### 撤销操作
```bash
git checkout -- <file>     # 撤销工作区修改
git reset HEAD <file>       # 撤销暂存区修改
git reset --hard HEAD~1     # 撤销最近一次提交
```

## 🚨 注意事项

### ⚠️ 敏感信息处理
在提交前确保删除或隐藏：
- 数据库密码
- API密钥
- 个人信息
- 临时文件

### 📁 .gitignore设置
已经为你创建了`.gitignore`文件，包含：
- 编译产物 (*.o, gobang)
- 日志文件 (*.log)
- 临时文件
- IDE配置文件

### 🔒 权限设置
确保GitHub仓库设置：
- ✅ Public（让HR和面试官能看到）
- ✅ 开启Issues（接收反馈）
- ✅ 开启Wiki（扩展文档）

## 🎯 提升项目可见性

### GitHub优化
1. **添加Topics标签**
   - cpp, websocket, game, mysql, realtime
2. **完善项目描述**
   - 简洁明了的一句话描述
3. **开启GitHub Pages**
   - 展示项目文档或在线demo

### 代码质量
1. **添加代码注释**
2. **编写单元测试**  
3. **使用GitHub Actions**（CI/CD）
4. **代码覆盖率报告**

记住：**好的Git历史就是你的开发简历！**
