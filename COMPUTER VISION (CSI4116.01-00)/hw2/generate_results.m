function contentAwareResize = generate_results(filename, reduceAmt, reduceWhat)
    % Read the image from the specified filename
    im = imread(filename);
    
    % Determine whether to reduce width or height
    if strcmp(reduceWhat, 'WIDTH')
        for i = 1:reduceAmt
            im = reduceWidth(im, false);
        end
    elseif strcmp(reduceWhat, 'HEIGHT')
        for i = 1:reduceAmt
            im = reduceHeight(im, false);
        end
    else
        error('Invalid value for reduceWhat. Use either "WIDTH" or "HEIGHT".');
    end
    
    % Output the content-aware resized image
    contentAwareResize = im;
end
