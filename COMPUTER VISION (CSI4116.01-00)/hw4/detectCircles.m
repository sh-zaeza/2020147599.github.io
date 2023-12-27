function [centers] = detectCircles(im, edges, radius, top_k)
    % initialize a given parameters
    quantization_value = 5;
    
    % initialize rows and columns + accomularor matrix
    [rows, cols, ~] = size(im);
    accumulator = zeros(rows, cols);
    
    % quantize gradient orientations
    quantized_orientations = round(edges(:, 4) / quantization_value) * quantization_value;
    
    % iterate through edge points
    for i = 1:size(edges, 1)
        x = edges(i, 1);
        y = edges(i, 2);
        
        % calculate possible circle centers
        x_center = round(x - radius * cosd(quantized_orientations(i)));
        y_center = round(y - radius * sind(quantized_orientations(i)));
        
        % ignore centers outside the image
        if x_center > 0 && x_center <= cols && y_center > 0 && y_center <= rows
            accumulator(y_center, x_center) = accumulator(y_center, x_center) + 1;
        end
    end
    
    % find top_k circles with the highest votes
    [~, sorted_indices] = sort(accumulator(:), 'descend');
    top_indices = sorted_indices(1:top_k);
    
    % convert indices to (x, y) coordinates
    [top_y, top_x] = ind2sub(size(accumulator), top_indices);
    
    % combine x and y coordinates into centers matrix
    centers = [top_x, top_y];
end

