#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include "Vector/ui_vector.h"
#include <string>

struct big_integer 
{
	big_integer();
	big_integer(big_integer const& other);
	big_integer(int a);
	big_integer(long long a);
	big_integer(bool nsign, ui_vector const &ndata);
	explicit big_integer(std::string const& str);

	big_integer& operator=(big_integer const& other);

	void swap(big_integer &other) noexcept;

	big_integer& operator+=(big_integer const& rhs);
	big_integer& operator-=(big_integer const& rhs);
	big_integer& operator*=(big_integer const& rhs);
	big_integer& operator/=(big_integer const& rhs);
	big_integer& operator%=(big_integer const& rhs);

	big_integer& operator&=(big_integer const& rhs);
	big_integer& operator|=(big_integer const& rhs);
	big_integer& operator^=(big_integer const& rhs);
	
	big_integer& operator<<=(int rhs);
	big_integer& operator>>=(int rhs);
	
	
	big_integer operator+() const;
	big_integer operator-() const;
	big_integer operator~() const;
	
	big_integer& operator++();
	big_integer operator++(int);

	big_integer& operator--();
	big_integer operator--(int);


	friend bool operator!=(big_integer const& a, big_integer const& b);
	friend bool operator<(big_integer const& a, big_integer const& b);
	friend bool operator>(big_integer const& a, big_integer const& b);
	friend bool operator<=(big_integer const& a, big_integer const& b);
	friend bool operator>=(big_integer const& a, big_integer const& b);
	friend bool operator==(big_integer const& a, big_integer const& b);
	friend big_integer operator+(big_integer a, big_integer const& b);

	friend big_integer operator-(big_integer a, big_integer const& b);
	friend big_integer operator*(big_integer a, big_integer const& b);
	//friend big_integer operator*(big_integer a, int const& b);
	friend big_integer operator/(big_integer a, big_integer const& b);
	friend big_integer operator%(big_integer a, big_integer const& b);

	friend big_integer operator&(big_integer a, big_integer const& b);
	friend big_integer operator|(big_integer a, big_integer const& b);
	friend big_integer operator^(big_integer a, big_integer const& b);

	friend big_integer abs(big_integer const& a);
	friend big_integer mul_big_short(big_integer a, unsigned int const& b);

	friend big_integer div_big_short(big_integer a, unsigned int const& b);

	friend big_integer operator<<(big_integer a, int b);
	friend big_integer operator>>(big_integer a, int b);
	friend std::pair<big_integer, big_integer> longdivide(big_integer const & x, big_integer const& y);
	friend bool cmp_prefix(big_integer& r, big_integer& dq, size_t k, size_t m);
	friend void difference(big_integer& r, big_integer & dq, size_t k, size_t m);
	friend std::string to_string(big_integer const& a);
	template<class FuncT>
	friend big_integer bitwise_operation(big_integer a, big_integer const& b, FuncT f);

private:
	
	bool sign;
	ui_vector data;

	size_t length() const;
	unsigned int get_digit(size_t ind) const;
	void make_correct();
};
big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);

big_integer operator/(big_integer a, big_integer const& b);

big_integer operator%(big_integer a, big_integer const& b);

template<class FuncT>
big_integer bitwise_operation(big_integer a, big_integer const& b, FuncT f);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);


bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);
std::string to_string(big_integer const& a);

bool operator==(big_integer const& a, big_integer const& b);
std::pair<big_integer, big_integer> longdivide(big_integer const & x, big_integer const & y);
bool cmp_prefix(big_integer& r, big_integer& dq, size_t k, size_t m);
void difference(big_integer& r, big_integer & dq, size_t k, size_t m);
#endif