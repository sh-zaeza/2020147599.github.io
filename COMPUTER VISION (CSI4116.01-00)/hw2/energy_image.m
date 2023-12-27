function [energyImage, Ix, Iy] = energy_image(im, Ix_kernel, Iy_kernel)
    % Convert the input image to grayscale
    grayImage = rgb2gray(im);
    
    % Convert the grayscale image to double to avoid value truncation
    grayImage = double(grayImage);
    
    % Compute the x and y gradients using the specified kernels
    Ix = imfilter(grayImage, Ix_kernel);
    Iy = imfilter(grayImage, Iy_kernel);
    
    % Compute the energy image using the L2 norm of gradients
    energyImage = sqrt(Ix.^2 + Iy.^2);
end
