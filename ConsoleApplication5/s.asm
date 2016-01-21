data segment
i  dw  ?;
a  dw  ?;
b  dw  ?;
display db 'this is an example of simple compiler.$'
data ends
code segment
assume cs:code,ds:data
main proc   far
start:push ds;  
       mov  ax,0;
       push ax;
       mov  ax,data;
       mov  ds,ax;
   mov  ax,4;
 mov a, ax;为a赋值为4
   mov  ax,1;
 mov b, ax;为b赋值为1
   mov  ax,a;
   sub  ax,bx;
   mov  a,ax;做减法
   mov  ax,a;
   sub  ax,0;
   mov  dx,ax;做减法
lab 5:
   mov  ax,dx;
   cmp  ax,bx;
   jb  lab8;跳转至 8
   mov  ax,bx;
 mov bx, ax;为bx赋值为bx
   jmp  lab9;跳转至lab9
lab 8:
   mov  ax,bx;
 mov bx, ax;为bx赋值为bx
lab 9:
   mov  ax,bx;
   cmp  ax,bx;
   jne  lab11;跳转至 11
   jmp  lab17;跳转至lab17
lab 11:
   mov  ax,1;
 mov i, ax;为i赋值为1
lab 12:
   mov  ax,i;
   mov  cx,b;
   mul  cx;
   mov  bx,ax;做乘法
   mov  ax,bx;
 mov b, ax;为b赋值为bx
   mov  ax,i;
   add  ax,bx;
   mov  i,ax;做加法
lab 15:
   mov  ax,i;
   cmp  ax,a;
   jb  lab12;跳转至 12
   jmp  lab17;跳转至lab17
lab 17:
       mov  ah,9h;
       mov  dx,offset display;
      int 21h;
   mov  ax,4c00h;
   int  21h;
main endp
code ends
end start
