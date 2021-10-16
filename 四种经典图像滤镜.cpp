#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace cv;
using namespace std;
Mat src1,src2, img;//漏光效果图像声明---原图、模板、融合图像

//交叉滤镜函数
void cross(Mat src)
{
    int width = src.rows, height = src.cols;//原图行数列数

    Mat img(src.size(), CV_8UC3);//创建一个CV_8UC3类型，大小和原图一样的图像 。  CV_8UC3：3通道*8字节

    int x, y;
    uchar* P0, * P1;
    for (y = 0; y < height; y++) //图片的高度
    {
        P0 = src.ptr<uchar>(y);//原图像素首行的首地址（y，0）地址
        P1 = img.ptr<uchar>(y);//目标图像素首行的首地址（y，0）地址
        for (x = 0; x < width; x++)//列
        {
            //原图像素分量
            float R = P0[3 * x + 2];//R
            float G = P0[3 * x + 1];//G
            float B = P0[3 * x];//B

          //运用公式，使用中间量存储运算结果
            float newR, newG;
            float newB = B / 2 + 0x25;//0x25 = 37

            if (G < 128)
                newG = G * G / 128;
            else
                newG = 256 - (256 - G) * (256 - G) / 128;

            if (R < 128)
                newR = R * R * R / 16384;
            else
                newR = 256 - (256 - R) * (256 - R) * (256 - R) / 16384;

            //中间量赋值给目标图像
            P1[3 * x] = (uchar)MIN(255, MAX(0, newB));
            P1[3 * x + 1] = (uchar)MIN(255, MAX(0, newG));
            P1[3 * x + 2] = (uchar)MIN(255, MAX(0, newR));
        }

        namedWindow("原始图像", CV_WINDOW_AUTOSIZE);
        imshow("原始图像", src);
        namedWindow("交叉风格滤镜", CV_WINDOW_AUTOSIZE);
        imshow("交叉风格滤镜", img);
    }

    imwrite("E:\\vsobjects\\数字图像课程设计\\result_image\\交叉滤镜.jpg", img);
}

//怀旧滤镜函数
void recall(Mat src) {
    int width = src.rows, height = src.cols;//原图行数
    Mat img(src.size(), CV_8UC3);//创建和src原图一样的3通道8字节Mat图像

    int x, y;
    uchar* P0, * P1;
    for (y = 0; y < height; y++) //图片的高度-行数y
    {
        P0 = src.ptr<uchar>(y);//原图像素首行的首地址（y，0）地址
        P1 = img.ptr<uchar>(y);//目标图像素首行的首地址（y，0）地址

        for (x = 0; x < width; x++) //列
        {
            float R = P0[3 * x + 2];//R
            float G = P0[3 * x + 1];//G
            float B = P0[3 * x];//B
            //运用公式，并创建另外一组R G B变量存储结果，限制在255内
            float newR = MIN(0.393 * R + 0.769 * G + 0.189 * B, 255);
            float newG = MIN(0.349 * R + 0.686 * G + 0.168 * B, 255);
            float newB = MIN(0.272 * R + 0.534 * G + 0.131 * B, 255);
            //结果
            P1[3 * x] = (uchar)newB;
            P1[3 * x + 1] = (uchar)newG;
            P1[3 * x + 2] = (uchar)newR;
        }
       /* namedWindow("原始图像", CV_WINDOW_AUTOSIZE);
        imshow("原始图像", src);*/
        namedWindow("怀旧滤镜", CV_WINDOW_AUTOSIZE);
        imshow("怀旧滤镜", img);

    }
    imwrite("E:\\vsobjects\\数字图像课程设计\\result_image\\怀旧滤镜.jpg", img);
}

//光照效果函数
void Light(Mat src, int X, int Y, float r, float K)//原图、光照点、半径、光照系数
{
    int width = src.rows, height = src.cols;//原图行数 

    Mat img(src.size(), CV_8UC3);//创建一个同样大小的Mat类型的img图像、

    int x, y;//用来表示图像的行和列
    unsigned char* P0, * P1;

    for (y = 0; y < height; y++) //图片的高度--行数y
    {
        P0 = src.ptr<uchar>(y);//原图像素（y，0）地址
        P1 = img.ptr<uchar>(y);//目标图像（y，0）地址
        for (x = 0; x < width; x++) //列
        {
            //中间量，用来存储公式运算的目标图像像素结果
            float newR, newB, newG;

            //原始图像的3个像素分量
            float R = P0[3 * x + 2];//R
            float G = P0[3 * x + 1];//G
            float B = P0[3 * x];//B

            float distance = sqrt((X - x) * (X - x) + (Y - y) * (Y - y)); //光源到像素点距离
            float result = K * MAX(0, 1 - distance / r);//光照值
            if (distance < r) //光源点到像素点的距离<光源的半径r 时
            {
                //公式
                newB = B + result;
                newG = G + result;
                newR = R + result;
                //将运算结果赋值给目标图像的像素地址 
                P1[3 * x] = (uchar)MIN(255, MAX(0, newB));
                P1[3 * x + 1] = (uchar)MIN(255, MAX(0, newG));
                P1[3 * x + 2] = (uchar)MIN(255, MAX(0, newR));
            }
            else
            {
                //光源无法到达像素点的时候
                //像素值为原始值
                newB = B;
                newG = G;
                newR = R;
                P1[3 * x] = (uchar)MIN(255, MAX(0, newB));
                P1[3 * x + 1] = (uchar)MIN(255, MAX(0, newG));
                P1[3 * x + 2] = (uchar)MIN(255, MAX(0, newR));
            }
        }
       /* namedWindow("原始图像", CV_WINDOW_AUTOSIZE);
        imshow("原始图像", src);*/
        namedWindow("光照效果滤镜", CV_WINDOW_AUTOSIZE);
        imshow("光照效果滤镜", img);
    }
    //写入文件中，存储结果图片
    imwrite("E:\\vsobjects\\数字图像课程设计\\result_image\\光照效果滤镜.jpg", img);
}



