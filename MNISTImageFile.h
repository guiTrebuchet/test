
#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include <string>

class MNISTImageFile
{
private:
	int					m_magic = 0;
	int					m_imageCount = 0;
	int					m_imageWidth = 0;
	int					m_imageHeight = 0;
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
	MNISTImageFile(){}
	MNISTImageFile& operator=(const MNISTImageFile&){}
	MNISTImageFile(const MNISTImageFile&){}
public:
	MNISTImageFile(const char* filename)
		:	m_file(filename, std::ios::binary)
	{
    	if (!m_file.is_open())
			throw std::exception();//"Error when openning MNIST file.");

		m_file.read((char*)&m_magic, sizeof(m_magic));
		m_magic = ReverseInt(m_magic);
		m_file.read((char*)&m_imageCount,sizeof(m_imageCount));
		m_imageCount = ReverseInt(m_imageCount);
		m_file.read((char*)&m_imageHeight, sizeof(m_imageHeight));
		m_imageHeight = ReverseInt(m_imageHeight);
		m_file.read((char*)&m_imageWidth, sizeof(m_imageWidth));
		m_imageWidth = ReverseInt(m_imageWidth);
		
		//if(m_magic != )
			//throw std::exception();
	}
	void ReadData(int imageCount, std::vector<float>& data)
	{
		int size = imageCount * m_imageWidth * m_imageHeight;
		data.resize(size);
		m_file.read((char*)&data[0], size);
	}
	int GetImageCount()
	{
		return m_imageCount;
	}
	int	GetImageWidth()
	{
		return m_imageWidth;
	}
	int GetImageHeight()
	{
		return m_imageHeight;
	}
};

