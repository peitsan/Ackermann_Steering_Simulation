clc; clear;
% 清除命令窗口和工作空间变量

% 定义类似于2006年款本田思域的车辆参数
% l = 2.7; % 轴距长度
% w = 1.5; % 轮距
% slow = 1; % 慢速 m/s (3.6 kph, 2.240 mph)
% med = 7; % 中速 m/s (27 kph, 16.777 mph)
% fast = 15; % 快速 m/s (54 kph, 33.554 mph)
% dscale = 50; % se3_spline 使用的参数，定义生成的参考样条曲线的“曲折度”
%
% M = 1330; % 千克
% M1 = M/2;
% M2 = M/2;
% duration = 30;

% 生成合理的图形
l = 1.0; % 轴距长度
w = 0.5; % 轮距
slow = 1; % m/s (3.6 kph, 2.240 mph)
med = 7; % m/s (27 kph, 16.777 mph)
fast = 15; % m/s (54 kph, 33.554 mph)
vfast = 20; % m/s (72 kph, 44.738 mph)
% dscale = 9; % se2_spline 使用的参数，定义生成的参考样条曲线的“曲折度” pi/10
dscale = 12.9; % se2_spline 使用的参数，定义生成的参考样条曲线的“曲折度”

M = 1;
M1 = M/2;
M2 = M/2;

nx = 5; % 状态变量的维度
ny = 3; % 输出变量的维度
nu = 2; % 控制变量的维度
Ts = 0.05; % 采样时间
mpc_horizon = 8;

% 创建非线性模型预测控制器对象
nlobj = nlmpc(nx, ny, nu);
nlobj.Ts = Ts;
nlobj.PredictionHorizon = mpc_horizon;
nlobj.ControlHorizon = mpc_horizon;

nlobj.Model.IsContinuousTime = true;
nlobj.Model.NumberOfParameters = 3;
nlobj.Model.StateFcn = @ackerman_dynamics;
nlobj.Model.OutputFcn = @(state, control, l, M1, M2) state(1:3);

% 定义状态变量
nlobj.States(1).Name = 'x';
nlobj.States(1).Units = 'Meters';

nlobj.States(2).Name = 'y';
nlobj.States(2).Units = 'Meters';

nlobj.States(3).Name = 'theta';
nlobj.States(3).Units = 'Radians';

nlobj.States(4).Name = 'phi';
nlobj.States(4).Units = 'Radians';
nlobj.States(4).Min = -deg2rad(75);
nlobj.States(4).Max = deg2rad(75);

nlobj.States(5).Name = 'v';
nlobj.States(5).Units = 'Meters / Second';
nlobj.States(5).Min = 0.0;
nlobj.States(5).Max = 20.0;


% 定义控制变量
nlobj.ManipulatedVariables(1).Name = 'F';
nlobj.ManipulatedVariables(1).Units = 'Newtons';
nlobj.ManipulatedVariables(1).Min = -80;
nlobj.ManipulatedVariables(1).Max = 20;

nlobj.ManipulatedVariables(2).Name = 'Steering Change';
nlobj.ManipulatedVariables(2).Units = 'Rad / Second';

