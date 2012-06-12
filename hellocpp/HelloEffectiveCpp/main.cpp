#include <iostream>
#include <vector>

class Foo 
{
public:
	Foo(int a) :m_b(-1), m_vec(NULL), m_str("Hello World") { this->m_a = a; }
	~Foo() { m_vec.clear(); }

public:
	const int operator+(Foo& b) const { return this->m_a + b.m_a; }

	const char& operator[](int pos) const { return m_str[pos]; }
	// FIXME: no effect ?!
	char& operator[](int pos) { return const_cast<char&>(static_cast<const Foo&>(*this)[pos]); }

public:
	int get_a() const 
	{  
		// error C2166: l-value specifies const object, if no defined mutable
		m_a = 3;
		return m_a; 
	}

	std::vector<int> get_vec() const 
	{
		int i = 10;
		while (i--) 
		{
			// error C2662: 'push_back' : cannot convert 'this' pointer from 
			// 'const class std::vector<int,class std::allocator<int> >' to 
			// 'class std::vector<int,class std::allocator<int> > &' Conversion loses qualifiers
			m_vec.push_back(i);
		}
		return m_vec;
	}

private:
	//int m_a;
	mutable int m_a;
	int m_b;
	//std::vector<int> m_vec;
	mutable std::vector<int> m_vec;
	std::string m_str;
};

int main() 
{
	Foo a(1);
	Foo b(2);
	std::cout << "a + b = " << a + b << std::endl;
	std::cout << "get_a: " << a.get_a() << std::endl;
	std::cout << "a[0]: " << a[0] << std::endl;

	std::vector<int> vec = a.get_vec();
	const std::vector<int>::iterator iter = vec.begin();
	std::cout << "iter: " << *iter << std::endl;
	*iter = 10;
	std::cout << "iter: " << *iter << std::endl;
	//iter++; error C2166: l-value specifies const object due to const
	std::vector<int>::const_iterator citer = vec.begin();
	std::cout << "citer: " << *citer << std::endl;
	//*citer = 11; error C2166: l-value specifies const object
	citer++;
	std::cout << "citer: " << *citer << std::endl;

	return 0;
}
