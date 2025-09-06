#include<stdio.h>
#include<string.h>
#include<mysql/mysql.h>
#define HOST "127.0.0.1"//主机地址
#define PORT 3306//端口
#define USER "root"//用户名
#define PASS "Gh12345."
#define DBNAME "gobang"
int main()
{
    //1.初始化mysql句柄
    MYSQL*mysql = mysql_init(NULL);//null表示使用默认的配置
    if(mysql == NULL)
    {
        printf("mysql init error\n");
        return -1;//表示程序异常退出
    }
    //2.连接mysql
    //MYsQL*mysql_real_connect(MYSQL*mysql,const char*host,const char*user,const char*passwd,const char*db,unsigned int port,const char*unix_socket,unsigned long client_flag);
    if(mysql_real_connect(mysql,HOST,USER,PASS,DBNAME,PORT,NULL,0)==NULL)
    {
        printf("connect mysql server failed: %s\n",mysql_error(mysql));
        mysql_close(mysql);//关闭连接
        return -1;
    }
    //3.设置客户端字符集
    // int mysql_ser_character_set(MYSQL*mysql,const char*csname);
    if(mysql_set_character_set(mysql,"utf8")!=0)
    {
        printf("set character error: %s\n",mysql_error(mysql));
        mysql_close(mysql);
        return -1;
    }
    //4.选择要操作的数据库
    //int mysql_select_db(MYSQL*mysql,const char*db);//选择数据库,int表示成功返回0，失败返回非0
    //mysql_select_db(mysql,DBNAME);//选择数据库,这里不需要选择数据库，因为连接的时候已经选择了
    //5.执行sql语句
    //int mysql_query(MYSQL*mysql,const char*stmt_str);//执行sql语句，成功返回0，失败返回非0
    //char *sql="insert stu values(null,'小明',18,53,50,60)";
    //char *sql="update stu set ch=ch+40 where sn=1";
    //char *sql="delete from stu  where sn=1";
    char *sql="select * from stu";
    int ret = mysql_query(mysql,sql);
    if(ret!=0){
        printf("query error: %s\n",mysql_error(mysql));
        mysql_close(mysql);
        return -1;
    }

    //6.如果sql语句是查询语句，则需要保存结果到本地
    //MYSQL_RES*mysql_store_result(MYSQL*mysql);//保存结果集到本地
    MYSQL_RES*res = mysql_store_result(mysql);
    if(res == NULL)
    {
        printf("store result error: %s\n",mysql_error(mysql));
        mysql_close(mysql);
        return -1;
    }
    //7.获取结果集中的结果条数
    //int mysql_num_rows(MYSQL_RES*result);//获取结果集中的结果条数
    //int mysql_num_fields(MYSQL_RES*result);//获取结果集中的字段的个数
    int num_row=mysql_num_rows(res);//获取结果集中的结果条数
    int num_col=mysql_num_fields(res);// 获取结果集中的字段的个数
    //8.遍历保存到本地的结果集
    for(int i=0;i<num_row;i++)
    {
        //获取一行数据
        //MYSQL_ROW mysql_fetch_row(MYSQL_RES*result);//获取一行数据
        MYSQL_ROW row = mysql_fetch_row(res);
        for(int j=0;j<num_col;j++)
        {
            printf("%s\t",row[j]);
        }
        printf("\n");
    }
    //9.释放结果集
    mysql_free_result(res);
    //10.关闭连接，释放句柄
    mysql_close(mysql);
    return 0;
   

}

