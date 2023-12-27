function p2 = apply_homography(p1, H)
    % Convert the input point to homogeneous coordinates
    p1_homogeneous = [p1, 1];

    % Apply the homography transformation
    p2_homogeneous = H * p1_homogeneous';

    % Convert back to image 2 space by dividing by the third coordinate
    p2_homogeneous = p2_homogeneous ./ p2_homogeneous(end);

    % Extract the transformed point in image 2 space [x' y']
    p2 = p2_homogeneous(1:2)';
end

