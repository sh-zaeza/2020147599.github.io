function B = normalize_rows(A)
    B = A ./ sum(A, 2);
end