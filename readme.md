# 基于阿克曼底盘的转向仿真程序
> 阿克曼结构由德国工程师 Lankensperger 在 1817 年提出的一种解决汽车转向问题的结构，被广泛应用于乘用车辆中。

## 设计背景
- 本次设计采用基础的舵机拉杆转向机构(Car-liked Model)作为转向机构进行正向/逆向运动学分析。


## 实验任务
> 条件：已知目标轨迹为圆轨迹。

- 1. 正向运动学分析 
    - 已知目标轨迹，输入给定时刻线速度、加速度(几何中心平均)，计算舵机转向角、后轮左右电机速度(符合阿克曼底盘约束)

- 2. 逆向运动学分析
    - 根据左右后轮速度和舵机转角估算小车线速度和加速度。

- 3. 差速底盘仿真模型
    - 已知轨迹目标
    - 输入：左右后轮速度 $V_{L}$、  $V_{R}$, 舵机偏转角 $\theta$
    - 输出：线速度、加速度，拟合出的小车轨迹

- 4. PID巡线
