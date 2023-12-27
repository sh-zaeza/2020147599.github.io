%% CSI4116: Homework 2
% Due: 10/5/2022, 11:59pm HW2 skeleton code script written by Seong Jae Hwang 
% (<mailto:seongjae@yonsei.ac.kr seongjae@yonsei.ac.kr>)
% 
% This HW assignment has a long history from - Adriana Kovashka (University 
% of Pittsburgh) - Kristen Grauman (University of Texas, Austin)
% 
% Do not distribute outside of class
% 
% Follow through this pipeline to test your code, produce necessary outputs, 
% and save results. I will be using my own script similar to this, so feel free 
% to modify this script as needed.
% 
% To run the entire script, type in 'hw2' in Command Window. You can also run 
% each Section (Right click anywhere in section -> Run Section)
%% Content-Aware Image Resizing
% Setup: Comment or uncomment desired filename and seamDirection

% Image 1: prague.jpg
% filename = 'prague.jpg'; seamDirection = 'HORIZONTAL';

% Image 2: mall.jpg
% filename = 'mall.jpg'; seamDirection = 'VERTICAL';

% Image 3: my_image.jpg or my_image.png. You can modify this for your image
filename = 'my_image.png'; seamDirection = 'HORIZONTAL';

% Read the image
im = imread(filename);
%% Step 1: Simple gradient magnitude calculation.
% *Function to implement: energy_image*** The kernels are provided, but you 
% can try other kernels.

Ix_kernel = [1 0 -1; ...
             2 0 -2; ...
             1 0 -1];
Iy_kernel = Ix_kernel';
[energyImage, Ix, Iy] = energy_image(im, Ix_kernel, Iy_kernel);
%% Step 2: Compute the cumulative energy map of given seam direction.
% This function is provided.

M = cumulative_minimum_energy_map(energyImage, seamDirection);
%% Step 3: Find the optimal seam. Visualize using displaySeam.
% These functions are provided. horizontal seam = horizontal line to remove 
% and resize the height vertical seam = vertical line to remove and resize the 
% width

if strcmp(seamDirection, 'HORIZONTAL')
    seam = find_optimal_horizontal_seam(M);
elseif strcmp(seamDirection, 'VERTICAL')
    seam = find_optimal_vertical_seam(M);
end

% Plot and see if your seam makes sense
set(0, 'DefaultFigureVisible', 'on');
close all;
figure; hold on;
imshow(im);
displaySeam(im, seam, seamDirection);
hold off;
%% Step 4: Reduce the image width or height by 1
% *Function to implement: reduceWidth*** Hint: You are allowed to modify the 
% provided reduceHeight code.

display_flag = true;
if strcmp(filename, 'prague.jpg')
    reducedColorImage = reduceHeight(im, display_flag);
elseif strcmp(filename, 'mall.jpg')
    reducedColorImage = reduceWidth(im, display_flag);
else % You can modify this section for your own image 
    reducedColorImage = reduceHeight(im, display_flag);
end

% Make the figure full-screen and save as the requested '*_energy.png'
set(gcf, 'Position', get(0, 'Screensize'));
[image_name, ~] = split(filename, '.');
saveas(gcf, [image_name{1}, '_energy.png']);
%% Step 5: Reduce the image width or height multiple times
% *Function to implement: generate_results***

if strcmp(filename, 'prague.jpg')
    reduceAmt = 50;
    reduceWhat = 'HEIGHT';
elseif strcmp(filename, 'mall.jpg')
    reduceAmt = 50;
    reduceWhat = 'WIDTH';
else % You can modify this for your own image
    reduceAmt = 50;
    reduceWhat = 'HEIGHT';
end
contentAwareResize = generate_results(filename, reduceAmt, reduceWhat);
%% Step 6: Save the outputs
% Save the content-aware resized image and the energy image (Step 1)

% Reduce the image using the standard imresize function for comparison
[nr, nc, ~] = size(im);
if strcmp(reduceWhat, 'WIDTH')
    imresizeImage = imresize(im, [nr, nc - reduceAmt]);
else
    imresizeImage = imresize(im, [nr - reduceAmt, nc]);
end

% Plot and compare (1) original, (2) content-aware resize, and (3) standard
% resize
figure;
subplot(1, 3, 1); imshow(im); title('original');
subplot(1, 3, 2); imshow(contentAwareResize); title('content-aware');
subplot(1, 3, 3); imshow(imresizeImage); title('standard');
hold off;

% Make the figure full-screen and save as the requested '*_reduced.png'
set(gcf, 'Position', get(0, 'Screensize'));
[image_name, ext] = split(filename, '.');
saveas(gcf, [image_name{1}, '_reduced.png']);