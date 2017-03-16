
#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include <string>

class MNISTLabelFile
{
private:
	int					m_magic = 0;
	int					m_labelCount = 0;
	std::ifstream		m_file;

	int ReverseInt(int i)
	{
		unsigned char ch1, ch2, ch3, ch4;
		ch1=i&255;
		ch2=(i>>8)&255;
		ch3=(i>>16)&255;
		ch4=(i>>24)&255;
		return((int)ch1<<24)+((int)ch2<<16)+((int)ch3<<8)+ch4;
	}
	
	//	Non-copyable
	MNISTLabelFile(){}
	MNISTLabelFile& operator=(const MNISTLabelFile&){}
	MNISTLabelFile(const MNISTLabelFile&){}
public:
	MNISTLabelFile(const char* filename)
		:	m_file(filename, std::ios::binary)
	{
    	if (!m_file.is_open())
			throw std::exception();//"Error when openning MNIST file.");

		m_file.read((char*)&m_magic, sizeof(m_magic));
		m_magic = ReverseInt(m_magic);
		m_file.read((char*)&m_labelCount,sizeof(m_labelCount));
		m_labelCount = ReverseInt(m_labelCount);
		
		//if(m_magic != )
			//throw std::exception();
	}
	void ReadData(int count, std::vector<unsigned char>& data)
	{
		int size = count * sizeof(unsigned char);
		data.resize(size);
		m_file.read((char*)&data[0], size);
	}
	int GetLabelCount()
	{
		return m_labelCount;
	}
};

