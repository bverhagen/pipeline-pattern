#ifndef STATICASSERTEXCEPTION_HPP
#define STATICASSERTEXCEPTION_HPP

#include <string>
#include <exception>

class StaticAssertException : public std::exception {
	public:
		StaticAssertException(const std::string& errorMsg) : _msg(errorMsg) {
			;
		}

		virtual const char* what() const throw() {
			return _msg.c_str();
		}

	private:
		std::string _msg;
};

#endif
