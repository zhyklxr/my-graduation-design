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
 mov a, ax;Ϊa��ֵΪ4
   mov  ax,1;
 mov b, ax;Ϊb��ֵΪ1
   mov  ax,a;
   sub  ax,bx;
   mov  a,ax;������
   mov  ax,a;
   sub  ax,0;
   mov  dx,ax;������
lab 5:
   mov  ax,dx;
   cmp  ax,bx;
   jb  lab8;��ת�� 8
   mov  ax,bx;
 mov bx, ax;Ϊbx��ֵΪbx
   jmp  lab9;��ת��lab9
lab 8:
   mov  ax,bx;
 mov bx, ax;Ϊbx��ֵΪbx
lab 9:
   mov  ax,bx;
   cmp  ax,bx;
   jne  lab11;��ת�� 11
   jmp  lab17;��ת��lab17
lab 11:
   mov  ax,1;
 mov i, ax;Ϊi��ֵΪ1
lab 12:
   mov  ax,i;
   mov  cx,b;
   mul  cx;
   mov  bx,ax;���˷�
   mov  ax,bx;
 mov b, ax;Ϊb��ֵΪbx
   mov  ax,i;
   add  ax,bx;
   mov  i,ax;���ӷ�
lab 15:
   mov  ax,i;
   cmp  ax,a;
   jb  lab12;��ת�� 12
   jmp  lab17;��ת��lab17
lab 17:
       mov  ah,9h;
       mov  dx,offset display;
      int 21h;
   mov  ax,4c00h;
   int  21h;
main endp
code ends
end start
