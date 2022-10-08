# 卡尔曼滤波

[kalman_intro.pdf (unc.edu)](https://www.cs.unc.edu/~welch/media/pdf/kalman_intro.pdf)

[Microsoft Word - Kalman15.doc (yale.edu)](http://cs-www.cs.yale.edu/homes/yry/readings/general/Kalman1960.pdf)

> The Kalman filter is a set of mathematical equations that provides an efficient computational (recursive) means to estimate the state of a process, in a way that minimizes the mean of the squared error.

## The Discrete Kalman Filter

### The Process to be Estimated

$$
x_k =Ax_k-1 +Bu_k-1+ w_k-1
$$

$$
z_k =Hx_k+ v_k
$$

The random variables  and  represent the process and measurement noise (respectively). They are assumed to be independent (of each other), white, and with normal probability distributions

$$
 p (w )∼ N (0, Q )
 

$$

$$
p (v )∼ N (0, R )
$$

### The Computational Origins of the Filter

define  a priori and a posteriori estimate errors as

$$
e_k^- ≡x_k- ˆx_k^- 
$$

$$
e_k ≡x_k- ˆx_k 
$$

$$
P_k^-= E [(e_k^-)(e_k^-)^T ]
$$

$$
P_k= E [e_k e_k^T ]
$$

$$

ˆx_k=ˆx_k^- +K( z_k-Hˆx_k^-)
$$

残差

$$
( z_k-Hˆx_k^-)
$$

The  matrix K is chosen to be the gain or blending factor that minimizes the a 
posteriori error covariance P_k

对迹求导：

$$
K_k= P_k^- H ^T( HP_k^- H ^T +R) ^- 1

$$

$$
lim(R_k →0)K_k= H ^–1
$$

$$
lim(P_k^- →0)K_k= 0
$$

### The Probabilistic Origins of the Filter

Bayes’ rule 

![](https://img-blog.csdn.net/20180225203500452?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpeGluXzQwNzU5MTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**后验概率 = 标准相似度*先验概率**

卡尔曼滤波满足：

$$
 E [x_k ]=  ˆx_k
$$

$$
E [(x_k- ˆx_ k)(  x_k- ˆx_ k )^T ] =P_k.
$$

$$
p (x_k| z_k ) ∼N( E [x_k ], E[( x_k- ˆx_ k )(x_k -ˆx _k )^T ])

$$

$$
=N(ˆx_k,P_k)
$$

### The Discrete Kalman Filter Algorithm

the equations for the Kalman filter fall into two groups: time update equations and measurement update equations.

时间更新方程 预测

测量更新方程 校正

![preview](https://pic2.zhimg.com/v2-32b046b882950eac20eda2c502f4e005_r.jpg)

### Filter Parameters and Tuning

![](D:\desktop\QQ图片20220927181631.png)



## The Extended Kalman Filter (EKF)

线性化

### The Process to be Estimated

$$
x_k= f( x_k-1 ,u_k-1, w_k -1 )
$$

$$
z_k= h( x_k, v_k )
$$

It includes as parameters any driving function u_k-1, and the zero-mean process noise w_k. The non-linear function h in the measurement equation  relates the state x_k to the measurement z_k .

![](D:\desktop\QQ图片20220927183108.png)

It is important to note that a fundamental flaw of the EKF is that the distributions (or densities in the continuous case) of the various random variables are no longer normal after undergoing their respective nonlinear transformations. The EKF is simply an ad hoc state estimator that only approximates the optimality of Bayes’ rule by linearization. 

### The Computational Origins of the Filter

![](D:\desktop\QQ图片20220927183005.png)

测量误差：

$$
e˜ (x_k) ≡x_k -x_k˜

$$

测量残差：

$$
e˜ (z_k) ≡z_k -z_k˜
$$

![](D:\desktop\QQ图片20220927183944.png)

where ε and η  represent new independent random variables having zero mean and covariance matrices 

$$
WQW^T,VRV^T
$$

$$
ˆ x_k= ˜x_ k+ ˆe_k
$$

![](D:\desktop\QQ图片20220927184851.png)

$$
 ˆe_k= K_k˜e_zk
$$

![](D:\desktop\QQ图片20220927185051.png)

![](D:\desktop\QQ图片20220927185138.png)

![](D:\desktop\QQ图片20220927185259.png)

 ![](D:\desktop\QQ图片20220927193148.png)

# RM

在三维空间中进行状态估计，需要位置和速度，x_k有六维。

使用视觉传感器能得到图像的像素点位置，理论上应将像素点作为测量值，实际中对三维点坐标作非线性变换最后用yaw,pitch,distance作为测量量：枪管正对的角度不一定是陀螺仪的0度，计算得到的x,y值方差会跟随枪管正对的陀螺仪角度变化的

P:状态估计协方差,一般设定为单位阵让它自己收敛即可。

R:为测量噪声矩阵,一般只需要设置其对角元素,通常不会将R矩阵的对角线元素设为方差 σ^2,而是设置为 (3σ)^2。

一种做法：敌方装甲板不动，动云台，保存相关的原始坐标值，然后利用Matlab或者Python计算出方差填入该矩阵中。

由于相机的小孔成像模型，如果误差一个像素的话，敌方装甲板在不同距离所计算得到的distance方差是完全不同的。因此，一种显然的想法是根据distance的计算值动态改变distance的方差。目前的做法是测出不同距离的distance方差，然后用一条直线拟合出来，每次都不断计算更新distance的方差。yaw和pitch的方差与距离无关，设置固定值。

Q:如果仅设置Q矩阵的对角阵，默认了一个轴的速度与其位置值无关，这个命题显然是不成立的，速度值和位置值的过程噪声必然存在一些关联。得到同一个轴之间的关联之后，需要调节的参数就只剩下三个了，分别是x,y,z轴的过程噪声。x,y轴应该是等效的(因为存在陀螺仪绕z轴的旋转)，那么x,y轴的过程噪声也应当设置为一致的，即此时需要调节的仅有两个参数。

调参流程：滤波部分和预测部分。



## 运动模型：

### CV：匀速模型（Constant Velocity）

![](https://img-blog.csdnimg.cn/20200316145312750.png)

状态转移方程：

![](https://img-blog.csdnimg.cn/20200316145341316.png)

另一版本：![](https://img-blog.csdnimg.cn/20200316132023794.png)

![](https://img-blog.csdnimg.cn/20200316132048299.png)



![](C:\Users\lenovo\AppData\Roaming\marktext\images\2022-09-27-20-18-57-]O3D%B[9%V6ZQ0236J6@6SV.png)

![](D:\desktop\QQ图片20220927202012.png)

噪声(根据s=1/2​aΔt^2):

![](D:\desktop\QQ图片20220927202125.png)

![](D:\desktop\QQ图片20220927202329.png)

### CA:匀加速模型（Constant Acceleration)

![](https://img-blog.csdnimg.cn/20200316132108877.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200316132135781.png)

### CTRV: 恒定转弯率和速度幅度模型（Constant Turn Rate and Velocity)

![](https://img-blog.csdnimg.cn/20200316133255953.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200316133602730.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/2020031613422215.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2RqZmprajUy,size_16,color_FFFFFF,t_70)

积分：

![](https://img-blog.csdnimg.cn/20200316134342209.png)

噪声：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200316134900257.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200316134915330.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200316135248926.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2RqZmprajUy,size_16,color_FFFFFF,t_70)

### CTRA:恒定转动率和加速度（Constant Turn Rate and Acceleration)

![](https://img-blog.csdnimg.cn/20200316173500912.png)

![](https://img-blog.csdnimg.cn/20200316173510367.png)

![](https://img-blog.csdnimg.cn/20200316173625109.png)

### 恒定转向角和速度（CSAV）

### CCA:恒定曲率和加速度(Constant Curvature and Acceleration）


