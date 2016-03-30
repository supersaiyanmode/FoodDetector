/*
 * HaarSVM.h
 *
 *  Created on: Mar 27, 2016
 *      Author: sunkenglory
 */

#ifndef INCLUDE_HAARSVM_H_
#define INCLUDE_HAARSVM_H_
#include<vector>
#include <SVM.h>
#include <HaarRow.h>

using namespace std;

class HaarSVM: public SVM {
private:
	vector<HaarRow> windows;
public:
	HaarSVM(const std::vector<std::string>&, const std::string&);
	~HaarSVM();
	std::vector<double> get_feature_vector(const std::string&, bool);

	void preprocess(const Dataset&);
	void setWindowVector(const vector<HaarRow>& win) {
		windows = win;
	}
	void load_model();

	std::vector<HaarRow> getWindowVector() {
		return windows;
	}
};

#endif /* INCLUDE_HAARSVM_H_ */
