graph = WattsStrogatz(500,2,0.05);

disp("AvgPathLength:");
disp(mean(mean(distances(graph))));
disp("VariancePathLength: ");
disp(mean(var(distances(graph))));

figure('name','Graph with $N = 500$ nodes');

colormap hsv
deg = degree(graph);
nSizes = 2*sqrt(deg-min(deg)+0.2);
nColors = deg;
plot(graph,'MarkerSize',nSizes,'NodeCData',nColors,'EdgeAlpha',0.1)
title('Watts-Strogatz Graph with $N = 500$ nodes, $K = 25$, and $\beta = 0.15$','Interpreter','latex')
colorbar

clear ans
clear deg
clear nSizes
clear nColors