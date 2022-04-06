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
&emsp;(3) 變數宣告 :    
&emsp;&emsp;uchar* readImage //存放讀取的圖像   
&emsp;&emsp;uchar* writeImage //存放寫回的圖像   
&emsp;&emsp;double c //可將轉換後影像調整回 0~255   
&emsp;&emsp;double G //gamma 值     
&emsp;&emsp;int s // c * pow(r, G);     
&emsp;(4) 程式邏輯 :   
&emsp;&emsp;根據圖片的gray level調整pixel的亮度，若圖片偏黑，則G<1可以調亮;若圖片偏白，則 G>1 可以調暗，再用以上的公式完成power-law函數。   
