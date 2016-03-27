#include <algorithm>
#include <iterator>
#include <math.h>
#include <CImg.h>
#include <vector>
#include <HaarSVM.h>

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
	CImg<double> resized = input.resize(10, 10, 1, 3);
	CImg<double> grey =
			resized.spectrum() == 1 ?
					resized : resized.get_RGBtoHSI().get_channel(2);
	//Code Here
	//Calculate Summed Area Table
	//Calculate Borders
	for (int i = 1; i < grey.width(); i++) {
		grey[0][i] = grey[0][i] + grey[0][i - 1];
	}

	for (int i = 1; i < grey.height(); i++) {
		grey[i][0] = grey[i][0] + grey[i - 1][0];
	}

	for (int i = 1; i < grey.height(); i++) {
		for (int j = 1; j < grey.width(); j++) {
			grey[i][j] = grey[i][j] + grey[i - 1][j] + grey[i][j - 1]
					- grey[i - 1][j - 1];
		}
	}

	//Let Filter size be 1/4th of the Original Image Size
	int filter_size = grey.size() / 4;

	int mid_point = filter_size / 2;

	int X = grey.width() - filter_size - 1;
	int Y = grey.height() - filter_size - 1;
	double Hx, Hy;
	double area1, area2;
	vector<double> haar_features;
	for (int x = 0; x < X; x++) {
		for (int y = 0; y < Y; y++) {
			//Calculate Hx - Sliding Window
			area1 = (grey[x][y] + grey[x + mid_point][y + 2 * mid_point])
					- (grey[x + mid_point][y] + grey[x][y + 2 * mid_point]);

			area2 = (grey[x + mid_point][y]
					+ grey[x + 2 * mid_point][y + 2 * mid_point])
					- (grey[x + mid_point][y + 2 * mid_point]
							+ grey[x + 2 * mid_point][y]);

			Hx = (area1 - area2) / (filter_size * filter_size);

			//Calculate Hy - Sliding Window

			area1 = (grey[x][y] + grey[x + 2 * mid_point][y + mid_point])
					- (grey[x][y + mid_point] + grey[x + 2 * mid_point][y]);

			area2 = (grey[x][y + mid_point]
					+ grey[x + 2 * mid_point][y + 2 * mid_point])
					- (grey[x][y + 2 * mid_point]
							+ grey[x + 2 * mid_point][y + mid_point]);

			Hy = (area1 - area2) / (filter_size * filter_size);
			// Push the magnitude in the Vector
			haar_features.push_back(sqrt((Hx * Hx) + (Hy * Hy)));
		}
	}

	///
	std::vector<double> result(haar_features.size(), 0);
	std::copy(grey.begin(), grey.end(), result.begin());
	return result;
}
