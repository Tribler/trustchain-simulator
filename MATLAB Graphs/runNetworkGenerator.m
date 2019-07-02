h2 = WattsStrogatz(500,2,0.06);

disp("AvgPathLength:");
disp(mean(mean(distances(h2))));
disp("VariancePathLength: ");
disp(mean(var(distances(h2))));


plot(h2,'NodeColor','k','EdgeAlpha',0.1);
title('Watts-Strogatz Graph with $N = 500$ nodes, $K = 25$, and $\beta = 0.15$', ...
    'Interpreter','latex')