#ifndef __BOV_SVM_H__
#define __BOV_SVM_H__

#include <SVM.h>

class BOVSVM : public SVM {
public:
	BOVSVM(const std::vector<std::string>&, const std::string&);
	~BOVSVM();

	std::vector<double> get_feature_vector(const std::string&, bool);
};

#endif
