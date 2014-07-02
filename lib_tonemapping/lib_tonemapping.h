#include <stdio.h>
#include <iostream>
 
#include<opencv/highgui.h>
#include<opencv/cxcore.h>
#include<opencv/cv.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
 
using namespace cv;

namespace lib_tonemapping {
	public ref class Tonemap
	{
		public :char* logmap(char* imgpath)
		{
			int i,j;int max=0,max1=0;int dmax = 255;
			float b = 0.6;float temp_b,temp_1,temp_2,Y,y_max,V;float temp_gamma,temp_gam2,temp_gam3;
			unsigned char B,G,R;float temp_fix;int p[3];
			IplImage *img_data,*img_rec,*img_temp,*img_hsv,*imgdata;
			IplImage* img = cvLoadImage(imgpath, CV_LOAD_IMAGE_COLOR);

			printf("Image depth: %d\n",img->depth);

			img_data = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
			img_rec = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
			img_temp = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
			img_hsv = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
			imgdata = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);

			/*if (!img)
				{
					printf("Image can NOT Load!!!\n");
					return 1;
				}
				*/
			printf("Channel sequence:%s \n",img->channelSeq);
			printf("Height: %d\nwidth: %d\nnchannels:%d\n",img->height,img->width,img->nChannels);
			cvCvtColor(img,img_data,CV_BGR2XYZ);
			cvCvtColor(img,img_hsv,CV_BGR2HSV);
			printf("Channel sequence:%s \n",img_hsv->channelSeq);


			for(i=0;i<img_data->height;i++)
			{
				for(j=0;j<img_data->width;j++)
				{
					CvScalar pixelVal = cvGet2D( img_data, i, j);
					unsigned char Y = pixelVal.val[1];

					if(Y>max)
						max=Y;

				}
			}

			for(i=0;i<img_hsv->height;i++)
			{
				for(j=0;j<img_hsv->width;j++)
				{
					CvScalar pixelVal = cvGet2D( img_hsv, i, j);
					unsigned char V = pixelVal.val[2];

					if(V>max1)
						max1=V;

				}
			}

			printf("Max1: %d\n",max1);
			printf("Max: %d\n",max);


			temp_b = (float)log(b)/log(0.5);
			temp_1 = dmax*0.01*log(10.0);
			temp_2 = log(max+1.0);
			//float temp_2 = log(max1+1);


			for(i=0;i<img_data->height;i++)
			{
			for(j=0;j<img_data->width;j++)
			{
			CvScalar pixelVal = cvGet2D( img_data, i, j);
			Y = (float)pixelVal.val[1]/max;
			y_max = (float)Y/max;

			pixelVal.val[1] = (((float)temp_1/temp_2)*((float)log(Y+1)/log(2+.8*pow(y_max,temp_b))))*max;
			//printf("original: %d\t converted:%f\n",Y,pixelVal.val[1]);
			cvSet2D(img_data,i,j,pixelVal);

			}
			}

			temp_2 = log(max1+1.0);
			for(i=0;i<img_hsv->height;i++)
			{
				for(j=0;j<img_hsv->width;j++)
				{
				CvScalar pixelVal = cvGet2D( img_hsv, i, j);
				V= pixelVal.val[2]/max1;
				//printf("V:%f\n",V);
				y_max = (float)V/max1;

				pixelVal.val[2] = ((float)temp_1/temp_2)*((float)log(V+1)/log(2+.8*pow(y_max,temp_b)))*max1;
				//printf("originalh: %f\t convertedh:%f\n",V,pixelVal.val[2]);
				cvSet2D(img_hsv,i,j,pixelVal);
				}
			}

			cvCvtColor(img_data,img_rec,CV_XYZ2BGR);
			cvCvtColor(img_hsv,img_temp,CV_HSV2BGR);
			//cvNamedWindow("original", CV_WINDOW_FREERATIO );
			//cvShowImage("original", img);

   
			for(i=0;i<img_temp->height;i++)
			{
				for(j=0;j<img_temp->width;j++)
				{
					CvScalar pixelVal = cvGet2D( img_rec, i, j);
					B = pixelVal.val[0];
					G = pixelVal.val[1];
					R = pixelVal.val[2];

					temp_gamma = (float)B/255;
					temp_gam2 = (float)G/255;
					temp_gam3 = (float)R/255;

					if(temp_gamma<=0.018)
					{
						pixelVal.val[0] = 4.5*temp_gamma*255;
					}
					else
					{
						pixelVal.val[0] = (1.099*pow(temp_gamma*1.0,0.45)-0.099)*255;
					}
					if(temp_gam2<=0.018)
					{
					pixelVal.val[1] = 4.5*temp_gam2*255;
					}
					else
					{
					pixelVal.val[1] = (1.099*pow(temp_gam2*1.0,0.45)-0.099)*255;
					}
	

					if(temp_gam3<=0.018)
					{
					pixelVal.val[2] = 4.5*temp_gam3*255;
					}
					else
					{
					pixelVal.val[2] = (1.099*pow(temp_gam3*1.0,0.45)-0.099)*255;
					}

					cvSet2D(img_rec,i,j,pixelVal);

				}
			}
	
			//cvNamedWindow("Corrected", CV_WINDOW_FREERATIO );
			//cvShowImage("Corrected", img_rec);
			//cvNamedWindow("hsv", CV_WINDOW_FREERATIO );
			//cvShowImage("hsv", img_temp);

	
			p[0] = CV_IMWRITE_JPEG_QUALITY;
			p[1] = 100;
			p[2] = 0;

			cvWaitKey(0);
			cvReleaseImage(&img_rec);
			cvReleaseImage(&img_data);
			cvSaveImage("out.jpg",img_temp,p);
	

			   return "out.jpg";
		}
	};
}