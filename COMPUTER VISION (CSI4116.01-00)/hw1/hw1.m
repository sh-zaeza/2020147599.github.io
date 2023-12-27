% read the provided image
img = imread("pittsburgh.png");
% convert the image into grayscale
grayscale = rgb2gray(img);
% finding the darlest pixel in the image
darkest = min(grayscale(:));
[row_darkest, column_darkest] = ind2sub(size(grayscale), find(grayscale == darkest));

% creating a white square
white_squaresize = 31 ;

row_white_start = max(1, row_darkest - floor(white_squaresize/2));
row_white_end = min(size(grayscale, 1), row_darkest + floor(white_squaresize/2));
col_white_start = max(1, column_darkest - floor(white_squaresize/2));
col_white_end = min(size(grayscale, 2), column_darkest + floor(white_squaresize/2));

for r = row_white_start:row_white_end
    for c = col_white_start:col_white_end
        grayscale(r, c) = 255;
    end
end

% creating a gray center square
[rows, columns] = size(grayscale);
row_center = rows/2;
column_center = columns/2;

gray_squaresize_half = floor(121 / 2);
gray_row_start = max(1, row_center - gray_squaresize_half);
gray_row_end = min(rows, row_center + gray_squaresize_half);
gray_col_start = max(1, column_center - gray_squaresize_half);
gray_col_end = min(columns, column_center + gray_squaresize_half);

grayscale(gray_row_start:gray_row_end, gray_col_start:gray_col_end) = 150;

% saving the image
imwrite(grayscale, 'new_image.png');

% substracting the average value per channel
image_precise = double(img);

average_R = mean(mean(image_precise(:, :, 1)));
average_G = mean(mean(image_precise(:, :, 2)));
average_B = mean(mean(image_precise(:, :, 3)));

image_precise(:, :, 1) = image_precise(:, :, 1) - average_R;
image_precise(:, :, 2) = image_precise(:, :, 2) - average_G;
image_precise(:, :, 3) = image_precise(:, :, 3) - average_B;

% saving the image
imwrite(uint8(image_precise), 'mean_sub.png');