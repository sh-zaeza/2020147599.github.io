%% CSI 4116: Homework 4
% Due: 11/14/2023, 11:59pm

%% Part I
% This outputs edges and shows a figure. You can use any radii of your
% choice, including the ones I have provided below.

% jupiter.jpg example
im = imread('jupiter.jpg');
save_filename = 'jupiter_circles.png';
radius = 110;   % radius = 110, 50, 30 work well for jupiter.jpg
top_k = 1;

% egg.jpg example
% im = imread('egg.jpg');
% save_filename = 'egg_circles.png';
% radius = 7;   % radius = 7 works well for egg.jpg
% top_k = 20;   % egg has many small circles. use large top_k

% your image example
% im = imread('my_image.jpg');
% save_filename = 'my_image_circles.png';
% radius = 170;   % you need to find the right radius for your image
% top_k = 1;   % choose however many circles you want to detect

edges = detectEdges(im);
centers = detectCircles(im, edges, radius, top_k);

% Show the image and top k circles
close all;  % close existing figures
figure; imshow(im); 
viscircles(centers, radius * ones(size(centers, 1), 1));
title(sprintf('circle radius: %d', radius));
saveas(gcf, save_filename);

%% Part II
% This part is fairly easy and straightforward
origImg = imread('fish.jpg');

% Try different k's
k = 10;
[outputImg, meanColors, clusterIds] = quantizeRGB(origImg, k);

close all;  % close existing figures
figure;
subplot(1,2,1); imshow(origImg); title('Original image')
subplot(1,2,2); imshow(outputImg); title(sprintf('Quantized image (RGB) for k=%d', k));
saveas(gcf, sprintf('fish_k%d.png', k));