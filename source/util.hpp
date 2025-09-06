#ifndef __MYSQL_UTIL_HPP__
#define __MYSQL_UTIL_HPP__
#include "logger.hpp"
#include <iostream>
#include <string>
#include <mysql/mysql.h>
#include <memory>
#include <jsoncpp/json/json.h>
#include <sstream>
#include <vector>
#include <fstream>
#include<websocketpp/server.hpp>
#include<websocketpp/config/asio_no_tls.hpp>

typedef websocketpp::server<websocketpp::config::asio> wsserver_t;//

class mysql_util{
    public:
       static inline MYSQL*mysql_create(const std::string &host,
        const std::string &username,
        const std::string &password,
        const std::string &dbname,
        uint16_t port=3306){
              //1.初始化mysql句柄
             MYSQL*mysql = mysql_init(NULL);//null表示使用默认的配置，这个指针是一个MYSQL类型的指针，指向mysql的句柄
                if(mysql == NULL)
                {
                    ELOG("mysql init error\n");//日志输出
                    return NULL;//表示程序异常退出
                }
                //2.连接mysql
                //MYsQL*mysql_real_connect(MYSQL*mysql,const char*host,const char*user,const char*passwd,const char*db,unsigned int port,const char*unix_socket,unsigned long client_flag);
                if(mysql_real_connect(mysql,host.c_str(),username.c_str(),password.c_str(),dbname.c_str(),port,NULL,0)==NULL)
                {
                    ELOG("connect mysql server failed: %s\n",mysql_error(mysql));
                    mysql_close(mysql);//关闭连接
                    return NULL;
                }
                //3.设置客户端字符集
                // int mysql_ser_character_set(MYSQL*mysql,const char*csname);
                if(mysql_set_character_set(mysql,"utf8")!=0)
                {
                    ELOG("set character error: %s\n",mysql_error(mysql));
                    mysql_close(mysql);
                    return NULL;
                }
                //4.选择要操作的数据库
                //int mysql_select_db(MYSQL*mysql,const char*db);//选择数据库,int表示成功返回0，失败返回非0
                //mysql_select_db(mysql,DBNAME);//选择数据库,这里不需要选择数据库，因为连接的时候已经选择了
                return mysql;//返回mysql,返回的是一个指针
        };//默认端口号,是谁的端口号，mysql的端口号
        static bool mysql_exec(MYSQL*mysql,const std::string &sql){
            int ret = mysql_query(mysql,sql.c_str()); //执行sql语句,sql.c_str()表示将sql转换为c风格的字符串
            //mysql_query(MYSQL*mysql,const char*sql);int表示成功返回0，失败返回非0
            if(ret!=0){
                ELOG("%s\n",sql.c_str());//日志输出
                ELOG("query error: %s\n",mysql_error(mysql));
                mysql_close(mysql);
                return false;
            }
            return true;

        }//定义一个静态函数，用来执行sql语句，参数为mysql和sql语句
        static void mysql_destroy(MYSQL*mysql){
            if(mysql!=NULL){
                mysql_close(mysql);
            }
            return;
   
        }//定义一个静态函数，用来销毁mysql


};

class json_util{
    public:
        static bool serialize(const Json::Value &root, std::string&string){
                 //2.实例化一个StreamWriterBuilder工厂类对象并设置属性
                Json::StreamWriterBuilder swb;//StreamWriterBuilder是一个工厂类，用于创建StreamWriter对象
                //关键设置：禁用Unicode转义
                swb.settings_["emitUTF8"]=true;//直接输出UTF-8字符
                swb.settings_["indentation"]="    ";//保持美观缩进
                //3.通过StreamWriterBuilder工厂类对象生产一个StreamWriter对象
                //Json::StreamWriter *sw=swb.newStreamWriter();//创建一个StreamWriter对象,用于序列化,StreamWriter对象是一个抽象类,不能直接实例化,只能通过工厂类创建
                std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());//创建一个StreamWriter对象,用于序列化,StreamWriter对象是一个抽象类,不能直接实例化,只能只能通过工厂类创建
                //4.使用StreamWriter对象，对Json::Value中存储的数据进行序列化
                std::stringstream ss;//用于存储序列化后的数据,ss是一个内存流对象
                // 关键修改1: StreamWriter::write() 无返回值，删除错误的条件判断
                sw->write(root, &ss);  // 直接写入数据，无需检查返回值

