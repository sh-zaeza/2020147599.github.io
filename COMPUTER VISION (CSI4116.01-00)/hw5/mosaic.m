% Load images
img1 = imread('keble1.png');
img2 = imread('keble2.png');

%img1 = imread('uttower1.jpeg');
%img2 = imread('uttower2.jpeg');

% Load correspondence points

% keble1
PA = [165, 78; ...
    154, 186; ...
    327, 106; ...
    354, 170; ...
    340, 14; ...
    271, 43];

% keble2
PB = [68, 88; ...
    55, 198; ...
    229, 123; ...
    252, 186; ...
    243, 34; ...
    177, 57];

% uttower1 
%PA = [481, 310; ...
%    328, 510; ...
%    106, 507; ...
%    108, 619; ...
%    56, 176; ...
%    129, 483; ...
%    526, 537; ...
%    375, 293]; 

% uttower2 
%PB = [928, 331; ...
%    782, 540; ...
%    569, 545; ...
%    578, 652; ...
%    505, 232; ...
%    585, 519; ...
%    998, 567; ...
%    816, 322];

% Compute Homography
H = estimate_homography(PA, PB);

% Check if PA transformed to image 2 space matches PB
PA_transformed = [];
for i = 1:size(PA, 1)
    p1 = PA(i,:);
    p2 = apply_homography(p1, H);
    PA_transformed = [PA_transformed; p2];
end

% Create Mosaic
nr = size(img2, 1);
nc = size(img2, 2);
canvas = uint8(zeros(3*nr, 3*nc, 3));
canvas(nr:2*nr-1, nc:2*nc-1, :) = img2;

% Loop through each pixel in img1
for y = 1:size(img1, 1)
    for x = 1:size(img1, 2)
        p1 = [x, y];
        p1_homogeneous = [p1, 1];
        p2_homogeneous = H * p1_homogeneous';
        p2_homogeneous = p2_homogeneous ./ p2_homogeneous(end);

        % Shift coordinates to the canvas system
        p2 = round(p2_homogeneous(1:2)' + [nc, nr]);

        % Check if the transformed pixel falls within canvas boundaries
        if all(p2 > 0) && p2(1) <= size(canvas, 2) && p2(2) <= size(canvas, 1)
            % Add the pixel from img1 to the corresponding location on canvas
            canvas(p2(2), p2(1), :) = img1(y, x, :);
        end
    end
end

% Save Mosaic
imwrite(canvas, 'keble_mosaic.png');
%imwrite(canvas, 'uttower_mosaic.png');
