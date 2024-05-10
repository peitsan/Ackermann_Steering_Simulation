function [v_x, v_y, a, theta] = ackerman_fk_dynamics(state, control, l, w)
    % ACKERMAN_FORWARD_KINEMATICS - 正运动学模型

    % 解析输入参数
    phi = state(1,:); % 转向角
    v_l = state(2,:); % 后左轮线速度
    v_r = state(3,:); % 后右轮线速度
    F = control(1,:); % 发动机的推动力
    u = control(2,:); % 前轮转向角


    % 计算车辆质心速度矢量
    v_x = (v_l + v_r) / 2;       % 纵向速度的平均值
    v_y = (v_r - v_l) .* tan(u) ./ (2 * w);  % 横向速度，考虑车轮间距

    % 计算车辆质心切向加速度
    a = (v_r - v_l) ./ w;         % 切向加速度

    % 计算质心偏转角
    theta = atan(l .* tan(u) ./ (2 .* cos(phi)));

    % 由于所有计算都是按位运算，返回的变量已经是行向量
    % 如果需要确保返回的是行向量，可以使用方括号包围
    v_x = [v_x];
    v_y = [v_y];
    a = [a];
    theta = [theta];
    disp(v_x)
end