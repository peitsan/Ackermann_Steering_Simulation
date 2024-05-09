function ackerman_plotting(t, state_history, control_history, l, w)

x = state_history(1, :);
y = state_history(2, :);
theta = state_history(3, :);
phi = state_history(4, :);
v = state_history(5, :);
v_l = v .* (1 + tan(phi)/2);
v_r = v .* (1 - tan(phi)/2);


F = control_history(1, :);
dphi = control_history(2, :);

figure(2)
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

figure(3)
clf
subplot(2, 1, 1);
plot(t, v_l);
hold on
plot(t, v_r);
hold off
grid on
legend('左轮速度', '右轮速度');
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
end