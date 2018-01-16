// RpcClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "xmlrpc++0.7\XmlRpc.h"

using namespace XmlRpc;


int main() {

	XmlRpcClient c("localhost",8000);
	XmlRpcValue oneArg,result;
	oneArg[0] = 2;
	if (c.execute("findai", oneArg, result)) {
		std::cout << "xmlrpc server return message " << result << std::endl;
	}
	int a;
	scanf_s("%d", &a);
	return 0;
}

