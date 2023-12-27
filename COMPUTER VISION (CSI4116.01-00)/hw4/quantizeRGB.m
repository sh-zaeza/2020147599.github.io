function [outputImg, meanColors, clusterIds] = quantizeRGB(origImg, k)
    % reshape the original image to a matrix with RGB features as its rows
    [numRows, numCols, ~] = size(origImg);
    X = double(reshape(origImg, [numRows * numCols, 3]));

    % apply k-means clustering
    [clusterIds, meanColors] = kmeans(X, k, 'MaxIter', 100);

    % assign each pixel in the original image to its corresponding cluster mean color
    outputImg = zeros(numRows * numCols, 3, 'uint8');
    for i = 1:k
        indices = (clusterIds == i);
        outputImg(indices, :) = uint8(repmat(meanColors(i, :), [sum(indices), 1]));
    end

    % reshape the output image to the original size
    outputImg = reshape(outputImg, [numRows, numCols, 3]);
end
