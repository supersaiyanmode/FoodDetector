#ifndef __NEAREST_NEIGHBOR_H__
#define __NEAREST_NEIGHBOR_H__

#include <vector>
#include <string>

#include <CImg.h>

#include <Classifier.h>

class NearestNeighbor : public Classifier
{
public:
	NearestNeighbor(const std::vector<std::string> &_class_list) : Classifier(_class_list) {}
	~NearestNeighbor() {}

	void load_model();
	std::string classify(const std::string &);
	void train(const Dataset&);
	// Nearest neighbor training. All this does is read in all the images, resize
	// them to a common size, convert to greyscale, and dump them as vectors to a file
protected:
	// extract features from an image, which in this case just involves resampling and 
	// rearranging into a vector of pixel data.
	
	cimg_library::CImg<double> extract_features(const std::string &filename);

	static const int size=20;  // subsampled image resolution
	std::map<std::string, cimg_library::CImg<double> > models; // trained models
};

#endif

