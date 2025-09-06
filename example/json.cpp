#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<jsoncpp/json/json.h>

//使用jsoncpp库对多个数对数据对象进行序列化
std::string serialize(){
    //1.将需要进行序列化的数据存储在Json::Value对象中
    Json::Value root;
    root["姓名"]="小明";
    root["年龄"]=20;
    root["成绩"].append(90);
    root["成绩"].append(80);
    root["成绩"].append(70);//数组,存储多个数对数据,数组中的元素可以是任意类型
    //2.实例化一个StreamWriterBuilder工厂类对象并设置属性
    Json::StreamWriterBuilder swb;
    //关键设置：禁用Unicode转义
    swb.settings_["emitUTF8"]=true;//直接输出UTF-8字符
    swb.settings_["indentation"]="    ";//保持美观缩进
    //3.通过StreamWriterBuilder工厂类对象生产一个StreamWriter对象
    Json::StreamWriter *sw=swb.newStreamWriter();//创建一个StreamWriter对象,用于序列化,StreamWriter对象是一个抽象类,不能直接实例化,只能通过工厂类创建
    //4.使用StreamWriter对象，对Json::Value中存储的数据进行序列化
    std::stringstream ss;//用于存储序列化后的数据,ss是一个内存流对象
    // 关键修改1: StreamWriter::write() 无返回值，删除错误的条件判断
    sw->write(root, &ss);  // 直接写入数据，无需检查返回值

    /*int ret=sw->write(root,&ss);//序列化
    if(ret!=0){
        std::cout<<"序列化失败"<<std::endl;
        return;
    }*///Json::StreamWriter::write() 函数返回类型为 void，原代码中 if (ret != 0) 的条件判断是冗余的，直接删除即可。
    std::cout<<ss.str()<<std::endl;
    delete sw;//释放资源
    return ss.str();//返回序列化后的字符串
}

void unserialize(const std::string &str)//反序列化，将json字符串转换为Json::Value对象
{
    
   //1.实例化一个CharReaderBuilder工厂类
   Json::CharReaderBuilder crb;
   //2.使用CharReaderBuilder工厂类对象创建一个CharReader对象
   Json::CharReader *cr=crb.newCharReader();//创建一个CharReader对象，用于反序列化  
   //3.定义一个Json::Value对象，用于存储反序列化后的数据
   Json::Value root;
   //4.使用CharReader对象，对json字符串进行反序列化
   std::string errs;//用于存储错误信息
   //parse(char *start,char *end,Json::Value *val， string *errs),第一个参数是json字符串的起始地址，第二个参数是json字符串的结束地址，第三个参数是用于存储反序列化后的数据，第四个参数是用于存储错误信息
   bool ret =cr->parse(str.c_str(),str.c_str()+str.size(),&root,&errs);//反序列化
   if(ret==false){
       std::cout<<"json unserialize failed: "<< errs <<std::endl;//打印错误信息
       return;
   }
   //5.逐个元素去访问Json::Value对象中的数据
   std::cout<<"姓名:"<<root["姓名"].asString()<<std::endl;
   std::cout<<"年龄:"<<root["年龄"].asInt()<<std::endl;
   int sz=root["成绩"].size();//获取数组的大小
    for(int i=0;i<sz;i++){
         std::cout<<"成绩:"<<root["成绩"][i].asFloat()<<std::endl;//root["成绩"][i]表示数组中的第i个元素,类型是Json::Value
    }
    delete cr;//释放资源，为了防止内存泄漏
    return;
}

int main(){
   std::string str=serialize();//调用序列化函数,返回序列化后的字符串
    unserialize(str);//调用反序列化函数

   
return 0;
}

