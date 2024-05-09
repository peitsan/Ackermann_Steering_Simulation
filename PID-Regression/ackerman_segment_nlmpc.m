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
u_history = zeros(2, Tsteps);  % 存储控制历史，这里假设有两个控制输入
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
    
    % 计算误差和误差变化率
    error = y_ref(k, :) - xk(1:3)';
    if k == 1  
        error_rate = zeros(3, 1);
    else
        error_rate = (error - (xk(1:3)' - x_history(1:3, k-1))) / Ts;
    end
    
    % PID控制器参数
    Kp = [1, 1, 0.5];  % 比例增益
    Ki = [0.1, 0.1, 0.05];  % 积分增益
    Kd = [0, 0, 0.1];  % 微分增益，对于角度通常微分项较小或为0

    % 计算控制输入
     % 确保error和error_rate是列向量
     error_col = error(:);
     error_rate_col = error_rate(:);
 
     % 计算积分项时，确保cumsum返回的是向量
     integral_error = cumsum(error_col) * Ts;
     
     % 逐元素乘法和加法
    %  uk = Kp .* [error(1), error(2)] + Ki .* cumsum([error(1), error(2)], 1) * Ts + Kd .* [error_rate(1), error_rate(2)];
     %uk = Kp .* error + Ki .* cumsum(error, 1) * Ts + Kd .* error_rate;
     uk = zeros(2, 1);  % 初始化控制输入向量
     uk(1) = Kp(1) * error(1) + Ki(1) * cumsum(error(1), 1) * Ts + Kd(1) * error_rate(1);  % 速度控制量
     uk(2) = Kp(2) * error(2) + Ki(2) * cumsum(error(2), 1) * Ts + Kd(2) * error_rate(2);  % 转向角控制量

     % 确保uk是一个两元素的列向量
     u_history(:, k) = uk;  % 存储控制输入
     u_prev = uk;  % 更新上一步的控制输入
    
    ODEFUN = @(t, xk) ackerman_dynamics(xk, uk, l, M1, M2);  % 定义ODE函数
    [TOUT, YOUT] = ode45(ODEFUN,[0 Ts], xk');  % 使用ODE求解状态方程
    x_history(:, k+1) = YOUT(end, :);  % 存储下一步的状态
    waitbar(k/Tsteps, hbar);  % 更新进度条
end
close(hbar)  % 关闭进度条

end