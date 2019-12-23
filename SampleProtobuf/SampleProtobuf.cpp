// SampleProtobuf.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include <google/protobuf/util/time_util.h>

#include "addressbook.pb.h"

using namespace std;
using google::protobuf::util::TimeUtil;

// This function fills in a Person message based on user input.
void PromptForAddress(SampleProtobuf::Person* person) {

	//set id
	person->set_id(1);

	cout << "Enter name: ";
	getline(cin, *person->mutable_name());


	person->set_email("timge@example.com");
	
	SampleProtobuf::Person::PhoneNumber* phone_number1 = person->add_phones();
	phone_number1->set_number("1234567");
	phone_number1->set_type(SampleProtobuf::Person::MOBILE);

	SampleProtobuf::Person::PhoneNumber* phone_number2 = person->add_phones();
	phone_number2->set_number("2345678");
	phone_number2->set_type(SampleProtobuf::Person::HOME);

	SampleProtobuf::Person::PhoneNumber* phone_number3 = person->add_phones();
	phone_number3->set_number("3456789");
	phone_number3->set_type(SampleProtobuf::Person::WORK);

	*person->mutable_last_updated() = TimeUtil::SecondsToTimestamp(time(NULL));
}

// Iterates though all people in the AddressBook and prints info about them.
void ListPeople(const SampleProtobuf::AddressBook& address_book) {
	for (int i = 0; i < address_book.people_size(); i++) {
		const SampleProtobuf::Person& person = address_book.people(i);

		cout << "Person ID: " << person.id() << endl;
		cout << "  Name: " << person.name() << endl;
		if (person.email() != "") {
			cout << "  E-mail address: " << person.email() << endl;
		}

		for (int j = 0; j < person.phones_size(); j++) {
			const SampleProtobuf::Person::PhoneNumber& phone_number = person.phones(j);

			switch (phone_number.type()) {
			case SampleProtobuf::Person::MOBILE:
				cout << "  Mobile phone #: ";
				break;
			case SampleProtobuf::Person::HOME:
				cout << "  Home phone #: ";
				break;
			case SampleProtobuf::Person::WORK:
				cout << "  Work phone #: ";
				break;
			default:
				cout << "  Unknown phone #: ";
				break;
			}
			cout << phone_number.number() << endl;
		}
		if (person.has_last_updated()) {
			cout << "  Updated: " << TimeUtil::ToString(person.last_updated()) << endl;
		}
	}
}

int main(int argc, char ** argv)
{
	// Verify that the version of the library that we linked against is
   // compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	SampleProtobuf::AddressBook address_book;

	{
		// Read the existing address book.
		fstream input("addressbok.db", ios::in | ios::binary);
		if (!input) {
			cout <<"addressbok.txt : File not found.  Creating a new file." << endl;
		}
		else if (!address_book.ParseFromIstream(&input)) {
			cerr << "Failed to parse address book." << endl;
			return -1;
		}
	}

	// Add an address.
	PromptForAddress(address_book.add_people());

	{
		// Write the new address book back to disk.
		fstream output("addressbok.db", ios::out | ios::trunc | ios::binary);
		if (!address_book.SerializeToOstream(&output)) {
			cerr << "Failed to write address book." << endl;
			return -1;
		}
	}

	{
		// Read the existing address book.
		fstream input("addressbok.db", ios::in | ios::binary);
		if (!address_book.ParseFromIstream(&input)) {
			cerr << "Failed to parse address book." << endl;
			return -1;
		}
	}

	ListPeople(address_book);

	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();

	getchar();

	return 0;
}
