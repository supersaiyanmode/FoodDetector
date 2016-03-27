#ifndef __EIGEN_SVM_H__
#define __EIGEN_SVM_H__

#include <SVM.h>

class EigenSVM : public SVM {
public:
	EigenSVM(const std::vector<std::string>&, const std::string&);
	~EigenSVM();

	void preprocess(const Dataset&);

	std::vector<double> get_feature_vector(const std::string&, bool);
};

#endif
