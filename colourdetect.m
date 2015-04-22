image=imread('shapes.png');
k=imfinfo('shapes.png');
min=80;
max=80;
res=zeros(k.Height, k.Width);
for i=1:1:k.Height
for j=1:1:k.Width
    r=image(i,j,1);
    g=image(i,j,2);
    b=image(i,j,3);
    if (r>min&&b<max&&g<max)
        res(i,j)=255;
    else
        res(i,j)=0;
    end
end
end
imshow(res)
