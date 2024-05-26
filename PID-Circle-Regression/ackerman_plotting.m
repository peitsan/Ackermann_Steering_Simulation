function ackerman_plotting(t, state_history, control_history, l,w, a,b,c)

x = state_history(1, :);
y = state_history(2, :);
theta = state_history(3, :);
phi = state_history(4, :);
v = state_history(5, :);
v_l = v .* (1 + tan(theta)/2);
v_r = v .* (1 - tan(theta)/2);


F = control_history(1, :);
dphi = control_history(2, :);

figure(a)
subplot(3, 2, 1);
patch(t, x, y, 'b');
xlabel('Time')
ylabel('位置 (m)')
legend('X', 'Y')

subplot(3, 2, 2);
patch(t, theta, phi, 'b');
xlabel('Time')
ylabel('方向 (rad)');
legend('Theta', 'Phi');

subplot(3, 2, 3);
patch(t, v, 'b');
xlabel('Time');
ylabel('质心线速度 (m/s)');

subplot(3, 2, 5);
patch(t(2:end), F, 'b');
xlabel('Time');
ylabel('Input Force (N)');

subplot(3, 2, 6);
patch(t(2:end), dphi, 'b');
xlabel('Time');
ylabel('前轮横转角加速度 (rad/s)');

subplot(3, 2, 4);
hold on
plot_car_traj(state_history, l, w);
hold off
axis([min(x)-l*1.5, max(x)+l*1.5, min(y)-l*1.5, max(y)+l*1.5]);
axis equal
xlabel('X (m)');
ylabel('Y (m)');

figure(b)
clf
subplot(2, 1, 1);
plot(t, v_l);
hold on
plot(t, v_r);
hold on
plot(t, v);
hold off
grid on
legend('左轮速度', '右轮速度','质心速度');
xlabel('Time');
ylabel('Wheel Velocity (m/s)');

subplot(2, 1, 2);
plot(t, theta.*180./pi);
hold on
plot(t, phi.*180./pi);
hold off
ylim([-160 160])
grid on
legend('质心偏转角', '前轮横摆角');
xlabel('Time (s)');
ylabel('Angle (°)');

% 目标轨迹的圆心和半径
circle_center = [0, 0];
circle_radius = 2;

% 计算目标轨迹上的点（使用极坐标方程）
theta = linspace(0, 2*pi, 1000); % 生成1000个点在圆上
circle_x = circle_center(1) + circle_radius * cos(theta);
circle_y = circle_center(2) + circle_radius * sin(theta);

% 计算小车轨迹上每个点到目标轨迹圆心的距离
distance_to_circle_center = sqrt(x.^2 + y.^2);

% 计算误差：小车轨迹上每个点到圆心的距离与目标半径的差
error = distance_to_circle_center - circle_radius;

% 绘制误差图
figure(c);
plot(error, 'LineWidth', 2);
title('小车轨迹与目标圆形轨迹的误差');
xlabel('轨迹点序号');
ylabel('误差 (单位: 米)');

fprintf('误差平均值:   %g\n', mean(error));

end