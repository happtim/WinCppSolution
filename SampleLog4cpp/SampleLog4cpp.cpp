// SampleLog4cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <direct.h>

#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
#include <log4cpp/PropertyConfigurator.hh>


int main()
{       
	/*
	log4cpp.rootCategory=DEBUG, rootAppender             rootCategory要添加不然报错   

	log4cpp.appender.socket=RollingFileAppender          使用混动文件记录
	log4cpp.appender.socket.fileName=logs/current.log    文件名 注意此处需要初始化建好好logs目录
	log4cpp.appender.socket.maxFileSize=1024*1024*10=10M 单个文件最大
	log4cpp.appender.socket.maxBackupIndex=10            文件数量
	log4cpp.appender.socket.layout=PatternLayout         让用户根据类似于 C 语言 printf
	log4cpp.appender.socket.layout.ConversionPattern=%d [%p] %m%n  文件format

	日志记录级别
	DEBUG < INFO < WARN < ERROR < FATAL
	*/
	_mkdir("./logs"); //创建日志目录

	std::string initFileName = "log4cpp.properties";
	log4cpp::PropertyConfigurator::configure(initFileName);

	// Category第一可以有继承.第二可以设备日志级别.
	log4cpp::Category& socket = log4cpp::Category::getInstance(std::string("socket"));
	log4cpp::Category& running = log4cpp::Category::getInstance(std::string("running"));
	log4cpp::Category& spline = log4cpp::Category::getInstance(std::string("spline"));
	log4cpp::Category& operation = log4cpp::Category::getInstance(std::string("running.operation"));

	//socket 日志级别debug 都能打印出来
	socket.debug("connect to server");
	socket.info("receive message from server");
	socket.info("connect closed");

	//running 日志级别 INFO 打印两个, 又因为继承running 打印了一条running的. 
	running.debug("Running Hiding solar panels");
	running.error("Running Solar panels are blocked");
	running.debug("Running Applying protective shield");
	running.warn("Running Unfolding protective shield");
	running.info("Running Solar panels are shielded");

	//spline 日志级别 INFO 打印3个
	spline.debug("Spline Hiding solar panels");
	spline.error("Spline Solar panels are blocked");
	spline.debug("Spline Applying protective shield");
	spline.warn("Spline Unfolding protective shield");
	spline.info("Spline Solar panels are shielded");

	//operation 日志级别 ERROR 打印一个. 又因为继承running 也在running中打印一条相同的.
	operation.debug("Operation Hiding solar panels");
	operation.error("Operation Solar panels are blocked");
	operation.debug("Operation Applying protective shield");
	operation.warn("Operation Unfolding protective shield");
	operation.info("Operation Solar panels are shielded");

	log4cpp::Category::shutdown();

	return 0;
}
