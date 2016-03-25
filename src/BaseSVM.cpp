#include <algorithm>
#include <iterator>

#include <CImg.h>

#include <BaseSVM.h>

using namespace cimg_library;
BaseSVM::BaseSVM(const std::vector<std::string>& cl, const std::string& wd): SVM(cl, wd) {

}

BaseSVM::~BaseSVM() {

}

std::vector<double> BaseSVM::get_feature_vector(const std::string& filename) {
	CImg<double> input(filename.c_str());
	CImg<double> resized = input.resize(10, 10, 1, 3);
	CImg<double> grey = resized.spectrum() == 1? resized: resized.get_RGBtoHSI().get_channel(2);
	std::vector<double> result(grey.width() * grey.height(), 0);
	std::copy(grey.begin(), grey.end(), result.begin());
	return result;
}
