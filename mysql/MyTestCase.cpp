#include <iostream>
#include <string>
#include <mysql/mysql.h>

class MyTestCase 
{
// Construct / Destruct
public:
    MyTestCase() :m_MYSQL(NULL), m_nConnectTimeOut(20), m_nReadTimeOut(20), m_nWriteTimeOut(20), 
		m_bReConnect(false), m_strCharSet(std::string("UTF-8")) 
    {
        m_MYSQL = mysql_init(NULL);
        if (NULL == m_MYSQL) 
        {
            std::cout << "mysql_init error" << std::endl;
        }

        if (mysql_thread_safe()) 
        {
            std::cout << "mysql thread safe" << std::endl;
        }

		SetConnectTimeOut(m_nConnectTimeOut);

		SetReadTimeOut(m_nReadTimeOut);

		SetWriteTimeOut(m_nWriteTimeOut);

		SetReConnect(m_bReConnect);

		SetCharSet(m_strCharSet);
    }
    ~MyTestCase() 
    {
        DisConnect();
    }

// Attribute
public:
	int GetConnectTimeOut() 
	{
		return m_nConnectTimeOut;
	}
	void SetConnectTimeOut(int nConnectTimeOut) 
	{
		if (!m_MYSQL) 
		{
			return;
		}

		if (0 != mysql_options(m_MYSQL, MYSQL_OPT_CONNECT_TIMEOUT, &nConnectTimeOut)) 
		{
			return;
		}

		m_nConnectTimeOut = nConnectTimeOut;
	}

	int GetReadTimeOut() 
	{
		return m_nReadTimeOut;
	}
	void SetReadTimeOut(int nReadTimeOut) 
	{
		if (!m_MYSQL) 
		{
			return;
		}

		if (0 != mysql_options(m_MYSQL, MYSQL_OPT_READ_TIMEOUT, &nReadTimeOut)) 
		{
			return;
		}

		m_nReadTimeOut = nReadTimeOut;
	}

	int GetWriteTimeOut() 
	{
		return m_nWriteTimeOut;
	}
	void SetWriteTimeOut(int nWriteTimeOut) 
	{
		if (!m_MYSQL) 
		{
			return;
		}

		if (0 != mysql_options(m_MYSQL, MYSQL_OPT_WRITE_TIMEOUT, &nWriteTimeOut)) 
		{
			return;
		}

		m_nWriteTimeOut = nWriteTimeOut;
	}

	bool GetReConnect() 
	{
		return m_bReConnect;
	}
	void SetReConnect(bool bReConnect) 
	{
		if (!m_MYSQL) 
		{
			return;
		}

		if (0 != mysql_options(m_MYSQL, MYSQL_OPT_RECONNECT, &bReConnect)) 
		{
			return;
		}

		m_bReConnect = bReConnect;
	}

	std::string GetCharSet() 
	{
		return m_strCharSet;
	}
	void SetCharSet(std::string strCharSet) 
	{
		if (!m_MYSQL) 
		{
			return;
		}

		if (0 != mysql_options(m_MYSQL, MYSQL_SET_CHARSET_NAME, strCharSet.c_str())) 
		{
			return;
		}

		m_strCharSet = strCharSet;
	}

// Operation
public:
    void Connect() 
    {
    
    }

    void DisConnect() 
    {
        if (m_MYSQL) 
        {
            mysql_close(m_MYSQL);
            m_MYSQL = NULL;
        }
    }

	bool Ping() 
	{
		unsigned long nPreThreadId = 0;
		unsigned long nCurThreadId = 0;

		if (!m_MYSQL) 
		{
			return false;
		}

		if (m_bReConnect) 
		{
			nPreThreadId = mysql_thread_id(m_MYSQL);
		}

		if (0 != mysql_ping(m_MYSQL)) 
		{
			return false;
		}

		if (m_bReConnect) 
		{
			nCurThreadId = mysql_thread_id(m_MYSQL);
		}
		if (nCurThreadId != nPreThreadId) 
		{
			mysql_kill(m_MYSQL, nPreThreadId);
		}

		return true;
	}

    // The client has a maximum communication buffer size [16KB, 16MB]
    // Set by max_allowed_packet parameter
    void Execute(std::string strSql) {}

// Attribute
private:
    MYSQL* m_MYSQL;
    int m_nConnectTimeOut;
	int m_nReadTimeOut;
	int m_nWriteTimeOut;
	bool m_bReConnect;
	std::string m_strCharSet;
};

int main() 
{
    MyTestCase objMyTestCase;
    return 0;
}
