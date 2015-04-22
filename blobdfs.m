function blob
global I;
I=imread('shapes.png');
k=imfinfo('shapes.png');
I=rgb2gray(I);
I=im2bw(I,0.75);
global a;
a=-1*ones(k.Height, k.Width);
blobimg=zeros(k.Height, k.Width, 3);
global shape;
global top;
global stack;
shape=0;
top=0;
stack=zeros(k.Height*k.Width, 2);
ht=k.Height;
wd=k.Width;
for r=1:1:ht
    for l=1:1:wd
        if (I(r,l)==0)&&(a(r,l)==-1)
            dfs(r,l);
        end
    end
end
for r=1:1:ht
    for l=1:1:wd
        if a(r,l)==-1
            blobimg(r,l,1)=0;
            blobimg(r,l,2)=0;
            blobimg(r,l,3)=0;
        else
            blobimg(r,l,2)=a(r,l)*2;
            blobimg(r,l,3)=a(r,l)*a(r,l);
            blobimg(r,l,1)=a(r,l)*3;
        end
    end
end
imshow(blobimg);
end

function push(a,b)
global top;
global stack;
top=top+1;
stack(top,1)=a;
stack(top,2)=b;
end

function pop()
global top;
top=top-1;
end

function dfs(m,n)
global a;
global stack;
global shape;
global top;
global I;
shape=shape+1;
push(m,n);
a(m,n)=0;
chk=0;
while top>0
    
    x=stack(top,1);
    y=stack(top,2);
    if I(x,y)==0
        a(x,y)=shape/5;
        chk=0;
    end
    for i=x-1:1:x+1
        for j=y-1:1:y+1
            if (I(i,j)==0)&&(a(i,j)==-1)
                push(i,j);
                chk=1;
                break
            end
        end
        if chk==1
            break;
        end
    end
    if chk==0
        pop();
        continue;
    end
    if chk==1
    continue;
    end
end
end
