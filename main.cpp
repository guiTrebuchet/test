#include <stdio.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <exception>
#include <cmath>

#include "MNISTImageFile.h"
#include "MNISTLabelFile.h"

struct LabelData
{
public:
	float*		m_pData;
	int		m_label;
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
	MNISTImageFile mnist_image_file("train-images.idx3-ubyte");
	if(mnist_image_file.GetImageCount() != 60000)
	{
		printf("MNIST image file, image count != 60000");
		return 1;
	}
	MNISTLabelFile mnist_label_file("train-labels.idx1-ubyte");
	if(mnist_label_file.GetLabelCount() != 60000)
	{
		printf("MNIST label file, label count != 60000");
		return 1;
	}


	//	Read images and labels
	std::vector<float> imageData(60000 * 784);
	std::vector<unsigned char> labelData(60000);
	
	mnist_image_file.ReadData(60000, imageData);
	mnist_label_file.ReadData(60000, labelData);
	
	std::vector<LabelData> objList(60000);
	for(int i = 0; i<60000; ++i)
	{
		objList[i].m_pData = (float*)&imageData[i * 784];
		objList[i].m_label = labelData[i];
	}
	
	/*float middle_points[10 * 784] = {0};

	int labelCount[10] = {0};

	for(int i = 0; i<60000; ++i)
	{
		for(int x = 0; x<784; ++x)
		{
			middle_points[objList[i].m_label * 784 + x] += objList[i].m_pData[x];
		}
		++labelCount[objList[i].m_label];
	}
	for(int i = 0; i<10; ++i)
	{
		for(int x = 0; x<784; ++x)
		{
			middle_points[i * 784 + x] /= (float)labelCount[i];
		}
	}*/

	//PrintVector(&middle_points[784 * 0], 784);

	MNISTImageFile mnist_test_image_file("t10k-images.idx3-ubyte");
	MNISTLabelFile mnist_test_label_file("t10k-labels.idx1-ubyte");
	
	//	Read test images and labels
	std::vector<float> imageTestData(10000 * 784);
	std::vector<unsigned char> labelTestData(10000);
	
	mnist_test_image_file.ReadData(10000, imageTestData);
	mnist_test_label_file.ReadData(10000, labelTestData);

	std::vector<LabelData> objTestList(10000);
	for(int i = 0; i<10000; ++i)
	{
		objTestList[i].m_pData = (float*)&imageTestData[i * 784 * sizeof(float)];
		objTestList[i].m_label = (int)labelTestData[i * sizeof(unsigned char)];
	}

	int testResult_sampleSizePerLabel[10] = {0};
	int testResult_CorrectResultPerLabel[10] = {0};
	
	for(int i = 1000; i<1100; ++i)
	{
		LabelData testObj = objTestList[i];
		int closest = 0;
		float closest_dist = dist(testObj.m_pData, objList[0].m_pData, 784);

		for(int j = 0; j<60000; ++j)
		{
			float d = dist(testObj.m_pData, objList[j].m_pData, 784);
			if(d < closest_dist)
			{
				closest = j;
				closest_dist = d;
			}
		}

		printf("Image %i processed.\n", i);
		testResult_sampleSizePerLabel[testObj.m_label] += 1;
		if(closest == testObj.m_label)
			testResult_CorrectResultPerLabel[testObj.m_label] += 1;

	}

	printf("Results : \n");
	for(int i = 0; i<10; ++i)
	{
		printf("	Label %i :\n", i);
		printf("		test image count : %i\n", testResult_sampleSizePerLabel[i]);
		printf("		test label discovered : %i\n", testResult_CorrectResultPerLabel[i]);
		printf("		success rate : %f\n", (float)testResult_CorrectResultPerLabel[i] / (float)testResult_sampleSizePerLabel[i]);

	}
	/*float* data = read_mnist("train-images.idx3-ubyte");
	for(int i = 0; i<784; i++)
	{
		printf("float%i = %f\n", i, data[555][i]);
	}*/
	return 0;
}
