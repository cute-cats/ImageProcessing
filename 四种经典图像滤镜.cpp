#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace cv;
using namespace std;
Mat src1,src2, img;//©��Ч��ͼ������---ԭͼ��ģ�塢�ں�ͼ��

//�����˾�����
void cross(Mat src)
{
    int width = src.rows, height = src.cols;//ԭͼ��������

    Mat img(src.size(), CV_8UC3);//����һ��CV_8UC3���ͣ���С��ԭͼһ����ͼ�� ��  CV_8UC3��3ͨ��*8�ֽ�

    int x, y;
    uchar* P0, * P1;
    for (y = 0; y < height; y++) //ͼƬ�ĸ߶�
    {
        P0 = src.ptr<uchar>(y);//ԭͼ�������е��׵�ַ��y��0����ַ
        P1 = img.ptr<uchar>(y);//Ŀ��ͼ�������е��׵�ַ��y��0����ַ
        for (x = 0; x < width; x++)//��
        {
            //ԭͼ���ط���
            float R = P0[3 * x + 2];//R
            float G = P0[3 * x + 1];//G
            float B = P0[3 * x];//B

          //���ù�ʽ��ʹ���м����洢������
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

            //�м�����ֵ��Ŀ��ͼ��
            P1[3 * x] = (uchar)MIN(255, MAX(0, newB));
            P1[3 * x + 1] = (uchar)MIN(255, MAX(0, newG));
            P1[3 * x + 2] = (uchar)MIN(255, MAX(0, newR));
        }

        namedWindow("ԭʼͼ��", CV_WINDOW_AUTOSIZE);
        imshow("ԭʼͼ��", src);
        namedWindow("�������˾�", CV_WINDOW_AUTOSIZE);
        imshow("�������˾�", img);
    }

    imwrite("E:\\vsobjects\\����ͼ��γ����\\result_image\\�����˾�.jpg", img);
}

//�����˾�����
void recall(Mat src) {
    int width = src.rows, height = src.cols;//ԭͼ����
    Mat img(src.size(), CV_8UC3);//������srcԭͼһ����3ͨ��8�ֽ�Matͼ��

    int x, y;
    uchar* P0, * P1;
    for (y = 0; y < height; y++) //ͼƬ�ĸ߶�-����y
    {
        P0 = src.ptr<uchar>(y);//ԭͼ�������е��׵�ַ��y��0����ַ
        P1 = img.ptr<uchar>(y);//Ŀ��ͼ�������е��׵�ַ��y��0����ַ

        for (x = 0; x < width; x++) //��
        {
            float R = P0[3 * x + 2];//R
            float G = P0[3 * x + 1];//G
            float B = P0[3 * x];//B
            //���ù�ʽ������������һ��R G B�����洢�����������255��
            float newR = MIN(0.393 * R + 0.769 * G + 0.189 * B, 255);
            float newG = MIN(0.349 * R + 0.686 * G + 0.168 * B, 255);
            float newB = MIN(0.272 * R + 0.534 * G + 0.131 * B, 255);
            //���
            P1[3 * x] = (uchar)newB;
            P1[3 * x + 1] = (uchar)newG;
            P1[3 * x + 2] = (uchar)newR;
        }
       /* namedWindow("ԭʼͼ��", CV_WINDOW_AUTOSIZE);
        imshow("ԭʼͼ��", src);*/
        namedWindow("�����˾�", CV_WINDOW_AUTOSIZE);
        imshow("�����˾�", img);

    }
    imwrite("E:\\vsobjects\\����ͼ��γ����\\result_image\\�����˾�.jpg", img);
}

//����Ч������
void Light(Mat src, int X, int Y, float r, float K)//ԭͼ�����յ㡢�뾶������ϵ��
{
    int width = src.rows, height = src.cols;//ԭͼ���� 

    Mat img(src.size(), CV_8UC3);//����һ��ͬ����С��Mat���͵�imgͼ��

    int x, y;//������ʾͼ����к���
    unsigned char* P0, * P1;

    for (y = 0; y < height; y++) //ͼƬ�ĸ߶�--����y
    {
        P0 = src.ptr<uchar>(y);//ԭͼ���أ�y��0����ַ
        P1 = img.ptr<uchar>(y);//Ŀ��ͼ��y��0����ַ
        for (x = 0; x < width; x++) //��
        {
            //�м����������洢��ʽ�����Ŀ��ͼ�����ؽ��
            float newR, newB, newG;

            //ԭʼͼ���3�����ط���
            float R = P0[3 * x + 2];//R
            float G = P0[3 * x + 1];//G
            float B = P0[3 * x];//B

            float distance = sqrt((X - x) * (X - x) + (Y - y) * (Y - y)); //��Դ�����ص����
            float result = K * MAX(0, 1 - distance / r);//����ֵ
            if (distance < r) //��Դ�㵽���ص�ľ���<��Դ�İ뾶r ʱ
            {
                //��ʽ
                newB = B + result;
                newG = G + result;
                newR = R + result;
                //����������ֵ��Ŀ��ͼ������ص�ַ 
                P1[3 * x] = (uchar)MIN(255, MAX(0, newB));
                P1[3 * x + 1] = (uchar)MIN(255, MAX(0, newG));
                P1[3 * x + 2] = (uchar)MIN(255, MAX(0, newR));
            }
            else
            {
                //��Դ�޷��������ص��ʱ��
                //����ֵΪԭʼֵ
                newB = B;
                newG = G;
                newR = R;
                P1[3 * x] = (uchar)MIN(255, MAX(0, newB));
                P1[3 * x + 1] = (uchar)MIN(255, MAX(0, newG));
                P1[3 * x + 2] = (uchar)MIN(255, MAX(0, newR));
            }
        }
       /* namedWindow("ԭʼͼ��", CV_WINDOW_AUTOSIZE);
        imshow("ԭʼͼ��", src);*/
        namedWindow("����Ч���˾�", CV_WINDOW_AUTOSIZE);
        imshow("����Ч���˾�", img);
    }
    //д���ļ��У��洢���ͼƬ
    imwrite("E:\\vsobjects\\����ͼ��γ����\\result_image\\����Ч���˾�.jpg", img);
}



