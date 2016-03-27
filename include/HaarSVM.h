/*
 * HaarSVM.h
 *
 *  Created on: Mar 27, 2016
 *      Author: sunkenglory
 */

#ifndef INCLUDE_HAARSVM_H_
#define INCLUDE_HAARSVM_H_

#include <SVM.h>

class HaarSVM: public SVM {
public:
	HaarSVM(const std::vector<std::string>&, const std::string&);
	~HaarSVM();

	std::vector<double> get_feature_vector(const std::string&, bool);

	void preprocess(const Dataset&) {
	}

};

#endif /* INCLUDE_HAARSVM_H_ */
