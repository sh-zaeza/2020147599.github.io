function [x, y, scores, Ih, Iv] = extract_keypoints(image)
%% extract_keypoints
% input:
%   image: colored image. Not grayscale or double yet.
% output:
%   x: n x 1 vector of x (col) locations that survive non-maximum suppression. 
%   y: n x 1 vector of y (row) locations that survive non-maximum suppression.
%   scores: n x 1 vector of R scores of the keypoints correponding to (x,y).
%   Ih: x- (horizontal) gradient. Also appeared as Ix in the slides.
%   Iv: y- (vertical) gradient. Also appeared as Iy in the slides.

% The kernels are provided, but you can try other kernels.
Ih_kernel = [1 0 -1; ...
             2 0 -2; ...
             1 0 -1];
Iv_kernel = Ih_kernel';

%% [10 pts] Part A: Setup (Implement yourself)
k = 0.05;
window_size = 5;

% Convert the input image to grayscale and double
input_image = double(rgb2gray(image));

% Compute horizontal and vertical image gradients (Ih and Iv)
Ih = imfilter(input_image, Ih_kernel);
Iv = imfilter(input_image, Iv_kernel);

% Initialize the R score matrix
[rows, cols] = size(input_image);
R = zeros(rows, cols);

%% [15 pts] Part B: R score matrix (Implement yourself)
for i = 3:(rows - 2)
    for j = 3:(cols - 2)
        % Compute the M matrix for the current window
        window_Ih = Ih(i-2:i+2, j-2:j+2);
        window_Iv = Iv(i-2:i+2, j-2:j+2);
        
        M = [sum(sum(window_Ih.^2)), sum(sum(window_Ih .* window_Iv));
             sum(sum(window_Ih .* window_Iv)), sum(sum(window_Iv.^2))];
        
        % Compute the R score for the current pixel
        R(i, j) = det(M) - k * trace(M)^2;
    end
end

%% Part C: Thresholding R scores (Provided to you, do not modify)
% Threshold standards is arbitrary, but for this assignment, I set the 
% value of the 1th percentile R as the threshold. So we only keep the
% largest 1% of the R scores (that are not -Inf) and their locations.
R_non_inf = R(~isinf(R));
top_R = sort(R_non_inf(:), 'descend');
R_threshold = top_R(round(length(top_R)*0.01));
R(R < R_threshold) = -Inf;

%% [15 pts] Part D: Non-maximum Suppression (Implement yourself)
% Initialize the output variables
x = [];
y = [];
scores = [];

% Perform non-maximum suppression
for i = 1:size(input_image, 1)
    for j = 1:size(input_image, 2)
        if R(i, j) == -Inf
            continue;  % Skip suppressed keypoints
        end
        
        % Define the neighboring indices
        neighbors = [
            i-1, j-1; i-1, j; i-1, j+1;
            i, j-1; i, j+1;
            i+1, j-1; i+1, j; i+1, j+1;
        ];
        
        % Check if R(i,j) is greater than its neighbors
        is_max = all(R(i, j) >= R(sub2ind(size(R), neighbors(:, 1), neighbors(:, 2))));
        
        if is_max
            % This is a keypoint, save its information
            x = [x; j];
            y = [y; i];
            scores = [scores; R(i, j)];
        end
    end
end