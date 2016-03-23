#include <SVM.h>

SVMCore::SVMCore(const std::string& str, bool is_training_mode) {

}

SVMCore::~SVMCore() {

}

SVM::SVM(const std::vector<std::string>& cl): Classifier(cl) {
		
}

SVM::~SVM() {
	for (std::map<std::string, SVMCore*>::iterator it = svm_cores.begin(); 
						it != svm_cores.end(); it++ ) {
		delete it->second;
	}
}

void SVM::train(const Dataset& filenames) {

}

void SVM::load_model() {

}

std::string SVM::classify(const std::string&) {
	return "";
}

std::vector<double> SVM::get_feature_vector(const std::string& filename) {
	return std::vector<double>();
}

