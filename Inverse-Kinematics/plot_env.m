function plot_env(ex1)

%% First display the lines representing the entire trajectories
%% Ex1
subplot(1, 3, 1)
title(sprintf('Slow start v = %g', ex1.slow.x0(end)))
plot_obstacle(ex1.obstacle);
hold on
plot(ex1.lifted_y_ref(1, :), ex1.lifted_y_ref(2, :), ex1.ref_color);
if isfield(ex1.slow, 'x_history')
    plot(ex1.slow.x_history(1, :), ex1.slow.x_history(2, :), ex1.slow.color);
end
axis equal
hold off
ylim([-2,8])
xlim([-1,5])

subplot(1, 3, 2)
title(sprintf('Medium start v = %g', ex1.med.x0(end)))
plot_obstacle(ex1.obstacle);
hold on
plot(ex1.lifted_y_ref(1, :), ex1.lifted_y_ref(2, :), ex1.ref_color);
if isfield(ex1.med, 'x_history')
    plot(ex1.med.x_history(1, :), ex1.med.x_history(2, :), ex1.med.color);
end
axis equal
hold off
ylim([-2,8])
xlim([-1,5])

subplot(1, 3, 3)
title(sprintf('Fast start v = %g', ex1.fast.x0(end)))
plot_obstacle(ex1.obstacle);
hold on
plot(ex1.lifted_y_ref(1, :), ex1.lifted_y_ref(2, :), ex1.ref_color);
if isfield(ex1.fast, 'x_history')
    plot(ex1.fast.x_history(1, :), ex1.fast.x_history(2, :), ex1.fast.color);
end
axis equal
hold off
ylim([-2,8])
xlim([-1.5,4])

end