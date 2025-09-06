/*#ifndef _M__LOGGER_H_
#define _M__LOGGER_H_
#include<stdio.h>
#include<time.h>

#define INF 0
#define DBG 1
#define ERR 2
#define DEFAULT_LEVEL INF
#define LOG(level,format,...)do{\
    if(DEFAULT_LEVEL>level)break;\
    time_t t=time(NULL);\
    struct tm *lt-localtime(&t);\
    char buf[32]=0;\
    strftime(buf ,31,"%H:%M:%S",lt);\
    fprintf(stdout,"[%s %s %d]" format "\n",buf,__FILE__,__LINE__,##__VA_ARGS__);\//##__VA_ARGS__ is a gcc extension
} while(0)
#define ILOG(format,...) LOG(INF,format,##__VA_ARGS__)
#define DLOG(format,...) LOG(DBG,format,##__VA_ARGS__)
#define ELOG(format,...) LOG(ERR,format,##__VA_ARGS__)//##__VA_ARGS__ is a gcc extension, it is used to ignore the last comma when there is no arguments
#endif*/
#ifndef M_LOGGER_H  // 头文件守卫（避免使用双下划线开头）
#define M_LOGGER_H 
#include <stdio.h>
#include <time.h>
#include <string.h>  // 添加strftime函数依赖 
 
/* 日志级别定义 
 * INF: 信息级（最高优先级）
 * DBG: 调试级 
 * ERR: 错误级（最低优先级）
 * 数值越小优先级越高 */
#define INF 0 
#define DBG 1 
#define ERR 2 
 
/* 默认日志输出级别 
 * 仅显示高于等于该级别的日志 */
#define DEFAULT_LEVEL INF 
 
/* 核心日志宏 
 * 功能：根据级别过滤并格式化输出日志 
 * 特性：自动添加时间戳、文件名、行号 */
#define LOG(level, format, ...) do { \
    if (level < DEFAULT_LEVEL) break; /* 级别过滤 */ \
    time_t t = time(NULL); /* 获取当前时间戳 */ \
    struct tm *lt = localtime(&t); /* 转换为本地时间 */ \
    char buf[32] = {0}; \
    strftime(buf, 31, "%H:%M:%S", lt); /* 格式化时间 */ \
    fprintf(stdout, "[%s %s:%d] " format "\n", \
        buf, __FILE__, __LINE__, ##__VA_ARGS__); \
} while(0)
 
/* 各级别快捷调用宏 
 * 注：##__VA_ARGS__处理无参数时的逗号问题 */
#define ILOG(format, ...) LOG(INF, format, ##__VA_ARGS__)
#define DLOG(format, ...) LOG(DBG, format, ##__VA_ARGS__)
#define ELOG(format, ...) LOG(ERR, format, ##__VA_ARGS__)
 
#endif // M_LOGGER_H 