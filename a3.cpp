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

#include <errno.h>

#include <defs.h>
#include <utils.h>
#include <Classifier.h>
#include <NearestNeighbor.h>

//Use the cimg namespace to access the functions easily
using namespace cimg_library;
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
    
    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

int getdirandfiles (string dir, vector<string> &allpathnfiles)
{
    vector<string> folders = vector<string>();
    
    getdir(dir,folders);
    for (unsigned int i = 0;i < folders.size();i++) {
        
        
        //cout << folders[i] << endl;
        if(folders[i].compare(".")!=0 && folders[i].compare("..")!=0)
        {string dir = string("./train/"+folders[i]);
        
        vector<string> files = vector<string>();
        
        getdir(dir,files);
        for (unsigned int i = 0;i < files.size();i++) {
            if(files[i].compare(".")!=0 && files[i].compare("..")!=0)
                //cout << files[i] << endl;
                allpathnfiles.push_back(dir+"/"+files[i]);
            
        }}
    }
        return 0;
}
void k_means_clustering(vector< vector<SiftDescriptor> >&trainingset, int k)
{
     k=2;
    int maxitn=1000;
    vector<vector<SiftDescriptor> >centroids;
    
    int ran[k];
    ran[0]=rand()%trainingset.size();
    for(int i=1;i<k;)
    {
        int r=rand()%trainingset.size();
        bool repeat=false;
        for(int j=0;j<i;j++)
        {
            if(ran[j]==r)
            {
                repeat=true;
                break;
            }
        }
        if(!repeat){
            ran[i]=r;
            i++;
        }
    }
    
    ////initial random centroids
    for(int i=0;i<k;i++)
    {
        centroids.push_back(trainingset[ran[i]]);
    }
    cout<<trainingset.size()<<endl;
    cout<<trainingset[0].size()<<endl;
    vector<vector<SiftDescriptor> >clusters[k];
    float interblockdist=0;
    float previnterblockdist=0;
    cout<<centroids.size()<<endl;
    cout<<trainingset[0][0].descriptor.size()<<endl;
    //vector<SiftDescriptor> trainingpt;
    for(int i=0;i<maxitn;i++){
        for(unsigned int j=0;j<trainingset.size();j++)
        {
            //cout<< i <<" "<< j<<endl;
            float mindiff=10000000;
            int cluster=0;
            //trainingpt=trainingset[j];
            for(int n=0;n<centroids.size();n++)
            {

                int diff=0;
                for(unsigned int l=0;l<trainingset[j].size();l++)
                {
                    for(unsigned int m=0;m<trainingset[j][l].descriptor.size();m++)
                    {
                        cout<<i<<" "<<j<<" "<<n<<" "<<l<<" "<<m<<endl;
                        cout<<centroids.size()<<endl;
                        cout<<ran[0]<<" "<<ran[1]<<endl;
                        diff+=abs(trainingset[j][l].descriptor[m]-centroids[n][l].descriptor[m]);
                    }
                }
                if(diff<mindiff)
                {
                    mindiff=diff;
                    cluster=n;
                }
            }
            clusters[cluster].push_back(trainingset[j]);
            interblockdist+=mindiff;
        }
        cout<<"checkpoint"<<endl;
        //recalculate clusters
        vector<vector<SiftDescriptor> > newcentroids;
        for(int l=0;l<k;l++)
        {
            vector<SiftDescriptor> newcenter;
            int clustersize=clusters[l].size();
            for(int n=0;n<clustersize;n++)
            for(int j=0;j<128;j++)
            {
                //newcenter.descriptor[j]=+(clusters[l][n].descriptor[j]/clustersize);
            }
            //push newcentroid to vec
            clusters[l].clear();
        }
        centroids=newcentroids;
        //stopping condition:
        if(i>1)
        {
            if(interblockdist/previnterblockdist<0.01)
                break;
        }

        previnterblockdist=interblockdist;
    }
    ////answer in clusters!
    
}
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
        //for(int i=0;i<filenames.size();i++)
        //cout<<filenames[i];
        cout<<filenames.size();
        cout<<"HI";
		// set up the classifier based on the requested algo
		Classifier *classifier=0;
		if(algo == "nn")
			classifier = new NearestNeighbor(class_list);
        
        else if(algo == "kmeans" && mode=="train")
        {
            cout<<"Hey";
            string dir = string("./train");
            int w=10;
            int k=2;
            vector<string> allpathnfiles = vector<string>();
            vector<vector<SiftDescriptor> >trainingvecs=vector<vector<SiftDescriptor> >();
            //function into vector of vectors
            getdirandfiles(dir,allpathnfiles);
            for(unsigned int i=0;i<allpathnfiles.size();i++)
            {
                CImg<double> input(allpathnfiles[i].c_str());
                CImg<double> resized = input.resize(w, w, 1, 3);
                
                CImg<double> grey = resized.spectrum() == 1? resized: resized.get_RGBtoHSI().get_channel(2);
                vector<SiftDescriptor> d = Sift::compute_sift(grey);

                trainingvecs.push_back(d);
                //till here
            }
            k_means_clustering(trainingvecs,k);
            
        }
		
        else if(algo == "ef" && mode=="train")
            //classifier = new NearestNeighbor(class_list);
        {
            cout<<"Hey";
            string dir = string("./train");
            
            int w=10;
            int eigenmax=20;
            
            vector<string> allpathnfiles = vector<string>();
            vector<vector<double> > trainingvecs=vector<vector<double> >();
//function into vector of vectors
            getdirandfiles(dir,allpathnfiles);
            for(unsigned int i=0;i<allpathnfiles.size();i++)
            {
            CImg<double> input(allpathnfiles[i].c_str());
            CImg<double> resized = input.resize(w, w, 1, 3);
                
            CImg<double> grey = resized.spectrum() == 1? resized: resized.get_RGBtoHSI().get_channel(2);
            std::vector<double> result(grey.width() * grey.height(), 0);
            std::copy(grey.begin(), grey.end(), result.begin());
                //cout<<grey.size()<<endl;
                trainingvecs.push_back(result);
            
//till here
            }
            cout<<trainingvecs.size();
            vector<double> avg=vector<double>(trainingvecs[0].size(),0);
            int len=trainingvecs.size();
            for(unsigned int i=0;i<avg.size();i++)
                cout<<avg[i]<<" ";

            for(int i=0;i<len;i++)
            {cout<<i<<endl;
                for(unsigned int j=0;j<trainingvecs[i].size();j++)
                    
                {
                    //cout<<trainingvecs[i][j]<<" ";

                avg[j]=avg[j]+(trainingvecs[i][j]/len);}
                //cout<<endl;
            }
            for(int i=0;i<len;i++)
            {//cout<<i<<endl;
                for(unsigned int j=0;j<trainingvecs[i].size();j++)
                    
                {
                    //cout<<trainingvecs[i][j]<<" ";
                    
                    trainingvecs[i][j]=trainingvecs[i][j]-avg[j];}
                //cout<<endl;
            }
            int datasize=trainingvecs.size();
            int vecsize=avg.size();
            CImg<double> mat(datasize,vecsize);
            //cout<<mat.width()<<" "<<mat.height()<<" "<<mat.depth()<<" "<<mat.spectrum<<endl;
            //cout<<mat.width()<<endl;
            //cout<<mat.height()<<endl;
        
            cout<<"Transform vec to CIMG"<<endl;
            for(int i=0;i<datasize;i++)
            {
                for(int j=0;j<vecsize;j++)
                    mat(i,j)=trainingvecs[i][j];
            }
            cout<<"Copleted transform vec to CIMG"<<endl;
            cout<<mat.width();
            cout<< " ";
            cout<< mat.height();
            cout<< " ";
            cout<< mat.depth();
            cout<< " " ;
            cout<< mat.spectrum();
            cout<< endl;

            CImg<double> t(mat,false);
            t.transpose();
            cout<<t.width();
            cout<< " ";
            cout<< t.height();
            cout<< " ";
            cout<< t.depth();
            cout<< " " ;
            cout<< t.spectrum();
            cout<< endl;

            //cout<<t.width()<<" "<<t.height()<<" "<<t.depth()<<" "<<t.spectrum<<endl;

            CImg<double> covmat=mat*t;
            cout<<covmat.width();
            cout<< " ";
            cout<< covmat.height();
            cout<< " ";
            cout<< covmat.depth();
            cout<< " " ;
            cout<< covmat.spectrum();
            cout<< endl;
            
            CImg<float> egval;
            CImg<float> egvec;
            
            covmat.symmetric_eigen(egval,egvec);

            cout<<egval.width();
            cout<< " ";
            cout<< egval.height();
            cout<< " ";
            cout<< egval.depth();
            cout<< " " ;
            cout<< egval.spectrum();
            cout<< endl;

            cout<<egvec.width();
            cout<< " ";
            cout<< egvec.height();
            cout<< " ";
            cout<< egvec.depth();
            cout<< " " ;
            cout<< egvec.spectrum();
            cout<< endl;
            cout<< endl;
            cout<< endl;
            cout<< endl;
            cout<< endl;
            /*for(int i=0;i<100;i++)
            {
                for(int j=0;j<100;j++)
                cout<<egvec(i,j)<<"**";
                cout<<endl;
            }
            */
            /* to save the eigen vector imagesss*/
            /*start here*/
            for(int i=0;i<eigenmax;i++)
            {   cout<<i<<" "<<egval(0,i)<<endl;
                
                    int maxpixel=egvec(0,i);
                    CImg<double> eigenfood(w,w);
                
                    for(int j=0;j<w;j++)
                        for(int k=0;k<w;k++)
                        {
                            eigenfood(j,k)=egvec(j*10+k,i);
                            if(maxpixel<eigenfood(j,k))
                                maxpixel=eigenfood(j,k);
                            //cout<<j<<" "<<k<<" "<<j*10+k<<" "<<i<<" "<<egvec(j*10+k,i)<<" ";
                        }
                
                cout<<endl;
                cout<<maxpixel;
                        for(int j=0;j<w;j++)
                            for(int k=0;k<w;k++)
                                eigenfood(j,k)/=maxpixel;

                                    
                if(i==0)
                        eigenfood.save("eigenfood.png");
                
                
                
                cout<<endl;

            }
            
            /*end here*/
            
            
            /*create a new Cimg for solver to use*/
            /////here start

            CImg<double> eigenvecforsolve(eigenmax,w*w);
            for(int j=0;j<eigenmax;j++)
                for(int i=0;i<w*w;i++)
                    eigenvecforsolve(j,i)=egvec(j,i);
            cout<< endl;

            cout<<eigenvecforsolve.width();
            cout<< " ";
            cout<< eigenvecforsolve.height();
            cout<< " ";
            cout<< eigenvecforsolve.depth();
            cout<< " " ;
            cout<< eigenvecforsolve.spectrum();
            cout<< endl;
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //global stuff always neeeded! Cimg<double>eigenvecforsolve & vector<double>avg
            std::string filename1="./train/bagel/106970.jpg";
            CImg<double> input(filename1.c_str());
            CImg<double> resized = input.resize(w, w, 1, 3);
            
            CImg<double> grey = resized.spectrum() == 1? resized: resized.get_RGBtoHSI().get_channel(2);
            //std::vector<double> result(grey.width() * grey.height(), 0);
            //std::copy(grey.begin(), grey.end(), result.begin());
            CImg<double> lhs(1,w*w);
            
            
            cout<<lhs.width();
            cout<< " ";
            cout<< lhs.height();
            cout<< " ";
            cout<< lhs.depth();
            cout<< " " ;
            cout<< lhs.spectrum();
            cout<< endl;
            for(int i=0;i<w;i++)
                for(int j=0;j<w;j++)
                {
                    lhs(0,i*10+j)=grey(i,j)-avg[i*10+j];
                }
            CImg<double> ans=lhs.solve(eigenvecforsolve);
            cout<<ans.width();
            cout<< " ";
            cout<< ans.height();
            cout<< " ";
            cout<< ans.depth();
            cout<< " " ;
            cout<< ans.spectrum();
            cout<< endl;
            cout<<"answer";
            for(int i=0;i<ans.width();i++)
                for(int j=0;j<ans.height();j++)
                    cout<<ans(i,j)<<endl;
            

            std::vector<double> finalresult(ans.width() * ans.height(), 0);

            std::copy(ans.begin(), ans.end(), finalresult.begin());
            cout<<"finalresult"<<endl;
            for(unsigned int i=0;i<finalresult.size();i++)
                cout<<finalresult[i]<<endl;

            
            
            //for(int i=0;i<grey)
            
           
    }
    
        else
            throw std::string("unknown classifier " + algo);
		// now train or test!
		/*if(mode == "train")
			classifier->train(filenames);
		else if(mode == "test")
			classifier->test(filenames);
		else
			throw std::string("unknown mode!");*/

		delete classifier;
	}
	catch(const string &err) {
		cerr << "Error: " << err << endl;
	}
}