//©�⺯��ͼ��
void Lightleak(Mat src1, Mat src2) {
    Mat img;//�ںϽ��ͼ��
    int width = src1.rows, height = src1.cols;//ԭͼ����������
    uchar* s1;//ԭͼָ��
    uchar* s2;//ģ��ָ��
    uchar* d;//�ںϽ��ͼָ��
    img.create(src1.size(), CV_8UC3);//����һ��ͬ����С��Mat���͵�imgͼ��

    int x, y;//ͼ����к���
    if (src1.isContinuous() && src2.isContinuous())
    {
        for (y = 0; y < height; y++) {//ͼƬ�ĸ߶�

            s1 = src1.ptr<uchar>(y);//ԭͼ�������е��׵�ַ��y��0����ַ
            s2 = src2.ptr<uchar>(y);//Ŀ��ͼ�������е��׵�ַ��y��0����ַ
            d = img.ptr<uchar>(y);//Ŀ��ͼ�������е��׵�ַ��y��0����ַ
            for (x = 0; x < width; x++)//��
            {
                //imgΪĿ��ͼ����������ԭʼͼ��һ����
                img.create(src1.rows, src1.cols, CV_8UC3);
                //ԭʼͼ�����
                float R1 = s1[3 * x + 2];//R
                float G1 = s1[3 * x + 1];//G
                float B1 = s1[3 * x];//B
                //ģ�����
                float R2 = s2[3 * x + 2];//R
                float G2 = s2[3 * x + 1];//G
                float B2 = s2[3 * x];//B
                float newR, newG, newB;//�ںϵ�ͼ�����ط���
                //���ù�ʽ
                if (R1 <= 128) newR = R1 * R2 / 128;  else newR = 255 - ((255 - R1) * (255 - R2)) / 128;
                if (G1 <= 128) newG = G1 * G2 / 128;  else newG = 255 - ((255 - G1) * (255 - G2)) / 128;
                if (B1 <= 128) newB = B1 * B2 / 128;  else newB = 255 - ((255 - B1) * (255 - B2)) / 128;
                //���յ�ͼ�����ظ�ֵ
                d[3 * x] = (uchar)MIN(255, MAX(0, newB));
                d[3 * x + 1] = (uchar)MIN(255, MAX(0, newG));
                d[3 * x + 2] = (uchar)MIN(255, MAX(0, newR));
                /*if (src1.isContinuous() && src2.isContinuous() && img.isContinuous())
                {
                    unsigned char* s1, * s2, * d;//*s1--B *S2--A *d--C
                    for (s1 = src1.data, s2 = src2.data, d = img.data; d < img.dataend; s1++, s2++, d++)//d < img.dataend�������ʱ��d�����˴��ͼ������һ��λ��ʱ����ֹͣ����
                      if (*s1 <= 128)
                           *d= (*s1) * (*s2) / 128;
                       else
                           *d= 255 - ((255 - (*s1)) * (255 - (*s2)))/128;//--����ԭʼͼ�񲢷��������ͼ��Ķ�Ӧλ����
                }*/

            }
         /*   namedWindow("ԭʼͼ��", CV_WINDOW_AUTOSIZE);
            imshow("ԭʼͼ��", src1);*/
            namedWindow("ģ��", CV_WINDOW_AUTOSIZE);
            imshow("ģ��", src2);
            namedWindow("©��Ч���˾�", CV_WINDOW_AUTOSIZE);
            imshow("©��Ч���˾�", img);

        }
    }
    imwrite("E:\\vsobjects\\����ͼ��γ����\\result_image\\©��Ч���˾�.jpg", img);
}

int main(int argc, char** argv)
{//argv[1~7]
    // ����2.jpg �� ©��ԭʼ.jpg��picmin.jpg��©��2.jpg��©��3.jpg��©��4.jpg������ѩ�е�������͸.jpg" 
   
   //��ȡͼ��
    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    if (!image.data) { cout << "could not open or find the image" << std::endl; return -1; }
    //��ȡͼ������ͼ���Сһ��
    src1 = imread(argv[3], CV_LOAD_IMAGE_COLOR);//ԭʼͼ��
    src2 = imread(argv[6], CV_LOAD_IMAGE_COLOR);//©��ģ��

    //����Ч������������
    int X, Y;//��Դ����
    float r, K;//��İ뾶�����ղ���
    cout << "�����������Դ���λ��X,Y ���յİ뾶r ���յ�ϵ��K �ĸ�����";
    cin >> X;
    cin >> Y;
    cin >> r;
    cin>>K;
    cout << "�������ֵ����Ϊ (" << X<<","<< Y <<")  "<<r<<"   "<<K ;

    //���ý��溯��
    cross(image);
    //���û��ɺ���
    recall(image);
    //ʹ�ù��պ���
    Light(image, X, Y, r, K);
    //����©�⺯��  
    Lightleak(src1, src2);

    waitKey(0);
    return 0;
}