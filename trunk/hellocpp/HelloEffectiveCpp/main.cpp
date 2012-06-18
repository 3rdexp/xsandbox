#include <iostream>
#include <vector>

namespace CCTEC 
{

class Object 
{
public:
	Object() {}
	virtual ~Object() {}
};

class CLabel : public Object 
{
public:
	CLabel() {}
	virtual ~CLabel() {}
};

class Foo 
{
public:
	Foo() :m_a(-1), m_b(-1), m_c(-1), m_vec(NULL), m_str("Hello World") {}
	Foo(const Foo& rhs) :m_c(-1) 
	{
		this->m_a = rhs.m_a;
		this->m_b = rhs.m_b;
		// As for the copy constructor and the copy assignment operator, 
		// the compiler-generated versions simply copy each NON-STATIC 
		// data member of the source object over to the target object
		//this->m_c = rhs.m_c;
		this->m_vec = rhs.m_vec;
		this->m_str = rhs.m_str;
	}
	~Foo() { m_vec.clear(); }

public:
	Foo& operator=(const Foo& rhs) 
	{
		// self-assignment-safe
		if (this == &rhs) return *this;

		this->m_a = rhs.m_a;
		this->m_b = rhs.m_b;
		this->m_vec = rhs.m_vec;
		this->m_str = rhs.m_str;

		return *this;
	}

	Foo& operator+(const Foo& b) 
	{ 
		this->m_a + b.m_a;

		return *this;
	}

	const char& operator[](int pos) const { return m_str[pos]; }

	char& operator[](int pos) 
	{
		std::cout << "calls const" << std::endl;
		return const_cast<char&>(static_cast<const Foo&>(*this)[pos]); 
	}

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
	const int m_c;
	//std::vector<int> m_vec;
	mutable std::vector<int> m_vec;
	std::string m_str;
};

class HomeForSale 
{
public:
	HomeForSale();
	~HomeForSale();

// prevent copying
private:
	HomeForSale(const HomeForSale&);
	HomeForSale& operator=(const HomeForSale&);
};

};

int main() 
{
	CCTEC::Foo a;
	CCTEC::Foo b;
	const CCTEC::Foo c;

	b = a;

	std::cout << "get_a: " << a.get_a() << std::endl;
	std::cout << "a[0]: " << a[0] << std::endl;
	std::cout << "c[0]: " << c[0] << std::endl;

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

	CCTEC::Object obj;
	//std::auto_ptr<CCTEC::Object> pobj_auto_ptr(new CCTEC::Object);
	CCTEC::Object* pobj = NULL;
	CCTEC::CLabel lab;
	//std::auto_ptr<CCTEC::CLabel> plab_auto_ptr(new CCTEC::CLabel);
	CCTEC::CLabel* plab = NULL;

	//pobj_auto_ptr = dynamic_cast<std::auto_ptr<CCTEC::Object>>(plab_auto_ptr);
	pobj = dynamic_cast<CCTEC::Object*>(&lab);
	// wrong: base-to-derived
	//plab = dynamic_cast<CCTEC::CLabel*>(&obj);
	plab = static_cast<CCTEC::CLabel*>(&obj);

	int n1 = 0;
	float n2 = 0.01f;
	n1 = static_cast<int>(n2);

	if (_MSC_VER != 1200) system("pause");

	return 0;
}
