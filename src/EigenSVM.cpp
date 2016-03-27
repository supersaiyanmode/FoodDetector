#include <algorithm>

#include <CImg.h>
#include <EigenSVM.h>

EigenSVM::EigenSVM(const std::vector<std::string>& cl, const std::string& wd): SVM(cl, wd) {

}

EigenSVM::~EigenSVM() {

}

void EigenSVM::preprocess(const Dataset& data) {
	for (Dataset::const_iterator it = data.begin(); it != data.end(); it++) {
		//This is one iteration for a given food type.

		std::string cur_food_name = it->first;
		std::vector<std::string> files = it->second;

		//Calculate average
		

		//Calculate top K Eigen vectors
		


	}
}

std::vector<double> EigenSVM::get_feature_vector(const std::string& filename, bool is_train_mode) {
	return std::vector<double>();
}
