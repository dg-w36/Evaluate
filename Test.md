@(dg_w的笔记本)
#仪器光电杯竞赛准备
## 第一阶段工作总结
我们基本完成了第一阶段的工作，对算法的设计有了初步的计划，并对安卓方面的开发有了一定的认识，接下来进入第二阶段的程序编写任务，在编写的过程中，关于算法设计的成员还要进一步完善算法。

### 关于opencv的学习
之前老师推荐给我一本入门的书，可以用来查阅使用，写得比较简单 《opencv3编程入门》  
这本书是作者博客的总结版本，[网址](http://blog.csdn.net/poem_qianmo)在此。
**这几天我会找出咱们主要使用的几个函数，然后告诉大家，咱们看看手册**。
### 算法设计方面
根据张来同学的总结，我们目前总结出来一下几个因素，并给出大致算法：
1.  拍摄人时的处理一：**我们应该使人眼的位置处于对焦点位置，从而使人身的位置也相对正确**
	- 实现方法分析：通过 **眼部识别**  算法，找出人眼的相关位置，并将眼部位置与 **对焦点** 位置进行比对，根据距离情况进行打分。
	- 算法使用：openCV中的 **objectDetection**模块中的眼部识别。
	- 对焦点的位置获取：图像中，横向与纵向 **三分线的交点** ，共四个为：$(x_0,y_0),(x_1,y_1)(x_2,y_2),(x_3,y_3)$
	- 设置距离区间：设置最大距离值 $D$, 应使距离处于$ 0 \sim D$ 间
	- 分数计算：我们判断得眼睛位置为 $(x_e,y_e)$，评分为：$$ 100 * (1- \frac{min\{d_0,d_1,d_2,d_3\}}{D}) $$ 其中：$d_i = \sqrt {(x_e - x_i)^2 + (y_e - y_i)^2}$
2. 拍摄人时的处理二：**使人体在图片中的大小合适**
	- 实现方法分析：**人体与头部的相对比例** 是基本确定的，所以我们通过脸部识别，识别出图中**脸的大致大小** ，求出脸的大小与整个图片大小的比值，推算身体占图片的比值。
	- 算法使用：openCV中的 **objectDetection**模块中的脸部识别。
	- 需要获取的值：图片的高与宽：$H,W$，脸部区域的高与宽：$H_f,W_f$, 脸部大小与人体大小的比例$P_f$
	- 分数计算：$$100 * (1 - \frac{H_f W_f P_f}{H W})$$
3.  拍摄景物时的处理：**使独立物体处于合适的位置**
	- 实现方法分析：这种方法用于 **大小相对较小** 的物体，我们通过其与背景的 **色差**，获取景物的 **当前位置**，并将其位置与 **对焦点** 进行比对。
	- 算法使用：使用 *Grahp cut* 相关算法，openCV中的**Grabcut**算法。
	- 独立物体位置的获取：以 **Grabcut** 算法为例，该算法可以获取所选物体的**轮廓**，我们先将轮廓换为**方形边框**，只需确定 **左上角** $A(x_a,y_a)$ 和 **右下角** $B(x_b,y_b)$ 的值，有:$$x_a = min\{x_1,x_2,x_3,\cdots,x_n\},y_a = min\{y_1,y_2,y_3,\cdots,y_n\}$$ $$x_b = max\{x_1,x_2,x_3,\cdots,x_n\},y_b = max\{y_1,y_2,y_3,\cdots,y_n\}$$*$(x_i,y_i)$ 是轮廓上点的坐标*。则用轮廓的中点坐标来代表大致位置 $p(x_p,y_p) = (\frac{x_a + x_b}{2} , \frac{y_a+y_b}{2})$ 。
	- 与焦点为值得比较参照 **第一条** 。
4. 拍摄景物时的处理：**使对称物体处于相对中央的位置**
	- 实际方法分析：筛选出 **对称物体的边界点** ，然后根据边界点的 **对称关系** 求出对称物体 **对称轴** 的大致位置，并与我们预先设定的 **取景框对称轴区间** 进行比对
	 ![Alt text](./样例图片.png)
	 以上方的图片为例，**灰线** 为对称轴位置，其两边的 **红线** 位置为可接受区间位置。黑色框表示对称图形，我们在 **蓝色线** 位置对色彩信息进行扫描，获取了六个边界点，计算 **两两边界点中点的y坐标 **，取 **距离中轴线最近的y坐标** 作为图片对称轴位置。
- 需获取的值：取景框中轴线y坐标 $Y_m$，可接受距离区间 $D$ ，对称图形的中轴线y坐标 $y_p$
- 分数计算：$$100 * (1 - \frac{(y_p-Y_m)^2}{D^2} )$$
5. 拍摄景物时的处理：**保证物体在图片中的正直**
	- 实际方法分析：物体歪斜的主要原因是拍照时没有将手机握稳，因此，我们可以根据手机自带的陀螺仪和加速度计获得当前手机的姿态信息，拍照时，姿态越端正，评分越高。
	- 姿态相关知识：
		- 手机姿态描述的坐标系建立：		
> The coordinate-system is defined relative to the screen of the phone in its default orientation. The axes are not swapped when the device's screen orientation changes.
> The X axis is horizontal and points to the right, the Y axis is vertical and points up and the Z axis points towards the outside of the front face of the screen. In this system, coordinates behind the screen have negative Z values.
>  ![Alt text](./axis_device.png)

		- 手机姿态的描述
	> All values are in SI units (m/s^2)
	> - values[0]: Acceleration minus Gx on the x-axis
	> - values[1]: Acceleration minus Gy on the y-axis
	>- values[2]: Acceleration minus Gz on the z-axis
	- 姿态确定算法：根据加速度计各个方向的数值确定姿态。
	- 相关网址：[Android Sensor](http://developer.android.com/guide/topics/sensors/sensors_overview.html)

### 工作安排
1. 算法设计部分：
张来，继续进行评估算法的设计。
2. 算法编写部分：
王浩强负责上述前四条。
冯凯文、于济凡负责上述第5条内容
3. 手机软件编写：
冯凯文、于济凡负责编写无评估算法下的软件
**评估算法目前并没有完善，需要进一步细化。初步预计第二阶段在一个半月内完成，每周至少讨论一次，大家互相监督进度**
