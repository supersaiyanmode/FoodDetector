// B657 assignment 3 skeleton code, D. Crandall
//
// Compile with: "make"
//
// This skeleton code implements nearest-neighbor classification
// using just matching on raw pixel values, but on subsampled "tiny images" of
// e.g. 20x20 pixels.
//
// It defines an abstract Classifier class, so that all you have to do
// :) to write a new algorithm is to derive a new class from
// Classifier containing your algorithm-specific code
// (i.e. load_model(), train(), and classify() methods) -- see
// NearestNeighbor.h for a prototype.  So in theory, you really
// shouldn't have to modify the code below or the code in Classifier.h
// at all, besides adding an #include and updating the "if" statement
// that checks "algo" below.
//
// See assignment handout for command line and project specifications.
//
#include "CImg.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Sift.h>
#include <sys/types.h>
#include <dirent.h>
#include <map>
#include <numeric>


#include <defs.h>
#include <utils.h>
#include <Classifier.h>
#include <NearestNeighbor.h>
#include <SVM.h>
#include <BaseSVM.h>

//Use the cimg namespace to access the functions easily
using namespace cimg_library;

int main(int argc, char **argv)
{
	try {
		if(argc < 3)
			throw string("Insufficent number of arguments");

		std::string mode = argv[1];
		std::string algo = argv[2];

		// Scan through the "train" or "test" directory (depending on the
		//  mode) and builds a data structure of the image filenames for each class.
		Dataset filenames; 
		std::vector<std::string> class_list = list_files(mode);
		for(std::vector<std::string>::const_iterator c = class_list.begin(); c != class_list.end(); ++c)
			filenames[*c] = list_files(mode + "/" + *c, true);

		// set up the classifier based on the requested algo
		Classifier *classifier=0;
		if(algo == "nn")
			classifier = new NearestNeighbor(class_list);
		else if (algo == "eigen")
			classifier = new BaseSVM(class_list, "svm-test");
		else
			throw std::string("unknown classifier " + algo);

		// now train or test!
		if(mode == "train")
			classifier->train(filenames);
		else if(mode == "test")
			classifier->test(filenames);
		else
			throw std::string("unknown mode!");

		delete classifier;
	}
	catch(const string &err) {
		cerr << "Error: " << err << endl;
	}
}


