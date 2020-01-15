
#include "../stdafx.h"

#include "server.hpp"
#include "client.hpp"


void test_protobuf() {

	try
	{

		asio::io_context io_context_server;
		serialization::server server(io_context_server, 9601);

		std::thread thread_server([&io_context_server]() {io_context_server.run(); });

	
		asio::io_context io_context_client;
		serialization::client client(io_context_client, "127.0.0.1", "9601");
		std::thread thread_client([&io_context_client]() {io_context_client.run(); });

		serialization::CamelMessage camelMessage_;
		auto pReportToAgvs = camelMessage_.mutable_reporttoagvs();
		//camelMessage_.set_allocated_reporttoagvs(pReportToAgvs.get());
		pReportToAgvs->set_battery(0.8);
		pReportToAgvs->set_currentstation("S200");
		pReportToAgvs->set_ischarging(false);
		pReportToAgvs->set_version("0102");

		auto pPosion = pReportToAgvs->mutable_posion();
		//pReportToAgvs->set_allocated_posion(pPosion.get());
		pPosion->set_x(1.11);
		pPosion->set_y(-2.22);
		pPosion->set_theta(1.57);

		auto pDevice = pReportToAgvs->add_devices();
		pDevice->set_id(1);
		pDevice->set_name("test01");
		pDevice->set_value(0.2);
		pDevice->set_isactivate(true);

		if ((time(0) % 2))
		{
			pDevice = pReportToAgvs->add_devices();
			pDevice->set_id(2);
			pDevice->set_name("test05");
			pDevice->set_value(0.8);
			pDevice->set_isactivate(true);
		}

		client.to_write(camelMessage_);

		thread_server.join();
		thread_client.join();

	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}