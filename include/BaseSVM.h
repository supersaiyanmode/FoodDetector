#ifndef __EIGEN_SVM_H__
#define __EIGEN_SVM_H__

#include <SVM.h>

class BaseSVM : public SVM {
public:
	BaseSVM(const std::vector<std::string>&, const std::string&);
	~BaseSVM();

	std::vector<double> get_feature_vector(const std::string&);
};

#endif