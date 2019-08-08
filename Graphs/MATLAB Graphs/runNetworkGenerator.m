clear h2
clear graph


graph = WattsStrogatz(500,5,0.01);

disp("AvgPathLength:");
disp(mean(mean(distances(graph))));
disp("VariancePathLength: ");
disp(mean(var(distances(graph))));

figure('name','Graph');

colormap hsv
deg = degree(graph);
nSizes = 2*sqrt(deg-min(deg)+0.2);
nColors = deg;
plot(graph,'MarkerSize',nSizes,'NodeCData',nColors,'EdgeAlpha',0.1)
colorbar

clear ans
clear deg
clear nSizes
clear nColors