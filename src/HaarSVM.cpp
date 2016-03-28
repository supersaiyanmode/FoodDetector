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
	vector<HaarRow> window = this->getWindowVector();
	CImg<double> input(filename.c_str());
	CImg<double> resized = input.resize(60, 60, 1, 3);
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

	vector<double> haar_features;

	double Hx, Hy;
	double area1, area2;
	for (size_t i = 0; i < window.size(); i++) {
		int x = window[i].x;
		int y = window[i].y;
		int w = window[i].size_width;
		int h = window[i].size_height;
		int midpoint_x = w / 2;
		int midpoint_y = h / 2;
		//Hx
		area1 = (SAT[x][y] + SAT[(x + midpoint_x)][y + 2 * midpoint_y])
				- (SAT[x + midpoint_x][y] + SAT[x][y + 2 * midpoint_y]);
		area2 = (SAT[x + midpoint_x][y]
				+ SAT[x + 2 * midpoint_x][y + 2 * midpoint_y])
				- (SAT[x + midpoint_x][y + 2 * midpoint_y]
						+ SAT[x + 2 * midpoint_x][y]);
		Hx = (area1 - area2);

		//Hy
		area1 = (SAT[x][y] + SAT[x + 2 * midpoint_x][y + midpoint_y])
				- (SAT[x][y + midpoint_y] + SAT[x + 2 * midpoint_x][y]);

		area2 = (SAT[x][y + midpoint_y]
				+ SAT[x + 2 * midpoint_x][y + 2 * midpoint_y])
				- (SAT[x][y + 2 * midpoint_y]
						+ SAT[x + 2 * midpoint_x][y + midpoint_y]);

		Hy = (area1 - area2);
		haar_features.push_back(sqrt((Hx * Hx) + (Hy * Hy)));

	}

	return haar_features;
}