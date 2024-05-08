function ackerman_plotting(t, state_history, control_history, l, w)
% 绘制Ackermann转向仿真结果的图形

x = state_history(1, :); % 获取车辆的x坐标
y = state_history(2, :); % 获取车辆的y坐标
theta = state_history(3, :); % 获取车辆的方向角
phi = state_history(4, :); % 获取车辆的转向角
v = state_history(5, :); % 获取车辆的速度

F = control_history(1, :); % 获取输入力
dphi = control_history(2, :); % 获取转向角变化率

figure(1)
clf;
subplot(3, 2, 1);
patch(t, [x; y]');
xlabel('时间')
ylabel('位置 (m)')
legend('X', 'Y')

subplot(3, 2, 2);
patch(t, [theta; phi]');
xlabel('时间')
ylabel('方向 (rad)');
legend('Theta', 'Phi');

subplot(3, 2, 3);
patch(t, v);
xlabel('时间');
ylabel('速度 (m/s)');

subplot(3, 2, 5);
patch(t(2:end), F);
xlabel('时间');
ylabel('输入力 (N)');

subplot(3, 2, 6);
patch(t(2:end), dphi);
xlabel('时间');
ylabel('转向角变化率 (rad/s)');

subplot(3, 2, 4);
hold on
plot_car_traj(state_history, l, w);
hold off
axis([min(x)-l*1.5, max(x)+l*1.5, min(y)-l*1.5, max(y)+l*1.5]);
axis equal
xlabel('X (m)');
ylabel('Y (m)');

end