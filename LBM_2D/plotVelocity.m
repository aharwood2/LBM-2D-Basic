data = csvread('velocity.csv');
nx = data(1);
ny = data(2);
data(1) = [];
data(1) = [];
data = reshape(data, ny, nx);
surf(1:nx, 1:ny, data);
view (2);
axis equal