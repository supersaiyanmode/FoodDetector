#include <algorithm>
#include <iterator>
#include <math.h>
#include <CImg.h>
#include <vector>
#include <HaarSVM.h>
#include <iostream>
#include<string>

using namespace cimg_library;
using namespace std;

HaarSVM::HaarSVM(const std::vector<std::string>& cl, const std::string& wd) :
		SVM(cl, wd) {

}

HaarSVM::~HaarSVM() {

}

std::vector<double> HaarSVM::get_feature_vector(const std::string& filename,
		bool) {
	CImg<double> input(filename.c_str());
	CImg<double> resized = input.resize(40, 40, 1, 3);
	CImg<double> grey =
			resized.spectrum() == 1 ?
					resized : resized.get_RGBtoHSI().get_channel(2);
	//Code Here
	//Calculate Summed Area Table
	//Calculate Borders
	vector<vector<double> > SAT;
	SAT.resize(grey.width(), vector<double>(grey.height(), 0));

	double temp_val = grey(0, 0, 0, 0);
	SAT[0][0] = temp_val;
	for (int i = 1; i < grey.width(); i++) {
		//grey[0,i,0,0] = grey[0,i,0,0] + grey[0,i - 1,0,0];
		temp_val = temp_val + grey(i, 0, 0, 0);
		SAT[i][0] = temp_val;
	}
	temp_val = grey(0, 0, 0, 0);
	for (int i = 1; i < grey.height(); i++) {
		temp_val = temp_val + grey(0, i, 0, 0);
		SAT[0][i] = temp_val;
	}

	for (int i = 1; i < grey.height(); i++) {
		for (int j = 1; j < grey.width(); j++) {
			SAT[i][j] = grey(i, j, 0, 0) + grey(i - 1, j, 0, 0)
					+ grey(i, j - 1, 0, 0) - grey(i - 1, j - 1, 0, 0);
		}
	}

	//Let Filter size be 1/4th of the Original Image Size
	int filter_size = grey.width() / 8;

	int mid_point = filter_size / 2;

	int X = grey.width() - filter_size - 1;
	int Y = grey.height() - filter_size - 1;
	double Hx, Hy;
	double area1, area2;
	vector<double> haar_features;
	for (int x = 0; x < X; x++) {

		for (int y = 0; y < Y; y++) {
			//Calculate Hx - Sliding Window
			area1 = (SAT[x][y] + SAT[(x + mid_point)][y + 2 * mid_point])
					- (SAT[x + mid_point][y] + SAT[x][y + 2 * mid_point]);

			area2 = (SAT[x + mid_point][y]
					+ SAT[x + 2 * mid_point][y + 2 * mid_point])
					- (SAT[x + mid_point][y + 2 * mid_point]
							+ SAT[x + 2 * mid_point][y]);

			Hx = (area1 - area2) / (filter_size * filter_size);

			//Calculate Hy - Sliding Window

			area1 = (SAT[x][y] + SAT[x + 2 * mid_point][y + mid_point])
					- (SAT[x][y + mid_point] + SAT[x + 2 * mid_point][y]);

			area2 = (SAT[x][y + mid_point]
					+ SAT[x + 2 * mid_point][y + 2 * mid_point])
					- (SAT[x][y + 2 * mid_point]
							+ SAT[x + 2 * mid_point][y + mid_point]);

			Hy = (area1 - area2) / (filter_size * filter_size);
			// Push the magnitude in the Vector

			haar_features.push_back(sqrt((Hx * Hx) + (Hy * Hy)));
		}
	}

	///
	//std::vector<double> result(haar_features.size(), 0);
	//std::copy(grey.begin(), grey.end(), result.begin());
	//cout << haar_features.size() << endl;
	return haar_features;
}
