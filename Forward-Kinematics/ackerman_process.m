function [x_history, y_ref] = ...
    ackerman_process(nlobj, uk, ex1, y_target, l, w,M1, M2, dscale)

Ts = nlobj.Ts;
x0 = ex1.x0;

dist = se2_dist(x0(1:3), y_target);
duration = dist / 5.0;
Tsteps = ceil(duration / Ts);

t = 0:Ts:Ts*Tsteps;
x_history = zeros(4, Tsteps + 1);

% Create reference targets along the line segment to avoid overshooting
% http://www.cs.cornell.edu/courses/cs4620/2013fa/lectures/16spline-curves.pdf
y_ref = se2_spline(x0(1:3), y_target, Tsteps, dscale)';
y_ref = y_ref(2:end, :);


hbar = waitbar(0,'Simulation Progress');
state = ex1.input;
[v_x, v_y, a, theta] = ackerman_fk_dynamics(state, uk, l, w)
x_history(:) = [v_x; v_y; a; theta]

close(hbar)

end
