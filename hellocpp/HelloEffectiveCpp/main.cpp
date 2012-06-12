#include <iostream>

class Foo 
{
public:
	Foo(int a) { this->m_a = a; }
	~Foo() { std::cout << "Foo destruct" << std::endl; }

public:
	int operator+(Foo& b) { return this->m_a + b.m_a; }

	bool operator<(Foot& b) 
	{
		if ()
	}

private:
	int m_a;
	int m_b;
};

int main() 
{
	Foo a(1);
	Foo b(2);
	std::cout << a + b << std::endl;
	return 0;
}