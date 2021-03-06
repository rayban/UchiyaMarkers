//
//  imgProcGM.cpp
//  RandomDotMarkers
//
//  Created by Guillaume Moreau on 19/05/11.
//  Copyright 2011 Ecole Centrale de Nantes. All rights reserved.
//
#include "Skeletonize.h"
#include "Intersections.h"

#include <iostream>

using namespace std;


#include "imgProcGM.h"

imgProcGM::imgProcGM(IplImage *src) : Skeletonize(src)
{
   // nothing
}


void imgProcGM::processImage(IplImage *colorSrcImg, int threshold)
{
   IplImage *graySrcImg = cvCreateImage(cvGetSize(colorSrcImg), IPL_DEPTH_8U, 1);
   
   IplImage *hsvSrcImg = cvCreateImage(cvGetSize(colorSrcImg), IPL_DEPTH_8U, 3);
   // Source Image in color
   cvCvtColor(colorSrcImg, hsvSrcImg, CV_BGR2YUV);
   
   //cvShowImage("YUV", hsvSrcImg);
   
   cvSplit(hsvSrcImg,graySrcImg,NULL,NULL,NULL);
   cvThreshold(graySrcImg, graySrcImg, threshold, 255, CV_THRESH_BINARY);

   //cvEqualizeHist(graySrcImg, graySrcImg);
   //cvShowImage("Y", graySrcImg);
   
   // Applying Threshold + Morphological Closing
   //cvThreshold(graySrcImg, graySrcImg, threshold, 255, CV_THRESH_BINARY);
   /*cvMorphologyEx(graySrcImg, graySrcImg, NULL,
    cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_RECT),
    CV_MOP_OPEN);*/
   cvMorphologyEx(graySrcImg, graySrcImg, NULL,
                  cvCreateStructuringElementEx(3, 3, 2, 2, CV_SHAPE_RECT),
                  CV_MOP_CLOSE,
                  2);
   
   cvShowImage("Binarization", graySrcImg);
   
   // Applying the Skeletonization
   m_src = graySrcImg;
   skeletonize();
   cvShowImage("Skeletonization", graySrcImg);
   
   // Computing Intersections and Drawing in Red them on the original Image
   vector<CvPoint> crossroads = Intersections(graySrcImg)();
   for (unsigned int i = 0; i < crossroads.size(); ++i)
   {
      cvCircle(colorSrcImg, crossroads[i], 2, CV_RGB(255, 0, 0), -1);
   }
   
   cvReleaseImage(&hsvSrcImg);
   cvReleaseImage(&graySrcImg);

}
