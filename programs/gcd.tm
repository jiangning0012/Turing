;step1:拷贝tape1到tape2，并使tape1只剩b
;step2:对tape2求余数a mod b
;step3:将tape2上的内容粘贴到tape1的后面，重复step1

;接受态就是tape2为空


#Q = {q0,one1,one2,two1,two2,two3,two4,two5,two6,two7,two8,two9,three1,three2,three3,three4,three5,halt_accept}
#S = {0,1}
#G = {0,1,X,Y,S,_}
#q0 = q0
#B = _
#N = 2
#F = {halt_accept}

;q0:start state
q0 1_ 1S *r one1
q0 0_ 0S *r one1  ;S标注tape2最左端

;step1
one1 1_ _1 rr one1
one1 0_ _0 rr one2
one2 1_ 11 rr one2
one2 __ __ ** two1

;step2 a mod b
two1 __ __ *l two2  ;向左移一位，找到b的最后一个1
two2 _1 _Y *l two3  ;标记为Y并移动到0
two3 _1 _1 *l two3
two3 _0 _0 *l two4  ;当前在0
two4 _0 _0 *l two4  ;向左移并找到a的最后一个1或B
two4 _X _X *l two4

;two4 __ ;找到B,a<b X变1 0,1,Y变B

two4 _1 _X *r two5  ;若为1则标记这个为X
two5 _0 _0 *r two5
two5 _1 _1 *r two5
two5 _X _X *r two5  ;向右直到Y
two5 _Y _Y *l two6  ;向左看一眼
two6 _1 _1 ** two2
two6 _0 _0 *l two7  ;b<a 此时要把纸带上的X变成0 Y变成1
two7 _0 _0 *l two7
two7 _1 _1 *l two7
two7 _X _0 *l two7
two7 _Y _1 *l two7
two7 _S _S *r two8  ;到最左端，下面需要到达最右端重新开始
two8 _0 _0 *r two8
two8 _1 _1 *r two8
two8 _X _0 *r two8
two8 _Y _1 *r two8
two8 __ __ ** two1  ;重新再来

;two4 __ ;a<b X变1 0,1,Y变B
two4 _S _S *r two9
two4 __ __ *r two9
two9 _X _1 *r two9
two9 _0 __ *r two9
two9 _1 __ *r two9
two9 _Y __ *r two9
two9 __ __ ** three1

;step3 
three1 __ __ *l three1   ;tape为空是接受态
three1 _S _S ** halt_accept
three1 _1 _1 *l three2
three2 _1 _1 *l three2
three2 _S _S ** three3  ;到达tape2最左端
three3 _S 0_ rr three4
three4 _1 1_ rr three4
three4 __ __ l* three5
three5 1_ 1_ l* three5
three5 0_ 0_ l* three5
three5 __ __ r* q0