                /*int ret=sw->write(root,&ss);//序列化
                if(ret!=0){
                    std::cout<<"序列化失败"<<std::endl;
                    return;
                }*///Json::StreamWriter::write() 函数返回类型为 void，原代码中 if (ret != 0) 的条件判断是冗余的，直接删除即可。
         //       std::cout<<ss.str()<<std::endl;//输出序列化后的数据
                string = ss.str();
                return true;

        }
        static bool unserialize(const std::string &str, Json::Value &root){//&str是一个引用，将其如果传递给函数，会将str的地址传递给函数，这样函数中对str的修改会影响到外部的str
                //1.实例化一个CharReaderBuilder工厂类
                Json::CharReaderBuilder crb;
                //2.使用CharReaderBuilder工厂类对象创建一个CharReader对象
               // Json::CharReader *cr=crb.newCharReader();//创建一个CharReader对象，用于反序列化  
                std::unique_ptr<Json::CharReader> cr(crb.newCharReader());//创建一个CharReader对象，用于反序列化
                //4.使用CharReader对象，对json字符串进行反序列化
                std::string err;//用于存储错误信息
                //parse(char *start,char *end,Json::Value *val， string *errs),第一个参数是json字符串的起始地址，第二个参数是json字符串的结束地址，第三个参数是用于存储反序列化后的数据，第四个参数是用于存储错误信息
                bool ret =cr->parse(str.c_str(),str.c_str()+str.size(),&root,&err);//反序列化,参数为json字符串的起始地址，json字符串的结束地址，用于存储反序列化后的数据，用于存储错误信息
                if(ret==false){
                    ELOG("json unserialize failed: %s", err.c_str());//打印错误信息
                    return false;
                }
                return true;
        }
};
//定义一个字符串工具类,实现字符串的分割，三个参数分别是源字符串，分隔符，存储分割后的结果
class string_util{
    public:
        static int  split(const std::string &src,const std::string &sep,std::vector<std::string>&res){
            size_t pos, idx=0;//123,234，，，345；pos表示分隔符的位置，idx表示源字符串的起始位置
            //std::string::npos表示没有找到分隔符
            while(idx<src.size()){//循环结束条件，idx大于等于src的大小
                pos = src.find(sep,idx);//查找分隔符的位置,find函数返回值是size_t类型，表示分隔符的位置 ，参数为分隔符的起始位置
                if (pos == std::string::npos){
                    //没有找到，字符串中间没有间隔字符了，就跳出循环
                    res.push_back(src.substr(idx));//res.push_back(src.substr(idx))表示把源字符串从idx开始到结束的部分添加到结果中
                    break;
                }
                if(pos==idx){//处理连续的分隔符
                    //如果分隔符的位置和起始位置相同，说明有连续的分隔符
                    //就把分隔符的位置加上分隔符的大小，继续查找下一个分隔符
                    idx+=sep.size();
                    continue;
                }
                res.push_back(src.substr(idx,pos-idx));////res.push_back(src.substr(idx,pos-idx))表示把源字符串从idx开始到pos-idx的部分添加到结果中
                idx = pos+sep.size();//继续查找下一个分隔符
                
            }
            return res.size();
        }
};      
class file_util{
    public:
       static bool read(const std::string &filename,std::string &body){
        //1.打开文件
        std::ifstream ifs(filename,std::ios::binary);
        if(ifs.is_open()==false){
            ELOG("%s file open failed!!",filename.c_str());
            return false;
        }
        //2.获取文件大小
        size_t fsize = 0;
        ifs.seekg(0,std::ios::end);
        fsize = ifs.tellg();
        ifs.seekg(0,std::ios::beg);
        body.resize(fsize);
        //3.读取文件内容
        ifs.read(&body[0],fsize);
        if(ifs.good()==false){
            ELOG("read %s file content failed!",filename.c_str());
            ifs.close();
            return false;
        }
        
        //4.关闭文件
        return true;
       } 
        
};
#endif