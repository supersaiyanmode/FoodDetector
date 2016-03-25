#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <iterator>

#include <SVM.h>

SVMCore::SVMCore(const std::string& wd, bool is_training_mode): working_dir(wd) {

}

SVMCore::~SVMCore() {

}

SVM::SVM(const std::vector<std::string>& cl, const std::string& wd): 
			Classifier(cl), working_dir(wd) {
	cache_dir = wd + "/_cache";

	(void)system(("rm -rf " + wd).c_str());
	(void)system(("mkdir " + wd).c_str());
	(void)system(("mkdir " + cache_dir).c_str());

	for (std::vector<std::string>::const_iterator it = cl.begin();
					it != cl.end(); it++) {
		svm_classifiers[*it] = new SVMCore(wd + "/" + *it, false);
		svm_trainers[*it] = new SVMCore(wd + "/" + *it, true);
	}
}

SVM::~SVM() {
	for (std::map<std::string, SVMCore*>::iterator it = svm_classifiers.begin(); 
						it != svm_classifiers.end(); it++ ) {
		delete it->second;
	}

	for (std::map<std::string, SVMCore*>::iterator it = svm_trainers.begin(); 
						it != svm_trainers.end(); it++ ) {
		delete it->second;
	}
}

void SVM::train(const Dataset& data) {
	for (Dataset::const_iterator it = data.begin(); it != data.end(); it++) {
		std::string cur_food_name = it->first;
		const std::vector<std::string>& cur_files = it->second;
		std::cout<<"Processing: "<<cur_food_name<<std::endl;
		std::ofstream out((working_dir + "/" + cur_food_name + ".svm").c_str());

		//Positive examples
		for (std::vector<std::string>::const_iterator it1 = cur_files.begin(); it1 != cur_files.end(); it1++) {
			std::cout<<"   Processing: "<<*it1<<std::endl;
			int feature_pos = 1;
			std::vector<double> features = get_feature_vector_cache(*it1, true);
			out<<"+1 ";
			for (std::vector<double>::iterator it2 = features.begin(); it2 != features.end(); it2++) {
				out<<feature_pos++<<":"<<*it2<<" ";
			}
			out<<std::endl;
		}
		
		//Negative examples
		for (Dataset::const_iterator it1 = data.begin(); it1 != data.end(); it1++) {
			if (it1->first == cur_food_name)
				continue;

			for (std::vector<std::string>::const_iterator it2 = it1->second.begin(); 
									it2 != it1->second.end(); it2++) {
				int feature_pos = 1;
				std::vector<double> features = get_feature_vector_cache(*it2, true);
				out<<"-1 ";
				for (std::vector<double>::iterator it3 = features.begin(); it3 != features.end(); it3++) {
					out<<feature_pos++<<":"<<*it3<<" ";
				}
				out<<std::endl;
			}
		}
		out.close();

		std::string outputModel = working_dir + "/" + cur_food_name + ".model.svm";
		svm_core.train(inputFile, outputModel);
	}
}

/*
 * Implements a file system based cache.
 */
std::vector<double> SVM::get_feature_vector_cache(const std::string& filename, bool train) {
	std::cout<<"Trying: "<<filename<<std::endl;
	std::string cached_name = filename;

	//Reference: http://stackoverflow.com/a/2896627/227884
	std::replace(cached_name.begin(), cached_name.end(), '/', '_');

	std::ifstream inFile((cache_dir + "/" + cached_name).c_str());
	std::vector<double> result;
	if (inFile.good()) {
		//Reference: http://stackoverflow.com/a/19602431/227884
		std::cout<<"Loaded "<<filename<<" from FS Cache."<<std::endl;
		std::istream_iterator<double> it(inFile);
		std::copy(it, std::istream_iterator<double>(), std::back_inserter(result));
		return result;
	}
	inFile.close();

	result = get_feature_vector(filename, train);
	std::ofstream outFile((cache_dir + "/" + cached_name).c_str());
	//Reference: http://stackoverflow.com/a/6406411/227884
	std::copy(result.begin(), result.end(), std::ostream_iterator<double>(outFile, " "));
	outFile.close();
	return result;
}

void SVM::load_model() {

}

std::string SVM::classify(const std::string&) {
	return "";
}

