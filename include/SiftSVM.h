#ifndef __SIFT_SVM_H__
#define __SIFT_SVM_H__

#include <SVM.h>

class SiftSVM : public SVM {
public:
	SiftSVM(const std::vector<std::string>&, const std::string&);
	~SiftSVM();

	std::vector<double> get_feature_vector(const std::string&, bool);

	void preprocess(const Dataset&);
};

#endif
