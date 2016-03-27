#ifndef __EIGEN_SVM_H__
#define __EIGEN_SVM_H__

#include <SVM.h>

class HaarSVM: public SVM {
public:
	HaarSVM(const std::vector<std::string>&, const std::string&);
	~HaarSVM();

	std::vector<double> get_feature_vector(const std::string&, bool);
};

#endif
