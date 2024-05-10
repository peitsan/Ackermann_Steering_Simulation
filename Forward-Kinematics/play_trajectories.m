function play_trajectories(ex1)

% Iterate through the states, displaying each in turn
max_t = size(ex1.y_ref, 2);

for idx = 1:max_t
    figure(5)
    clf
    plot_env(ex1)
    ex1_idx = min(idx, size(ex1.y_ref, 2));
    
    subplot(1, 3, 1);
    hold on; 
    plot_car(ex1.slow.x_history(1, ex1_idx), ...
             ex1.slow.x_history(2, ex1_idx), ...
             ex1.slow.x_history(3, ex1_idx), ...
             ex1.slow.x_history(4, ex1_idx), ...
             ex1.l, ex1.w, ex1.slow.color);
    
    subplot(1, 3, 2);
    hold on; 
    plot_car(ex1.med.x_history(1, ex1_idx), ...
             ex1.med.x_history(2, ex1_idx), ...
             ex1.med.x_history(3, ex1_idx), ...
             ex1.med.x_history(4, ex1_idx), ...
             ex1.l, ex1.w, ex1.med.color);
    
    subplot(1, 3, 3);
    hold on; 
    plot_car(ex1.fast.x_history(1, ex1_idx), ...
             ex1.fast.x_history(2, ex1_idx), ...
             ex1.fast.x_history(3, ex1_idx), ...
             ex1.fast.x_history(4, ex1_idx), ...
             ex1.l, ex1.w, ex1.fast.color);
  
    drawnow;
    pause(0.01);
end

end
