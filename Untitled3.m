image=imread('tiger.jpg');
k=imfinfo('tiger.jpg');
for i=1:1:k.Height
for j=1:1:k.Width
image(i,j)=0;
end
end
imshow(image)
