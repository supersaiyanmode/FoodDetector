#include <algorithm>

#include <CImg.h>
#include <EigenSVM.h>

EigenSVM::EigenSVM(const std::vector<std::string>& cl, const std::string& wd): SVM(cl, wd) {

}

EigenSVM::~EigenSVM() {

}

std::vector<double> EigenSVM::get_feature_vector(const std::string& filename) {
	return std::vector<double>();
}
