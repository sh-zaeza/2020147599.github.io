function H = estimate_homography(PA, PB)
    if ~isequal(size(PA), size(PB))
        error('Points matrices different sizes');
    end
    
    N = size(PA, 1);
    A = zeros(2 * N, 9);
    
    for i = 1:N
        x = PB(i, 1);
        y = PB(i, 2);
        X = PA(i, 1);
        Y = PA(i, 2);

        A(2 * i - 1, :) = [-X, -Y, -1, 0, 0, 0, x * X, x * Y, x];
        A(2 * i, :) = [0, 0, 0, -X, -Y, -1, y * X, y * Y, y];
    end

    [~, ~, V] = svd(A);
    h = V(:, end);
    H = reshape(h, 3, 3)';
end

