image=imread('shapes.png');
k=imfinfo('shapes.png');
edge=zeros(k.Height, k.Width);
imshow(edge);
for i=1:1:k.Width
for j=1:1:k.Height
min=0.11*image(j,i,1)+0.56*image(j,i,2)+0.33*image(j,i,3);
max=min;
for h=i-1:1:i+1
for l=j-1:1:j+1
val=0.11*image(l,h,1)+0.56*image(l,h,2)+0.33*image(l,h,3);
if val>max
max=val;
elseif val<min
min=val;
end
end
end
if (max-min)>0.5
edge(j,i)=0;
else
edge(j,i)=1;
end
end
end
