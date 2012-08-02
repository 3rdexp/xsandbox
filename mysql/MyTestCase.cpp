#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <mysql/mysql.h>

class MyTestCase 
{
// Construct / Destruct
public:
    MyTestCase() :m_MYSQL(NULL), m_nConnectTimeOut(20), m_nReadTimeOut(20), m_nWriteTimeOut(20), 
		m_bReConnect(false), m_strCharSet(std::string("utf8")) 
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

	std::string GetClientInfo() 
	{
		std::string strRet = "";

		strRet = mysql_get_client_info();
		std::cout << strRet << std::endl;

		return strRet;
	}

// Operation
public:
	void Connect(const char* strHost, const char* strUser, const char* strPasswd, const char* strDb, 
		unsigned int nPort = 3306, unsigned long nClinetFlag = 0, const char* strUNIXSocket = NULL) 
    {
		if (!m_MYSQL) 
		{
			return;
		}

		if (!mysql_real_connect(m_MYSQL, strHost, strUser, strPasswd, strDb, nPort, strUNIXSocket, nClinetFlag)) 
		{
			std::cout << "mysql_real_connect error" << std::endl;
		} 
		else 
		{
			std::cout << "mysql connect success" << std::endl;
		}
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

		// auto-reconnect is disabled, mysql_ping() returns an error
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

    bool Execute(const char* strSql) 
	{
		if (!m_MYSQL) 
		{
			return false;
		}

		if (0 != mysql_query(m_MYSQL, strSql)) 
		{
			return false;
		}

		return true;
	}
	bool Execute(const char* strSql, unsigned long nLength) 
	{
		if (!m_MYSQL) 
		{
			return false;
		}

		if (0 != mysql_master_query(m_MYSQL, strSql, nLength)) 
		{
			return false;
		}

		return true;
	}
	bool SlaveExecute(const char* strSql, unsigned long nLength) 
	{
		if (!m_MYSQL) 
		{
			return false;
		}

		if (0 != mysql_slave_query(m_MYSQL, strSql, nLength)) 
		{
			return false;
		}

		return true;
	}

	void TestCase1() 
	{
		char strSql[1024] = {'\0'};
		int i = 0;

		for (i = 0; i < 1000; i++) 
		{
			memset(strSql, 0, 1024);
			sprintf(strSql, "INSERT INTO test1 (value) VALUES ('test%d')", i);
			Execute(strSql);
		}
	}

	void TestCase2() 
	{
		const char* strSql = "SELECT * FROM test1";
		MYSQL_RES* result = NULL;
		MYSQL_FIELD* filed = NULL;
		MYSQL_ROW row;

		if (!SlaveExecute(strSql, strlen(strSql))) 
		{
			return;
		}

		// store result->data to client
		//result = mysql_store_result(m_MYSQL);
		result = mysql_use_result(m_MYSQL);
		if (!result) 
		{
			if (0 != mysql_field_count(m_MYSQL)) 
			{
				std::cout << "mysql_store_result should have returned data" << std::endl;
			}
			return;
		}

		while ((filed = mysql_fetch_field(result))) 
		{
			std::cout << "filed name: " << filed->name << std::endl;
		}

		while ((row = mysql_fetch_row(result)))
		{
			std::cout << row[1] << std::endl;
		}

		if (result) 
		{
			mysql_free_result(result);
			result = NULL;
		}
	}

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

	objMyTestCase.Connect("localhost", "root", "!123456", "test");
	
	objMyTestCase.GetClientInfo();

	objMyTestCase.TestCase2();

    return 0;
}
