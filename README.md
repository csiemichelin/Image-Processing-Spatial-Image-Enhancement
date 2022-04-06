**Spatial Image Enhancement** 

余昆霖  610410114 Data due : 2021/11/8 

1. Data handed in 

一、Power\_Law\_Transformation 資料夾 

- Power\_Law.cpp 
- Power\_Law\_Transformation.exe 
- Jetplane.bmp 
- Lake.bmp 
- Peppers.bmp 

二、Histogram\_Equalization 資料夾 

- Histogram\_Equalization.cpp 
- Histogram\_Equalization.exe 
- Jetplane.bmp 
- Lake.bmp 
- Peppers.bmp 

三、Laplacian\_Sharpening 資料夾 

- Laplacian.cpp 
- Laplacian\_Sharpening.exe 
- Jetplane.bmp 
- Lake.bmp 
- Peppers.bmp 

四、Homework 報告 

1) 610410114.docx 
1) 610410114.pdf 
2. Technical description 

一、showImage 

(1)  目的  : 將圖像跟直方圖放在同一視窗。 

二、Draw 

(1)  目的  :  畫直方圖 Histogram。 

三、power-law (gamma) transformation

1) 目的  :  可處理 gray level  偏黑或偏白的情形，若 gamma 值小於 1 則可取代 log transformation。 
1) 公式  :   

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.001.png)

3) 變數宣告 :  

uchar\* readImage     //存放讀取的圖像 

uchar\* writeImage   //存放寫回的圖像 

double c   //可將轉換後影像調整回 0~255 double G   //gamma 值 

int s   //c \* pow(r, G);

4) 程式邏輯  :  根據圖片的 gray level 調整 pixel 的亮度，若圖片偏 黑，則 G<1 可以調亮;  若圖片偏白，則 G>1 可以調暗。，再用以 上的公式完成 power-law 函數。 

四、histogram equalization 

1) 目的  :  將一些像是 dark , bright , low-contrast  的 image 轉成 high- contrast image。 
1) 公式  : 

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.002.png)

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.003.png)

3) 變數宣告 : 

uchar\* readImage     //存放讀取的圖像 

uchar\* writeImage   //存放寫回的圖像 

int n = 65536;   //為整張圖的pixel數256\*256 

int maxG = 255;  //gray\_level的最大值 

int nj[256];    //對應到gray level為rk的pixel個數 

double Pr[256];  //出現機率 

double Sk[256];    //均衡化值(新的像素值) 

int int\_Sk[256];  //將Sk由double轉int 

4) 程式邏輯  :  

Step1 :  利用 mset 將 nj 矩陣初始為 0 

Step2 : 計算 gray level 為 rk 的 pixel 個數並存在 nk 中 

Step3 : 計算出現機率(Pr) = 出現次數(nj) / 像素總數 (n) 

Step4 :  要將色彩空間範圍擴展為[0, 255]  共  256  個像素級數，就 必須將原累計機率乘以 255，得到均衡化值，即 Sk[i] = Pr[i] \* maxG ;  

Step5 :  再將所有的 Sk[i]做 sumation 

Step6 : 將 Sk 從 double 直接轉成 int，可以捨去浮點數的部分

Step7 : 最後將均衡化值寫回新的像素中

五、image sharpening using the Laplacian operator 

1) 目的  :  與 smooth spatial filters 相反，主要用在 edge detection 上， 也就是把影像中，物體的邊界找出來，利用二階微分實現 Laplacian operator 
1) 公式  : 

對影像微分，指的就是影像的像素值相減，再進行二次微分得到: 

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.004.png)

最後代入 Laplacian operator 得到: 

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.005.png)

而在 image enhancement 上使用的 basic Laplacian operator 為: 

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.006.png)

而這裡通常是用在銳化圖像上，故我們選取 center 為負的去做， 其四鄰域化簡後的一般式為: 

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.007.png)

3) 變數宣告 : 

src.at<uchar>   //存取 src 一個 pixel 的 gray level dst.at<uchar>   //存取 dst 一個 pixel 的 gray level mask  //Laplacian mask  鄰域，差值越大凸顯臨邊

