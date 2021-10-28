#include "iostream"
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main() {
    Mat img = imread("C:\\Users\\lenovo\\Desktop\\2022\\2022\\task_4.png");
    if (img.empty()) {
        cout << "Error" << endl;
        return -1;
    }
    imshow("1", img);
    Mat image;
    GaussianBlur(img,image,Size(3,3),0);
    cvtColor(image,image,COLOR_RGB2GRAY);
    imshow("2",image);
    Canny(image,image,100,250);
    imshow("3",image);
    vector<vector<Point>> contours;
    vector<vector<Point>> contours_ex;
    findContours(image,contours_ex,RETR_EXTERNAL,CHAIN_APPROX_NONE);
    findContours(image,contours,RETR_TREE,CHAIN_APPROX_NONE);
    for (int i = 0; i < contours_ex.size(); i++)
    {
        int conloursize = contours_ex[i].size();
        for (int j = 0; j < contours.size(); j++)
        {
            int tem_size = contours[j].size();
            if (conloursize == tem_size)
            {
                swap(contours[j], contours[contours.size() - 1]);
                contours.pop_back();
                break;
            }
        }
    }
    Mat mask=Mat::zeros(image.size(),CV_8UC1);
    Mat Contours=Mat::zeros(image.size(),CV_8UC1);
    Mat mask0=Mat::zeros(image.size(),CV_8UC1);
    drawContours(Contours,contours_ex,-1, Scalar(255));
    drawContours(mask,contours,0,Scalar(255));
    imshow("4",Contours);
    imshow("5",mask);
    RotatedRect rect=minAreaRect(contours[0]);
    Point2f vertex[4];
    rect.points(vertex);
    for(int i=0;i<4;i++){
        line(mask0,vertex[i],vertex[(i+1)%4],Scalar(255));
    }
    vector<vector<Point>> rect_;
    findContours(mask0,rect_,RETR_EXTERNAL,CHAIN_APPROX_NONE);
    drawContours(mask0,rect_,-1,Scalar(255),FILLED);
    imshow("6",mask0);
    Mat image0(img.rows,img.cols,CV_8UC1);
    img.copyTo(image0,mask0);
    imshow("7",image0);
    imwrite("C:\\Users\\lenovo\\Desktop\\2022\\2022\\result.jpg",image0);

    waitKey();
    return 0;
}

