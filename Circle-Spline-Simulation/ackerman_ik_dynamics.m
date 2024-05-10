function dstate = ackerman_ik_dynamics(state, control, l, M1, M2)
%ACKERMAN_DYNAMICS - 从https://ieeexplore.ieee.org/document/7075182中获取
%   params(1) = l       车辆长度
%   params(2) = M1      车辆后轴上的质量
%   params(3) = M2      车辆前轴上的质量
%   state(1) = x        车辆在世界X坐标系中的位置（论文中的x1）
%   state(2) = y        车辆在世界Y坐标系中的位置（论文中的x2）
%   state(3) = theta    车辆在世界θ坐标系中的方向
%   state(4) = phi      方向盘的转动（相对于θ）
%   state(5) = v        后轴中心的速度（论文中的u1 - 与论文中的控制输入1混淆）
%   control(1) = F      发动机的推动力
%   control(2) = u      转向角的变化

% l = params(1);
% M1 = params(2);
% M2 = params(3);

x = state(1);
y = state(2);
theta = state(3); %
phi = state(4);
v = state(5); 


F = control(1);
u = control(2);

dx = v * cos(theta);  
dy = v * sin(theta);
dtheta = v/l * tan(phi);
dphi = u;
dv = (F - M2 * dphi * sin(phi) / cos(phi)^3) / (M1 + M2 + M2 * tan(phi)^2);
% dv_l = v * (1 + tan(phi)/2);
dstate = [dx; dy; dtheta; dphi; dv];

end
