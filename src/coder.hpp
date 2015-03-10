#ifndef __CODER_H__
#define __CODER_H__

#include <string>
#include <iostream>

namespace gate
{
class Coder
{
	public:
		Coder():m_len(0)
		{
			
		}
		
		void append(const char* data, size_t size)
		{
			m_currStr.append(data, size);
		}
		const std::string encode();

		const std::string decode();
		
	private:
		std::string m_currStr;	// ��δ����������
		std::string m_currData; // ����֮�������
		unsigned int m_len;
	
};// class Coder
	
}// namespace gate

#endif //__CODER_H__