% 定义雅可比矩阵函数
nlobj.Jacobian.StateFcn = @ackerman_state_jacobian;
nlobj.Jacobian.OutputFcn = @(state, control, l, M1, M2) [[1 0 0 0 0]', [0 1 0 0 0]', [0 0 1 0 0]']';

% 使用零状态和控制输入验证函数
validateFcns(nlobj, zeros(5, 1), zeros(2, 1), [], {l, M1, M2});

% 创建非线性模型预测控制器的移动选项对象
nlopt = nlmpcmoveopt;
nlopt.Parameters = {l, M1, M2};
nlopt.OutputWeights = [10, 10, 10];
nlopt.MVWeights = [0.1, 0.01];
nlopt.MVRateWeights = [1, 1];

%% 实验 1 设置
ex1.l = l;
ex1.w = w;
ex1.obstacle.bl = [-1, -1]; % 障碍物左下角坐标
ex1.obstacle.tr = [1, 1]; % 障碍物右上角坐标
ex1.y0 = [-2; 0; pi/2]; % 初始位置
ex1.y1 = [2*cos(pi/10*3);-2*sin(pi/10*3); -(pi/10*3 + pi/2)]; % 目标位置
ex1.slow.x0 = [ex1.y0; 0; slow]; % 慢速初始状态
ex1.med.x0 = [ex1.y0; 0; med]; % 中速初始状态
ex1.fast.x0 = [ex1.y0; 0; fast]; % 快速初始状态
ex1.vfast.x0 = [ex1.y0; 0; vfast]; % 快速初始状态
% 打印实验 1 开始和起点到终点的距离
fprintf(sprintf('Ex 1 Start, Goal dist: %g\n', se2_dist(ex1.y0, ex1.y1)))

% 定义颜色
ex1.ref_color = 'k';
ex1.slow.color = 'b';
ex1.med.color = 'g';
ex1.fast.color = 'm';
ex1.vfast.color = 'r';

% 计算持续时间、时间步长，并生成参考轨迹
duration = se2_dist(ex1.y0, ex1.y1) / 5.0;
Tsteps = ceil(duration / Ts);
ex1.y_ref = [2*cos(linspace(0, 2*pi, Tsteps+1)); 2*sin(linspace(0, 2*pi, Tsteps+1)); zeros(1, Tsteps+1)];
ex1.lifted_y_ref = [ex1.y_ref; zeros(2, Tsteps + 1)];

%% 绘制环境
figure(1); clf
plot_env(ex1);

%% 实验 1 - 慢速
[ex1.slow.x_history, ex1.slow.u_history] = ...
    ackerman_segment_nlmpc(nlobj, nlopt, ex1.slow.x0, ex1.y1, l, M1, M2, dscale);

%% 绘制慢速轨迹
subplot(2, 2, 1);
plot_car_traj(ex1.slow.x_history, l, w, ex1.slow.color);
title(sprintf('Slow start v = %g', ex1.slow.x0(end)))
drawnow      

%% 实验 1 - 中速
[ex1.med.x_history, ex1.med.u_history] = ...
    ackerman_segment_nlmpc(nlobj, nlopt, ex1.med.x0, ex1.y1, l, M1, M2, dscale);

%% 绘制中速轨迹
subplot(2, 2, 2);
plot_car_traj(ex1.med.x_history, l, w, ex1.med.color);
title(sprintf('Medium start v = %g', ex1.med.x0(end)))
drawnow

%% 实验 1 - 快速
[ex1.fast.x_history, ex1.fast.u_history] = ...
    ackerman_segment_nlmpc(nlobj, nlopt, ex1.fast.x0, ex1.y1, l, M1, M2, dscale);

%% 绘制快速轨迹
subplot(2, 2, 3);
plot_car_traj(ex1.fast.x_history, l, w, ex1.fast.color);
title(sprintf('Fast start v = %g', ex1.fast.x0(end)))
drawnow

%% 实验 1 - 超速
[ex1.vfast.x_history, ex1.vfast.u_history] = ...
    ackerman_segment_nlmpc(nlobj, nlopt, ex1.vfast.x0, ex1.y1, l, M1, M2, dscale);

%% 绘制快速轨迹
subplot(2, 2, 4);
plot_car_traj(ex1.vfast.x_history, l, w, ex1.vfast.color);
title(sprintf('Vast Fast start v = %g', ex1.vfast.x0(end)))
drawnow

%% 绘制快速轨迹的正运动学参数
tc = 0:Ts:(size(ex1.fast.x_history,2)-1)*Ts;
ackerman_plotting(tc, ex1.fast.x_history, ex1.fast.u_history, l, w);


%% 打印实验 1 的最终速度
fprintf('Experiment 1 final velocities\n');
fprintf('Slow:   %g\n', ex1.slow.x_history(5, end));
fprintf('Medium: %g\n', ex1.med.x_history(5, end));
fprintf('Fast:   %g\n', ex1.fast.x_history(5, end));
fprintf('Fast:   %g\n', ex1.vfast.x_history(5, end));
fprintf('\nExperiment 2 final velocities\n');

%% 播放轨迹动画

play_trajectories(ex1);

