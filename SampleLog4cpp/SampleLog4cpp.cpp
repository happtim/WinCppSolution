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
	log4cpp.rootCategory=DEBUG, rootAppender             rootCategoryҪ��Ӳ�Ȼ����   

	log4cpp.appender.socket=RollingFileAppender          ʹ�û춯�ļ���¼
	log4cpp.appender.socket.fileName=logs/current.log    �ļ��� ע��˴���Ҫ��ʼ�����ú�logsĿ¼
	log4cpp.appender.socket.maxFileSize=1024*1024*10=10M �����ļ����
	log4cpp.appender.socket.maxBackupIndex=10            �ļ�����
	log4cpp.appender.socket.layout=PatternLayout         ���û����������� C ���� printf
	log4cpp.appender.socket.layout.ConversionPattern=%d [%p] %m%n  �ļ�format

	��־��¼����
	DEBUG < INFO < WARN < ERROR < FATAL
	*/
	_mkdir("./logs"); //������־Ŀ¼

	std::string initFileName = "log4cpp.properties";
	log4cpp::PropertyConfigurator::configure(initFileName);

	// Category��һ�����м̳�.�ڶ������豸��־����.
	log4cpp::Category& socket = log4cpp::Category::getInstance(std::string("socket"));
	log4cpp::Category& running = log4cpp::Category::getInstance(std::string("running"));
	log4cpp::Category& spline = log4cpp::Category::getInstance(std::string("spline"));
	log4cpp::Category& operation = log4cpp::Category::getInstance(std::string("running.operation"));

	//socket ��־����debug ���ܴ�ӡ����
	socket.debug("connect to server");
	socket.info("receive message from server");
	socket.info("connect closed");

	//running ��־���� INFO ��ӡ����, ����Ϊ�̳�running ��ӡ��һ��running��. 
	running.debug("Running Hiding solar panels");
	running.error("Running Solar panels are blocked");
	running.debug("Running Applying protective shield");
	running.warn("Running Unfolding protective shield");
	running.info("Running Solar panels are shielded");

	//spline ��־���� INFO ��ӡ3��
	spline.debug("Spline Hiding solar panels");
	spline.error("Spline Solar panels are blocked");
	spline.debug("Spline Applying protective shield");
	spline.warn("Spline Unfolding protective shield");
	spline.info("Spline Solar panels are shielded");

	//operation ��־���� ERROR ��ӡһ��. ����Ϊ�̳�running Ҳ��running�д�ӡһ����ͬ��.
	operation.debug("Operation Hiding solar panels");
	operation.error("Operation Solar panels are blocked");
	operation.debug("Operation Applying protective shield");
	operation.warn("Operation Unfolding protective shield");
	operation.info("Operation Solar panels are shielded");

	log4cpp::Category::shutdown();

	return 0;
}
