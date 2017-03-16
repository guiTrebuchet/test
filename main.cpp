#include <stdio.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <exception>
#include <cmath>

struct LabelData
{
public:
	float*	m_pData;
	int		m_label;
};

class MNISTFile
{
private:
	std::vector<char> m_buffer;

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
	MNISTFile(){}
	MNISTFile& operator=(const MNISTFile&){}
	MNISTFile(const MNISTFile&){}
public:
	MNISTFile(const char* filename)
	{
		std::ifstream file (filename, std::ios::binary);
    	if (!file.is_open())
			throw std::exception();//"Error when openning MNIST file.");

		//	/!\ Should check if image size match what we expect.
		int magic_number=0;
		int number_of_images=0;
		int n_rows=0;
		int n_cols=0;
		file.read((char*)&magic_number,sizeof(magic_number));
		magic_number= ReverseInt(magic_number);
		file.read((char*)&number_of_images,sizeof(number_of_images));
		number_of_images= ReverseInt(number_of_images);
		file.read((char*)&n_rows,sizeof(n_rows));
		n_rows= ReverseInt(n_rows);
		file.read((char*)&n_cols,sizeof(n_cols));
		n_cols= ReverseInt(n_cols);

		int bufferSize = number_of_images * n_rows * n_cols * sizeof(float);
		m_buffer.resize(bufferSize);

		file.read(&m_buffer[0], bufferSize);
	}
	char* GetBuffer()
	{
		return &m_buffer[0];
	}
};

class MNISTLabelFile
{
private:
	std::vector<char> m_buffer;

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
	MNISTLabelFile& operator=(const MNISTFile&){}
	MNISTLabelFile(const MNISTFile&){}
public:
	MNISTLabelFile(const char* filename)
	{
		std::ifstream file (filename, std::ios::binary);
    	if (!file.is_open())
			throw std::exception();//"Error when openning MNIST file.");

		//	/!\ Should check if image size match what we expect.
		int magic_number=0;
		int number_of_labels=0;
		file.read((char*)&magic_number,sizeof(magic_number));
		magic_number= ReverseInt(magic_number);
		file.read((char*)&number_of_labels,sizeof(number_of_labels));
		number_of_labels= ReverseInt(number_of_labels);

		printf("Read label file %s, magic number : %i, number of label : %i\n", filename, magic_number, number_of_labels);

		int bufferSize = number_of_labels * sizeof(int);
		m_buffer.resize(bufferSize);

		file.read(&m_buffer[0], bufferSize);
	}
	char* GetBuffer()
	{
		return &m_buffer[0];
	}
};

float dist(float* pVec1, float* pVec2, int dim)
{
	float value = 0;
	for(int i = 0; i<dim; ++i)
	{
		value += (pVec2[dim] - pVec1[dim]) * (pVec2[dim] - pVec1[dim]);
	}
	return sqrt(value);
}

void PrintVector(float* p, int dim)
{
	for(int i = 0; i<dim; ++i)
	{
		printf("x[%i] = %f\n", i, p[dim]);
	}
}
int main()
{
	MNISTFile mnist_file("train-images.idx3-ubyte");
	MNISTLabelFile mnist_label_file("train-labels.idx1-ubyte");
	std::vector<LabelData> labelData(60000);
	for(int i = 0; i<60000; ++i)
	{
		labelData[i].m_pData = (float*)&mnist_file.GetBuffer()[i * 784 * sizeof(float)];
		labelData[i].m_label = (int)*(unsigned char*)&mnist_label_file.GetBuffer()[i * sizeof(unsigned char)];
	}
	
	float middle_points[10 * 784] = {0};

	for(int j = 0; j<10; ++j)
	{
		int labelCount = 0;
		for(int i = 0; i<60000; ++i)
		{
			if(labelData[i].m_label == j)
			{
				for(int x = 0; x<784; ++x)
				{
					middle_points[j * 784 + x] += labelData[i].m_pData[x];
				}
				++labelCount;
			}
		}
		for(int x = 0; x<784; ++x)
		{
			middle_points[j * 784 + x] /= (float)labelCount;
		}
	}

	PrintVector(&middle_points[784 * 0], 784);

	MNISTFile mnist_test_file("t10k-images.idx3-ubyte");
	MNISTLabelFile mnist_test_label_file("t10k-labels.idx1-ubyte");
	std::vector<LabelData> testLabelData(10000);
	for(int i = 0; i<10000; ++i)
	{
		testLabelData[i].m_pData = (float*)&mnist_test_file.GetBuffer()[i * 784 * sizeof(float)];
		testLabelData[i].m_label = (int)*(unsigned char*)&mnist_test_label_file.GetBuffer()[i * sizeof(unsigned char)];
	}

	int testLabelCount[10] = {0};
	int testLabelPassed[10] = {0};
	
	for(int i = 0; i<10000; ++i)
	{
		LabelData testLabel = testLabelData[i];
		int closest = 0;
		float closest_dist = dist(testLabel.m_pData, &middle_points[0], 784);
		for(int j = 1; j<10; ++j)
		{
			float d = dist(testLabel.m_pData, &middle_points[j], 784);
			if(d < closest_dist)
			{
				closest = j;
				closest_dist = d;
			}
		}

		testLabelCount[testLabel.m_label] += 1;
		if(closest == testLabel.m_label)
			testLabelPassed[testLabel.m_label] += 1;

	}

	printf("Results : \n");
	for(int i = 0; i<10; ++i)
	{
		printf("	Label %i :\n", i);
		printf("		test image count : %i\n", testLabelCount[i]);
		printf("		test label discovered : %i\n", testLabelPassed[i]);
		printf("		success rate : %f\n", (float)testLabelPassed[i] / (float)testLabelCount[i]);

	}
	/*float* data = read_mnist("train-images.idx3-ubyte");
	for(int i = 0; i<784; i++)
	{
		printf("float%i = %f\n", i, data[555][i]);
	}*/
	return 0;
}
