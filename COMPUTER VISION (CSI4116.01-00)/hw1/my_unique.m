function N = my_unique(M)
    [m, ~] = size(M);
    N = M(1, :);

    for i = 2:m
        row_current = M(i, :);
        duplicate = false;
        for j = 1:size(N, 1)
            if isequal(row_current, N(j, :))
                duplicate = true;
                break;
            end
        end

        if ~duplicate
            N = [N; row_current];
        end
    end
end
