#include "coder.hpp"

#include <stdexcept>
#include <iostream>

extern "C" {
	#include <stdlib.h>
}

using namespace std;
	
namespace gate
{
	const std::string NULLSTR;
	const std::string Coder::decode()
	{
		try{
			
			if(m_currStr.length() == 0)
			{
				return NULLSTR;
			}
			
			if((unsigned char)m_currStr.at(0) == 0xff) // head
			{
				if(m_currStr.length() < 2)
				{
					cout << "not enough byte1" << endl;
					return NULLSTR;
				}
				// 获取包长度
				if(!m_len)
				{
					char len[2];
					m_currStr.copy(len, 2, 1);
					m_len = (len[0] << 8) ^ len[1];
				}
				cout << "package len: "<< m_len << endl;
				if(m_currStr.length() < m_len + 3)
				{
					cout << "not enough byte2" << endl;
					return NULLSTR;
				}
				std::string result = m_currStr.substr(3, m_len);
				m_currStr.erase(0, m_len+3);
				m_len = 0;
				cout << "finish" << m_currStr.length() << endl;
				return result;
			}else
			{
				m_currStr.clear();
				return NULLSTR;
			}
		}catch(const std::out_of_range& err) {
			cout << "error" << endl;
			m_currStr.clear();
			return NULLSTR;
		}
		
	}

	const std::string Coder::encode()
	{
	}

}
