#ifndef __M_ONLINE_H__
#define __M_ONLINE_H__
#include "util.hpp"
#include <mutex>
#include <unordered_map>

class online_manager{
   private:
        std::mutex _mutex;
        //用于建立游戏大厅用户的用户ID与通信连接的关系
        std::unordered_map<uint64_t,  wsserver_t::connection_ptr>  _hall_user;//unordered_map是一个哈希表，key是用户ID，value是通信连接
        //用于建立游戏房间用户的用户ID与通信连接的关系
        std::unordered_map<uint64_t,  wsserver_t::connection_ptr>  _room_user;
   public:
        //websocket连接建立的时候才会加入游戏大厅&游戏房间在线用户管理
        void enter_game_hall(uint64_t uid,   wsserver_t::connection_ptr &conn) {
            std::unique_lock<std::mutex> lock(_mutex);//加锁,自动锁定和解锁
            _hall_user.insert(std::make_pair(uid, conn));//make_pair函数用于将两个值组合成一个pair对象,key是uid，value是conn
        }
        void enter_game_room(uint64_t uid,   wsserver_t::connection_ptr &conn) {
            std::unique_lock<std::mutex> lock(_mutex);
            _room_user.insert(std::make_pair(uid, conn));
        }
        //websocket连接断开的时候，才会移除游戏大厅&游戏房间在线用户管理
        void exit_game_hall(uint64_t uid) {
            std::unique_lock<std::mutex> lock(_mutex);
            _hall_user.erase(uid);//erase函数用于删除指定的元素
        }
        void exit_game_room(uint64_t uid) {
            std::unique_lock<std::mutex> lock(_mutex);
            _room_user.erase(uid);
        }
        //判断当前指定用户是否在游戏大厅/游戏房间
        bool is_in_game_hall(uint64_t uid) {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _hall_user.find(uid);//find函数用于查找指定的元素,it是一个迭代器，指向查找的元素,it初始值为hall_user的开始位置
            //如果没有找到指定的元素，返回false
            if (it == _hall_user.end()) {
                return false;
            }
            return true;
        }
        bool is_in_game_room(uint64_t uid) {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _room_user.find(uid);
            if (it == _room_user.end()) {
                return false;
            }
            return true;
        }
        //通过用户ID在游戏大厅/游戏房间用户管理中获取对应的通信连接
        wsserver_t::connection_ptr get_conn_from_hall(uint64_t uid) {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _hall_user.find(uid);
            if (it == _hall_user.end()) {//如果没有找到指定的元素，返回一个空的连接
                return wsserver_t::connection_ptr();//返回一个空的连接
            }
            return it->second;//it->second表示迭代器指向的元素的第二个值,即conn
        }
        wsserver_t::connection_ptr get_conn_from_room(uint64_t uid) {
            std::unique_lock<std::mutex> lock(_mutex);
            auto it = _room_user.find(uid);
            if (it == _room_user.end()) {
                return wsserver_t::connection_ptr();
            }
            return it->second;
        }
};

#endif