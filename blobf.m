function p=push(a,b)
top=top+1;
stack(top,1)=a;
stack(top,2)=b;
end

function c=colr(m,n)
shape=shape+1;
push(m,n);
a(m,n)=0;
while top>0
    
    x=stack(top,1);
    y=stack(top,2);
    top=top-1;
    if I(x,y)==1
        a(x,y)=shape;
    end
    for i=x-1:1:x+1
        for j=y-1:1:y+1
            if I(i,j)==0&&a(x,y)==-1)
                push(i,j);
            end
        end
    end
end
end