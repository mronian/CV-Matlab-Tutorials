image=imread('shapes.png');
image=rgb2gray(image);
image=im2bw(image);
k=imfinfo('shapes.png');
block=3;
for i=1:block:k.Height-block-1
for j=1:block:k.Width-block-1
white=0;
black=0;
for h=i:1:i+block-1
for l=j:1:j+block-1
val=image(h,l);
if val==0
    black=black+1;
else
    white=white+1;
end
end
end
if black>=white
    do=0;
else
    do=255;
end
for lc=i:1:i+block-1
for lci=j:1:j+block-1
image(h,l)=do;
end
end
end
end
imshow(image)
