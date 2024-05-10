clc; clear;
load('pre-predict-data.mat')
% 清除命令窗口和工作空间变量
% 生成合理的图形
l = 1.0; % 轴距长度
w = 0.5; % 轮距
slow = 1; % m/s (3.6 kph, 2.240 mph)
med = 7; % m/s (27 kph, 16.777 mph)
fast = 15; % m/s (54 kph, 33.554 mph)
dscale = 5; % se2_spline 使用的参数，定义生成的参考样条曲线的“曲折度”

M = 1;
M1 = M/2;
M2 = M/2;

nx = 5; % 状态变量的维度
ny = 3; % 输出变量的维度
nu = 2; % 控制变量的维度
Ts = 0.05; % 采样时间
mpc_horizon = 10;

% 创建非线性模型预测控制器对象
nlobj = nlmpc(nx, ny, nu);
nlobj.Ts = Ts;
nlobj.PredictionHorizon = mpc_horizon;
nlobj.ControlHorizon = mpc_horizon;

nlobj.Model.IsContinuousTime = true;
nlobj.Model.NumberOfParameters = 3;
nlobj.Model.StateFcn = @ackerman_fk_dynamics;
nlobj.Model.OutputFcn = @(state, control, l, M1, M2) state(1:3);

% 定义状态变量
nlobj.States(1).Name = 'phi';
nlobj.States(1).Units = 'Radians';
nlobj.States(1).Min = -deg2rad(75);
nlobj.States(1).Max = deg2rad(75);

nlobj.States(2).Name = 'v_l';
nlobj.States(2).Units = 'Meters / Second';
nlobj.States(2).Min = 0.0;
nlobj.States(2).Max = 20.0;

nlobj.States(3).Name = 'v_r';
nlobj.States(3).Units = 'Meters / Second';
nlobj.States(3).Min = 0.0;
nlobj.States(3).Max = 20.0;

nlobj.States(4).Name = 'v_x';
nlobj.States(4).Units = 'Meters / Second';
nlobj.States(4).Min = 0.0;
nlobj.States(4).Max = 20.0;

nlobj.States(5).Name = 'v_y';
nlobj.States(5).Units = 'Meters / Second';
nlobj.States(5).Min = 0.0;
nlobj.States(5).Max = 20.0;

% 定义控制变量
nlobj.ManipulatedVariables(1).Name = 'F';
nlobj.ManipulatedVariables(1).Units = 'Newtons';
nlobj.ManipulatedVariables(1).Min = -80;
nlobj.ManipulatedVariables(1).Max = 20;

nlobj.ManipulatedVariables(2).Name = 'u';
nlobj.ManipulatedVariables(2).Units = 'Rad / Second';


% 创建非线性模型预测控制器的移动选项对象
nlopt = nlmpcmoveopt;
nlopt.Parameters = {l, M1, M2};
nlopt.OutputWeights = [10, 10, 10];
nlopt.MVWeights = [0.1, 0.01];
nlopt.MVRateWeights = [1, 1];

%% 实验 1 设置
ex1.l = l;
ex1.w = w;
ex1.obstacle.bl = [3, 0]; % 障碍物左下角坐标
ex1.obstacle.tr = [6, 3]; % 障碍物右上角坐标
ex1.y0 = [0; 0; pi/1.2]; % 初始位置
ex1.y1 = [2; 5; pi/2]; % 目标位置
ex1.slow.x0 = [ex1.y0; 0; slow]; % 慢速给定左右轮曲线
ex1.med.x0 = [ex1.y0; 0; med]; % 中速给定左右轮曲线
ex1.fast.x0 = [ex1.y0; 0; fast]; % 快速给定左右轮曲线

% 打印实验 1 开始和起点到终点的距离
fprintf(sprintf('Ex 1 Start, Goal dist: %g\n', se2_dist(ex1.y0, ex1.y1)))

% 定义颜色
ex1.ref_color = 'k';
ex1.slow.color = 'b';
ex1.med.color = 'm';
ex1.fast.color = 'r';

% 计算持续时间、时间步长，并生成参考轨迹
duration = se2_dist(ex1.y0, ex1.y1) / 5.0;
Tsteps = ceil(duration / Ts);
ex1.y_ref = se2_spline(ex1.y0, ex1.y1, Tsteps, dscale);
ex1.lifted_y_ref = [ex1.y_ref; zeros(2, Tsteps + 1)];
tc = 0:Ts:24*Ts;

%% 绘制环境
figure(1); clf
plot_env(ex1);

%% 实验 1 - 慢速
[ex1.slow.x_history] = ...
    ackerman_process(nlobj, ex1.slow.uk, ex1.slow, ex1.y1, l, w, M1, M2, dscale);

 
%% 绘制慢速轨迹
subplot(1, 3, 1);
% plot_car_traj(ex1.slow.x_history, l, w, ex1.slow.color);
title(sprintf('Slow start v = %g', ex1.slow.x0(end)))
drawnow

% %% 实验 1 - 中速
[ex1.med.x_history] = ...
    ackerman_process(nlobj, ex1.med.uk, ex1.med, ex1.y1, l, w, M1, M2, dscale);


%% 绘制中速轨迹
subplot(1, 3, 2);
% plot_car_traj(ex1.slow.x_history, l, w, ex1.slow.color);
title(sprintf('Slow start v = %g', ex1.med.x0(end)))
drawnow

% %% 实验 1 - 快速
[ex1.fast.x_history] = ...
    ackerman_process(nlobj, ex1.fast.uk, ex1.fast, ex1.y1, l, w, M1, M2, dscale);

%% 绘制中速轨迹
subplot(1, 3, 3);
% plot_car_traj(ex1.slow.x_history, l, w, ex1.slow.color);
title(sprintf('Slow start v = %g', ex1.fast.x0(end)))
drawnow

ackerman_fk_plotting(tc, ex1.slow.uk, ex1.slow.x_history, 2);
ackerman_fk_plotting(tc, ex1.med.uk, ex1.med.x_history, 3);
ackerman_fk_plotting(tc, ex1.fast.uk, ex1.fast.x_history, 4);

%% 打印实验 1 和实验 2 的最终速度
fprintf('Experiment 1 final acceleration\n');
fprintf('Slow:   %g\n', ex1.slow.x_history(3, end));
fprintf('Medium: %g\n', ex1.med.x_history(3, end));
fprintf('Fast:   %g\n', ex1.fast.x_history(3, end));

%% 播放轨迹动画

% play_trajectories(ex1);