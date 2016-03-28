#ifndef __DEEP_SVM_H__
#define __DEEP_SVM_H__

#include <SVM.h>

class DeepSVM : public SVM {
public:
	DeepSVM(const std::vector<std::string>&, const std::string&);
	~DeepSVM();

	std::vector<double> get_feature_vector(const std::string&, bool);

	void preprocess(const Dataset&);
};

#endif
