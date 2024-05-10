function ackerman_fk_plotting(t, uk, x_history, i)
    % ACKERMAN_FORWARD_KINEMATICS_SIMULATION - 正运动学轨迹仿真
    %   uk : [ 发动机的推动力, 前轮横转角]
    %   v_x: 质心速度水平分量 （meter/s）
    %   v_y: 质心速度垂直分量 （meter/s）
    %   a: 质心切向加速度 （meter/s^2）
    %   theta: 质心偏转角（rad/s）
    v_x = x_history(1, :);
    v_y = x_history(2, :);
    a = x_history(3, :);
    theta = x_history(4, :);
    phi = uk(2, :);

    figure(i)
    clf
    subplot(2, 1, 1);
    plot(t, v_x);
    hold on
    plot(t, v_y);
    hold off
    grid on
    legend('切向速度', '法向速度');
    xlabel('时间 (s)');
    ylabel('质心速度曲线 (m/s)');
    
    subplot(2, 1, 2);
    % plot(t, theta.*180./pi);
    % hold on
    % plot(t, phi.*180./pi);

    plot(t, mod(theta.*180./pi, 360) - 180);
    hold on
    plot(t, mod(phi.*180./pi, 360) - 180);
    
    hold off
    ylim([-200,200])
    grid on
    legend('质心偏转角', '前轮横转角');
    xlabel('Time (s)');
    ylabel('Angle (°)');
end