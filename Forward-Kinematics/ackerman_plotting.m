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
ylabel('Position (m)')
legend('X', 'Y')

subplot(3, 2, 2);
patch(t, theta, phi, 'b');
xlabel('Time')
ylabel('Orientation (rad)');
legend('Theta', 'Phi');

subplot(3, 2, 3);
patch(t, v, 'b');
xlabel('Time');
ylabel('Velocity (m/s)');

subplot(3, 2, 5);
patch(t(2:end), F, 'b');
xlabel('Time');
ylabel('Input Force (N)');

subplot(3, 2, 6);
patch(t(2:end), dphi, 'b');
xlabel('Time');
ylabel('Steering Angle Change (rad/s)');

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
legend('Left Wheel', 'Right Wheel');
xlabel('Time');
ylabel('Wheel Velocity (m/s)');

subplot(2, 1, 2);
plot(t, theta);
hold on
plot(t, phi);
hold off
legend('轮子离轴线切向角', '轮子离轴线法向角');
xlabel('Time (s)');
ylabel('Angle (m/s)');
end