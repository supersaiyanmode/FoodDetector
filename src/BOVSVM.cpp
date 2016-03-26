#include <algorithm>
#include <iterator>

#include <CImg.h>

#include <BOVSVM.h>

using namespace cimg_library;
BOVSVM::BOVSVM(const std::vector<std::string>& cl, const std::string& wd): SVM(cl, wd) {

}

BOVSVM::~BOVSVM() {

}

std::vector<double> BOVSVM::get_feature_vector(const std::string& filename, bool) {
	CImg<double> input(filename.c_str());
	//Apply SIFT
	

	//Apply K-means 
	
	//Create the histogram
	
	//return the histogram as vector<double>
	return std::vector<double>();
}
