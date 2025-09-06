#include "server.hpp"

#define HOST "127.0.0.1"
#define PORT 3306
#define USER "root"
#define PASS "Gh12345."
#define DBNAME "gobang"

void mysql_test(){
    MYSQL*mysql=mysql_util::mysql_create(HOST,USER,PASS,DBNAME,PORT);
    const char *sql="insert stu values(null,'小明',18,53,50,60)";
    bool ret=mysql_util::mysql_exec(mysql,sql);//执行sql语句
    if(ret==false)
    {
        return ;
    }
    mysql_util::mysql_destroy(mysql);
}
void json_test(){
    Json::Value root;
    std::string body;//用于存储序列化后的数据,定义body是一个字符串
    root["姓名"]="小明";
    root["年龄"]=20;
    root["成绩"].append(90);
    root["成绩"].append(80);
    root["成绩"].append(70);
    json_util::serialize(root ,body);
    DLOG("body:%s\n",body.c_str());

    Json::Value val;
    json_util::unserialize(body,val);
    std::cout<<"姓名:"<<val["姓名"].asString()<<std::endl;
    std::cout<<"年龄:"<<val["年龄"].asInt()<<std::endl;
    int sz=val["成绩"].size();//获取数组的大小
     for(int i=0;i<sz;i++){
          std::cout<<"成绩:"<<root["成绩"][i].asFloat()<<std::endl;//root["成绩"][i]表示数组中的第i个元素,类型是Json::Value
     }
}
void str_test(){
    std::string str="123,234,,,,345";
    std::vector<std::string> array;
    string_util::split(str,",",array);
    /*for(int i=0;i<array.size();i++){
        DLOG("array[%d]:%s\n",i,array[i].c_str());
    }*/
   for(auto s:array){
        DLOG("array:%s\n",s.c_str());//s,c_str()将字符串转换为c风格的字符串
    }
}
void file_test(){
    std::string filename ="./makefile";//文件名
    std::string body;
    file_util::read(filename,body);
    std::cout<<body<<std::endl;
}
void db_test()
{
    user_table ut(HOST, USER, PASS, DBNAME, PORT);
    Json::Value user;
    // user["username"] = "xiaoming";
    // user["password"] = "123123";

    //ut.insert(user);
    //bool ret = ut.select_by_name("xiaoming", user);//查询用户名为xiaoming的用户信息,ut是一个user_table对象,select_by_name是一个成员函数

    bool ret = ut.win(1);//更新用户的胜利次数

    // bool ret = ut.login(user);
    // if(ret == false){
    //     DLOG("LOGIN FAILED!");
    // }
    std::string body;//用于存储序列化后的数据,定义body是一个字符串
    json_util::serialize(user, body);//序列化,序列化后的数据存储在body中，序列化原因是为了将数据存储到数据库中
    std::cout << body << std::endl;
}
void online_test()
{
    online_manager om;
    wsserver_t::connection_ptr conn;//实例化一个连接对象，智能指针
    //conn是一个智能指针，指向一个连接对象
    uint64_t uid = 2;
    om.enter_game_room(uid, conn);
    if (om.is_in_game_room(uid)) {
        DLOG("IN GAME HALL");
    }else {
        DLOG("NOT IN GAME HALL");
    }
    om.exit_game_room(uid);
    if (om.is_in_game_room(uid)) {
        DLOG("IN GAME HALL");
    }else {
        DLOG("NOT IN GAME HALL");
    }

}
int main()
{
    gobang_server _server(HOST, USER, PASS, DBNAME, PORT);
    _server.start(8085);
    return 0;
}
