#include<iostream>
#include<string>
#include<websocketpp/server.hpp>
#include<websocketpp/config/asio_no_tls.hpp>
typedef websocketpp::server<websocketpp::config::asio> wsserver_t;
void print(const std::string &str){
    std::cout << str << std::endl;
}
void http_callback(wsserver_t *srv,websocketpp::connection_hdl hdl){
    //给客户端返回一个hello world的页面
    wsserver_t::connection_ptr conn=srv->get_con_from_hdl(hdl);//获取连接,conn是一个智能指针
    /*std::cout << "body: " << conn->get_request_body() << std::endl;//获取请求的body
    websocketpp::http::parser::request req=conn->get_request();
    std::cout << "method:" << req.get_method() << std::endl;
    std::cout << "uri:" << req.get_uri() << std::endl;

    std::string body="<html><body><h1>hello world</h1></body></html>";//返回的html页面
    conn->set_body(body);//设置返回的body
    conn->append_header("Content-Type","text/html");//设置返回的header,Content-Type表示返回的内容类型
    conn->set_status(websocketpp::http::status_code::ok);//设置返回的状态码*/
    // 关键修改：仅处理非 WebSocket 升级的普通 HTTP 请求
    if (conn->get_request_header("Upgrade") != "websocket") {
        std::cout << "body: " << conn->get_request_body() << std::endl;
        websocketpp::http::parser::request req = conn->get_request();
        std::cout << "method:" << req.get_method() << std::endl;
        std::cout << "uri:" << req.get_uri() << std::endl;

        std::string body = "<html><body><h1>hello world</h1></body></html>";
        conn->set_body(body);
        conn->append_header("Content-Type", "text/html charset=utf-8");
        conn->set_status(websocketpp::http::status_code::ok);
        wsserver_t::timer_ptr tp =srv->set_timer(5000,std::bind(print,"ok"));//设置一个定时器，std::bind绑定可调用对象与参数
        tp->cancel();//定时任务的取消，并不是真正的取消任务，而是会使定时任务被立即执行。
    }
}
void wsopen_callback(wsserver_t *srv,websocketpp::connection_hdl hdl){
    std::cout << "websocket握手成功!!" ;
}
void wsclose_callback(wsserver_t *srv,websocketpp::connection_hdl hdl){
    std::cout << "websocket连接断开!!" ;
}
void wsmsg_callback(wsserver_t *srv,websocketpp::connection_hdl hdl,wsserver_t::message_ptr msg){
    wsserver_t::connection_ptr conn=srv->get_con_from_hdl(hdl);//获取连接
    std::cout << "wsmsg:" << msg->get_payload() << std::endl;//打印消息
    std::string rsp="client say: "+msg->get_payload();//返回消息
    conn->send(rsp,websocketpp::frame::opcode::text);//发送消息,opcode::text表示文本消息,opcode::binary表示二进制消息
}


int main()
{
   //1.实例化server对象
   wsserver_t wssrv;
   //2.设置日志级别
   wssrv.set_access_channels(websocketpp::log::alevel::none);
   //3.初始化asio调度器
   wssrv.init_asio();
   wssrv.set_reuse_addr(true);
   //4.设置回调函数
   wssrv.set_http_handler(std::bind(http_callback,&wssrv,std::placeholders::_1));//设置http回调函数,当有http请求时调用
   wssrv.set_open_handler(std::bind(wsopen_callback,&wssrv,std::placeholders::_1));//设置websocket连接打开时的回调函数
   wssrv.set_close_handler(std::bind(wsclose_callback,&wssrv,std::placeholders::_1)); //设置websocket连接关闭时的回调函数
   wssrv.set_message_handler(std::bind(wsmsg_callback,&wssrv,std::placeholders::_1,std::placeholders::_2));//设置websocket消息处理回调函数
   //5.设置监听端口
   wssrv.listen(8085);
   //6.开始获取新连接
   wssrv.start_accept();
   //7.启动服务器
   wssrv.run();

    return 0;
}