4) 程式邏輯  :  

我所使用的 Laplacian mask 有以下兩種: 

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.008.png)

故選取 center 為負的 image enhancement basic Laplacian operator 去 做，使用雙層迴圈完成上面的公式，實現影像銳化的效果，且用 saturate\_cast 函數防止資料溢位。 

3. Experimental results 

一、前置作業  : 

1) 作業系統  : Windows 10  家用版
1) Visual studio 版本  : Visual Studio 2019
1) OpenCV 版本  : OpenCV 4.5.3 
1) 環境架設  : [https://forum.gamer.com.tw/C.php?bsn=60292&snA=6494 ](https://forum.gamer.com.tw/C.php?bsn=60292&snA=6494)

二、power-law (gamma) transformation(左:原圖，右:銳化或等化後的圖)

1) Jetplane.bmp   : G=1.6 

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.009.jpeg)

2) Lake.bmp  : G=0.45 

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.010.jpeg)

3) Peppers.bmp   : G=4.1 

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.011.jpeg)

三、histogram equalization(左:原圖，右:銳化或等化後的圖) 

1) Jetplane.bmp  

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.012.jpeg)

2) Lake.bmp 

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.013.jpeg)

3) Peppers.bmp

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.014.jpeg)

四、image sharpening using the Laplacian operator(左:原圖，右:銳化或等化

後的圖) 

1) Jetplane.bmp   : mask = { -1, -1, -1, -1, 8, -1, -1, -1, -1 } 

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.015.jpeg)

2) Lake.bmp  : mask = { -1, -1, -1, -1, 8, -1, -1, -1, -1 } 

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.016.jpeg)

3) Peppers.bmp   : mask = { 0, -1, 0, -1, 4, -1, 0, -1, 0 } 

![](Aspose.Words.dcffb0a4-438a-4641-857f-bcdefe53c2cf.017.jpeg)

4. Discussions 

一、畫直方圖若大小超過顯示視窗怎麼辦?

會傳入一個 int num 參數給 Draw 函數，使其可以動態調整 histogram 的縮放。

二、Power law 的 gamma 值如何去設置?

可以觀察原圖的亮度，就可以預設 gamma 值要大於 1 或小於 1，再利 用上下逼近法的概念慢慢去 TRACE。 

三、Power law 的 c 值如何去設置?

c  是常數，可將轉換後影像調整回 0~255，其方法沒有一定，我這裡 是對其做以下式子處理 c = 255 / (pow(255, G))。

四、在做 Laplacian operator 時，如何解決溢位情況?

利用 saturate\_cast 函數，其作用是:當運算完後，結果為負，則轉為

0，結果超出 255，則為 255。 

五、Laplacian 利用 src.at<uchar>去做，而不是用 for  迴圈實現 src.total() ?

因為 Laplacian 會考慮 pixel 上下左右的關係。 

5. References and Appendix 

一、OpenCV 前置作業  : 

[https://forum.gamer.com.tw/C.php?bsn=60292&snA=6494 ](https://forum.gamer.com.tw/C.php?bsn=60292&snA=6494)

二、histogram equalization : 

https://codeinfo.space/imageprocessing/histogram-equalization/ 

三、Laplacian operator : 

https://silverwind1982.pixnet.net/blog/post/250092094 

四、OpenCV 畫直方圖  : 

https://blog.csdn.net/i\_chaoren/article/details/74277408 

五、OpenCV 把兩張圖在同一視窗顯示的方法  : 

http://jashliao.eu/wordpress/2019/09/20/opencv- %E6%8A%8A%E5%85%A9%E5%BC%B5%E5%9C%96%E5%9C%A8% E5%90%8C%E4%B8%80%E8%A6%96%E7%AA%97%E9%A1%AF%E 7%A4%BA%E7%9A%84%E6%96%B9%E6%B3%95%E5%9C%96%E7 %89%87%E8%A4%87%E8%A3%BD%EF%BC%8C%E5%9C%96%E7%

89%87/ 
