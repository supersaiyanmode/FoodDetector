#include <algorithm>
#include <iterator>

#include <CImg.h>
#include <Sift.h>

#include <SiftSVM.h>

using namespace cimg_library;
SiftSVM::SiftSVM(const std::vector<std::string>& cl, const std::string& wd): SVM(cl, wd) {

}

SiftSVM::~SiftSVM() {

}

void SiftSVM::preprocess(const Dataset& data) {
	for (Dataset::const_iterator it = data.begin(); it != data.end(); it++ ) {
		const std::vector<std::string>& files = it->second;

		for (std::vector<std::string>::const_iterator it1 = files.begin(); it1 != files.end(); it1++) {
			CImg<double> img(it1->c_str());
			CImg<double> grey = img.spectrum() == 1 ? img : img.get_RGBtoHSI().get_channel(2);
			std::vector<SiftDescriptor> desc = Sift::compute_sift(grey);
		}
		
	}
}

std::vector<double> SiftSVM::get_feature_vector(const std::string& filename, bool) {

}
