#include <algorithm>
#include <iterator>

#include <CImg.h>

#include <BaseSVM.h>
#include <Config.h>

using namespace cimg_library;
BaseSVM::BaseSVM(const std::vector<std::string>& cl, const std::string& wd): SVM(cl, wd) {

}

BaseSVM::~BaseSVM() {

}

std::vector<double> BaseSVM::get_feature_vector(const std::string& filename, bool) {
	CImg<double> input(filename.c_str());
	CImg<double> resized = input.resize(40, 40, 1, 3);
	int force_grey = config.get<int>("base.forcegrey");
	if (resized.spectrum() == 3 && !force_grey) {
		std::vector<double> result;
		result.reserve(resized.width() * resized.height() * resized.spectrum());
		for (int x=0; x<resized.width(); x++) {
			for (int y=0; y<resized.height(); y++) {
				for (int s=0; s<resized.spectrum(); s++) {
					result.push_back(resized(x,y,0,s));
				}
			}
		}
		return result;
	}else {
		CImg<double> grey = resized.spectrum() == 1? resized: resized.get_RGBtoHSI().get_channel(2);
		std::vector<double> result(grey.width() * grey.height(), 0);
		std::copy(grey.begin(), grey.end(), result.begin());
		return result;
	} 
}
