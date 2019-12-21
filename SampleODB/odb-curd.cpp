// SampleODB.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <odb/core.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/connection.hxx>

#include <odb/sqlite/database.hxx>

#include "person.hxx"
#include "person-odb.hxx"
#include "database.hxx"

using namespace std;
using namespace odb::core;

int main2(int argc, char* argv[])
{

	//������ȡ���ݿ�
	//auto_ptr<database> db(new odb::sqlite::database("test.db"));

	//�������ݿ�ṹ ��Ҫ��odb����ʱ���� odb.exe -d sqlite --generate-query --generate-schema person.hxx 
	auto_ptr<database> db = create_database("test.db");


	unsigned long john_id, joe_id;

	// ��������
	{
		person john("John", "Doe", 33);
		person jane("Jane", "Doe", 32);
		person joe("Joe", "Dirt", 30);

		john.nicknames().push_back("Holle");
		john.nicknames().push_back("Kitty");

		transaction t(db->begin());

		// �������ݷ�������ID
		john_id = db->persist(john);
		db->persist(jane);
		joe_id = db->persist(joe);

		t.commit();
	}


	typedef odb::query<person> query;
	typedef odb::result<person> result;

	// ��ѯ����
	{
		transaction t(db->begin());

		result r(db->query<person>(query::age > 30));

		for (result::iterator i(r.begin()); i != r.end(); ++i)
		{
			cout << "Hello, " << i->first() << " " << i->last() << "!" << endl;
		}

		t.commit();
	}

	// ��������
	{
		transaction t(db->begin());

		auto_ptr<person> joe(db->load<person>(joe_id));
		joe->age(joe->age() + 1);
		db->update(*joe);

		t.commit();
	}

	//  ɾ������
	{
		transaction t(db->begin());
		db->erase<person>(john_id);
		t.commit();
	}

	return 0;

}
