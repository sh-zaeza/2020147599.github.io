function B = normalize_columns(A)
    B = A ./ sum(A, 1);
end