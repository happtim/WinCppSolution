// file      : hello/person.hxx
// copyright : not copyrighted - public domain

#ifndef PERSON_HXX
#define PERSON_HXX

#include <string>
#include <cstddef> // std::size_t
#include <set>
#include <map>
#include <vector>

#include <odb/core.hxx>
#include <odb/nullable.hxx>

typedef std::vector<std::string> names;
typedef std::set<std::string> emails;
typedef std::map<unsigned short, float> age_weight_map;



#pragma db value
class email_address
{
	public:
		email_address() {}
		email_address(const std::string& address)
		{
			std::string::size_type p(address.find('@'));
			recipient_.assign(address, 0, p);
			domain_.assign(address, p + 1, std::string::npos);
		}

		const std::string& recipient() const
		{
			return recipient_;
		}

		const std::string& domain() const
		{
			return domain_;
		}

		std::string address() const
		{
			return recipient_ + '@' + domain_;
		}

	private:
		friend class odb::access;

		std::string recipient_;
		std::string domain_;
};



#pragma db object
class person
{
	public:
		person(const std::string& first,
			const std::string& last,
			unsigned short age)
			: first_(first), last_(last) ,age_(age)
		{
		}

		person(const std::string& first,
			const std::string& last,
			const std::string & email,
			unsigned short age)
			: first_(first), last_(last), email_(email), age_(age)
		{
		}

		const std::string& first() const
		{
			return first_;
		}

		const std::string& last() const
		{
			return last_;
		}

		// Nicknames.
		const names& nicknames() const
		{
			return nicknames_;
		}

		names & nicknames() {
			return nicknames_;
		}

		// Email address.
		const email_address& email() const
		{
			return email_;
		}

		const age_weight_map& age_weight() const
		{
			return age_weight_;
		}

		 age_weight_map& age_weight() 
		{
			return age_weight_;
		}


		unsigned short age() const
		{
			return age_;
		}

		void age(unsigned short age)
		{
			age_ = age;
		}

	private:
		friend class odb::access;

		person() {}

		#pragma db id auto
		unsigned long id_;

		std::string first_;
		std::string last_;
		unsigned short age_;

		//定义集合
		names nicknames_;
		age_weight_map age_weight_;

		//定义嵌套类
		email_address email_;

};



#endif // PERSON_HXX