//漏光函数图像
void Lightleak(Mat src1, Mat src2) {
    Mat img;//融合结果图像
    int width = src1.rows, height = src1.cols;//原图行数、列数
    uchar* s1;//原图指针
    uchar* s2;//模板指针
    uchar* d;//融合结果图指针
    img.create(src1.size(), CV_8UC3);//创建一个同样大小的Mat类型的img图像

    int x, y;//图像的行和列
    if (src1.isContinuous() && src2.isContinuous())
    {
        for (y = 0; y < height; y++) {//图片的高度

            s1 = src1.ptr<uchar>(y);//原图像素首行的首地址（y，0）地址
            s2 = src2.ptr<uchar>(y);//目标图像素首行的首地址（y，0）地址
            d = img.ptr<uchar>(y);//目标图像素首行的首地址（y，0）地址
            for (x = 0; x < width; x++)//列
            {
                //img为目标图像，它和两张原始图像一样大
                img.create(src1.rows, src1.cols, CV_8UC3);
                //原始图像变量
                float R1 = s1[3 * x + 2];//R
                float G1 = s1[3 * x + 1];//G
                float B1 = s1[3 * x];//B
                //模板变量
                float R2 = s2[3 * x + 2];//R
                float G2 = s2[3 * x + 1];//G
                float B2 = s2[3 * x];//B
                float newR, newG, newB;//融合的图像像素分量
                //运用公式
                if (R1 <= 128) newR = R1 * R2 / 128;  else newR = 255 - ((255 - R1) * (255 - R2)) / 128;
                if (G1 <= 128) newG = G1 * G2 / 128;  else newG = 255 - ((255 - G1) * (255 - G2)) / 128;
                if (B1 <= 128) newB = B1 * B2 / 128;  else newB = 255 - ((255 - B1) * (255 - B2)) / 128;
                //最终的图像像素赋值
                d[3 * x] = (uchar)MIN(255, MAX(0, newB));
                d[3 * x + 1] = (uchar)MIN(255, MAX(0, newG));
                d[3 * x + 2] = (uchar)MIN(255, MAX(0, newR));
                /*if (src1.isContinuous() && src2.isContinuous() && img.isContinuous())
                {
                    unsigned char* s1, * s2, * d;//*s1--B *S2--A *d--C
                    for (s1 = src1.data, s2 = src2.data, d = img.data; d < img.dataend; s1++, s2++, d++)//d < img.dataend当这个的时候，d超出了存放图像的最后一个位置时，就停止操作
                      if (*s1 <= 128)
                           *d= (*s1) * (*s2) / 128;
                       else
                           *d= 255 - ((255 - (*s1)) * (255 - (*s2)))/128;//--两幅原始图像并放入第三个图像的对应位置中
                }*/

            }
         /*   namedWindow("原始图像", CV_WINDOW_AUTOSIZE);
            imshow("原始图像", src1);*/
            namedWindow("模板", CV_WINDOW_AUTOSIZE);
            imshow("模板", src2);
            namedWindow("漏光效果滤镜", CV_WINDOW_AUTOSIZE);
            imshow("漏光效果滤镜", img);

        }
    }
    imwrite("E:\\vsobjects\\数字图像课程设计\\result_image\\漏光效果滤镜.jpg", img);
}

int main(int argc, char** argv)
{//argv[1~7]
    // 灯塔2.jpg 、 漏光原始.jpg、picmin.jpg、漏光2.jpg、漏光3.jpg、漏光4.jpg、暴风雪中的汽船·透.jpg" 
   
   //读取图像
    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    if (!image.data) { cout << "could not open or find the image" << std::endl; return -1; }
    //读取图像，两张图像大小一样
    src1 = imread(argv[3], CV_LOAD_IMAGE_COLOR);//原始图像
    src2 = imread(argv[6], CV_LOAD_IMAGE_COLOR);//漏光模板

    //光照效果，参数输入
    int X, Y;//光源坐标
    float r, K;//光的半径、光照参数
    cout << "请依次输入光源点的位置X,Y 光照的半径r 光照的系数K 四个参数";
    cin >> X;
    cin >> Y;
    cin >> r;
    cin>>K;
    cout << "你输入的值依次为 (" << X<<","<< Y <<")  "<<r<<"   "<<K ;

    //运用交叉函数
    cross(image);
    //运用怀旧函数
    recall(image);
    //使用光照函数
    Light(image, X, Y, r, K);
    //调用漏光函数  
    Lightleak(src1, src2);

    waitKey(0);
    return 0;
}