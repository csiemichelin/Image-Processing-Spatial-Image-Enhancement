# Spatial Image Enhancement

## 目錄
 - [Data handed in](#Data handed in)
 - [Technical description](#Technical description)
 - [Experimental results](#Experimental results)  
## Data handed in  
**1. Power_Law_Transformation 資料夾**  
&emsp;Power_Law.cpp  
&emsp;Power_Law_Transformation.exe  
&emsp;Jetplane.bmp   
&emsp;Lake.bmp   
&emsp;Peppers.bmp   
**2. Histogram_Equalization 資料夾**  
&emsp;Histogram_Equalization.cpp  
&emsp;Histogram_Equalization.exe  
&emsp;Jetplane.bmp   
&emsp;Lake.bmp   
&emsp;Peppers.bmp   
**3. Laplacian_Sharpening 資料夾**  
&emsp;Laplacian_Sharpening.cpp  
&emsp;Laplacian_Sharpening.exe  
&emsp;Jetplane.bmp   
&emsp;Lake.bmp   
&emsp;Peppers.bmp   
## Technical description
**1. showImage**   
&emsp;(1) 目的 : 將圖像跟直方圖放在同一視窗。   
**2. Draw**     
&emsp;(1) 目的 : 畫直方圖 Histogram。   
**3. power-law (gamma) transformation**   
&emsp;(1) 目的 : 可處理 gray level 偏黑或偏白的情形，若 gamma 值小於 1 則可取代 log transformation。     
&emsp;(2) 公式 :   
&emsp;<img src="https://github.com/csiemichelin/Compiler-Implementation-Parser/blob/main/bison.jpg" width="500" height="250">    
&emsp;(3) 變數宣告 :    
&emsp;&emsp;uchar* readImage //存放讀取的圖像   
&emsp;&emsp;uchar* writeImage //存放寫回的圖像   
&emsp;&emsp;double c //可將轉換後影像調整回 0~255   
&emsp;&emsp;double G //gamma 值     
&emsp;&emsp;int s // c * pow(r, G);     
&emsp;(4) 程式邏輯 :   
&emsp;&emsp;根據圖片的gray level調整pixel的亮度，若圖片偏黑，則G<1可以調亮;若圖片偏白，則 G>1 可以調暗，再用以上的公式完成power-law。   
**4. histogram equalization**   
&emsp;(1) 目的 : 將一些像是 dark , bright , low-contrast 的 image 轉成 high-contrast image。     
&emsp;(2) 公式 :   
&emsp;(3) 變數宣告 :   
&emsp;&emsp;uchar* readImage //存放讀取的圖像  
&emsp;&emsp;uchar* writeImage //存放寫回的圖像    
&emsp;&emsp;int n = 65536; //為整張圖的pixel數256*256   
&emsp;&emsp;int maxG = 255; //gray_level的最大值   
&emsp;&emsp;int nj[256]; //對應到gray level為rk的pixel個數   
&emsp;&emsp;double Pr[256]; //出現機率   
&emsp;&emsp;double Sk[256]; //均衡化值(新的像素值)   
&emsp;&emsp;int int_Sk[256]; //將Sk由double轉int   
&emsp;(4) 程式邏輯 :      
&emsp;&emsp;Step1 : 利用 mset 將 nj 矩陣初始為 0    
&emsp;&emsp;Step2 : 計算 gray level 為 rk 的 pixel 個數並存在 nk 中    
&emsp;&emsp;Step3 : 計算出現機率(Pr) = 出現次數(nj) / 像素總數 (n)    
&emsp;&emsp;Step4 : 要將色彩空間範圍擴展為[0, 255] 共 256 個像素級數，就必須將原累計機率乘以 255，得到均衡化值
&emsp;&emsp;&emsp;&emsp;，即 Sk[i] = Pr[i] * maxG ;     
&emsp;&emsp;Step5 : 再將所有的 Sk[i]做 sumation    
&emsp;&emsp;Step6 : 將 Sk 從 double 直接轉成 int，可以捨去浮點數的部分   
&emsp;&emsp;Step7 : 最後將均衡化值寫回新的像素中   
