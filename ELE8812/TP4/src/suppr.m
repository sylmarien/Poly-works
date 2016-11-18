function g = suppr(image,a,b,delta)
%SUPPR Summary of this function goes here
%   Detailed explanation goes here

[M,N] = size(image);
a = round(M*a);
b = round(N*b);
x1 = a(1);
y1 = a(2);
x2 = b(1);
y2 = b(2);
g = image;

for x = 1:M
	for y = 1:N
		d = abs(x*(y2-y1) - y*(x2-x1) - (x1*y2 - x2*y1)) / sqrt((y2-y1)^2 + (x2-x1)^2);
		if d <= delta
			g(x,y) = 0;
		end
	end
end


end

