#include <iostream>
#include <map>
#include <vector>
#include <iomanip>

#include <Classifier.h>

void Classifier::test(const Dataset &filenames)
{
	std::cerr << "Loading model..." << std::endl;
	load_model();

	// loop through images, doing classification
	std::map<std::string, std::map<std::string, std::string> > predictions;
	for(std::map<std::string, std::vector<std::string> >::const_iterator c_iter=filenames.begin(); 
								c_iter != filenames.end(); ++c_iter) {
		for(std::vector<std::string>::const_iterator f_iter = c_iter->second.begin(); 
								f_iter != c_iter->second.end(); ++f_iter) {
			std::cerr << "Classifying " << *f_iter << "..." << std::endl;
			predictions[c_iter->first][*f_iter] = classify(*f_iter);
		}
	}
	// now score!
	std::map< std::string, std::map< std::string, double > > confusion;
	int correct=0, total=0;
	for(std::map<std::string, std::vector<std::string> >::const_iterator c_iter=filenames.begin(); 
								c_iter != filenames.end(); ++c_iter) {
		for(std::vector<std::string>::const_iterator f_iter = c_iter->second.begin(); 
								f_iter != c_iter->second.end(); ++f_iter, ++total) {
			confusion[c_iter->first][ predictions[c_iter->first][*f_iter] ]++;
		}
	}

	std::cout << "Confusion matrix:" << std::endl << std::setw(20) << " " << " ";
	for(size_t j=0; j<class_list.size(); j++)
		std::cout << std::setw(2) << class_list[j].substr(0, 2) << " ";

	for(size_t i=0; i<class_list.size(); i++)
	{
		std::cout << std::endl << std::setw(20) << class_list[i] << " ";
		for(size_t j=0; j<class_list.size(); j++)
			std::cout << std::setw(2) << confusion[ class_list[i] ][ class_list[j] ] << (j==i?".":" ");

		correct += confusion[ class_list[i] ][ class_list[i] ];
	}

	std::cout << std::endl << "Classifier accuracy: " << correct << " of " << total 
			<< " = " << std::setw(5) << std::setprecision(2) << correct/double(total)*100 << "%";
	std::cout << "  (versus random guessing accuracy of " << std::setw(5) << std::setprecision(2) 
			<< 1.0/class_list.size()*100 << "%)" << std::endl;
}

