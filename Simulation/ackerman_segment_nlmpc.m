function [x_history, u_history, y_ref] = ...
    ackerman_segment_nlmpc(nlobj, nlopt, x0, y_target, l, M1, M2, dscale)

Ts = nlobj.Ts;  % 采样时间
prediction_horizon = nlobj.PredictionHorizon;  % 预测时域
control_horizon = nlobj.ControlHorizon;  % 控制时域

dist = se2_dist(x0(1:3), y_target);  % 计算起点和目标点之间的距离
duration = dist / 5.0;  % 根据速度计算运行时间
Tsteps = ceil(duration / Ts);  % 计算总步数

t = 0:Ts:Ts*Tsteps;  % 时间向量
x_history = zeros(5, Tsteps + 1);  % 存储状态历史
x_history(:, 1) = x0;  % 初始状态
u_history = zeros(2, Tsteps);  % 存储控制历史
u_prev = [0; 0];  % 上一步的控制输入

% 创建沿线段的参考目标点，避免超调
% 参考：http://www.cs.cornell.edu/courses/cs4620/2013fa/lectures/16spline-curves.pdf
y_ref = se2_spline(x0(1:3), y_target, Tsteps, dscale)';  % 生成参考目标点
y_ref = y_ref(2:end, :);  % 去除起点

% dist = se3_dist(x0(1:3), y_target);
% delta = se3_delta(x0(1:3), y_target);
% dx = linspace(0, delta(1), Tsteps + 1);
% dy = linspace(0, delta(2), Tsteps + 1);
% dtheta = linspace(0, delta(3), Tsteps + 1);
% y_ref = ones(Tsteps + 1, 1) * x0(1:3)' + [dx', dy', dtheta'];
% y_ref = y_ref(2:end, :);

hbar = waitbar(0,'Simulation Progress');  % 进度条
for k = 1:Tsteps
    
    xk = x_history(:, k);  % 当前状态
    % 更新nlobj以避免预测超过总步数，即允许“残余”速度
    steps_left = Tsteps - k + 1;
    ph = min(prediction_horizon, steps_left);
    ch = min(control_horizon, steps_left);    
    nlobj.PredictionHorizon = ph;
    nlobj.ControlHorizon = ch;
    if size(nlopt.MV0, 1) > nlobj.ControlHorizon
        nlopt.MV0 = nlopt.MV0(1:nlobj.ControlHorizon, :);
    end
        
    [uk, nlopt, info] = nlmpcmove(nlobj, xk, u_prev, y_ref(k:k+ph-1, :), [], nlopt);  % 使用NL MPC控制器计算控制输入
    u_history(:, k) = uk;  % 存储控制输入
    u_prev = uk;  % 更新上一步的控制输入
    
    ODEFUN = @(t, xk) ackerman_dynamics(xk, uk, l, M1, M2);  % 定义ODE函数
    [TOUT, YOUT] = ode45(ODEFUN,[0 Ts], xk');  % 使用ODE求解状态方程
    x_history(:, k+1) = YOUT(end, :);  % 存储下一步的状态
    waitbar(k/Tsteps, hbar);  % 更新进度条
end
close(hbar)  % 关闭进度条

end