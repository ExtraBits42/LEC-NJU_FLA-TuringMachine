#Q = {a,b,c,c1,c2,accept,reject,clear1,clear2,preject,paccept,d1,d2,d3,d4,e1,e2,e3,f}

#S = {1}

#G = {1,_,f,a,l,s,e,t,r,u,e}

#q0 = a

#B = _

#F = {f}

#N = 2

a __ __ ** paccept
a 1_ 11 ** b

b 11 11 rr b
b 1_ 1_ ** c
b _1 _1 ** reject
b __ __ ** accept

c 1_ 11 *r c1

c1 1_ 11 ** c2

c2 11 11 *l c2
c2 1_ 1_ *r b

reject _1 _1 l* reject
reject 11 11 ** clear1

clear1 11 _1 l* clear1
clear1 _1 _1 r* preject

accept __ __ l* accept
accept 1_ 1_ ** clear2

clear2 1_ __ l* clear2
clear2 __ __ r* paccept

preject _1 f1 r* d1
d1 _1 a1 r* d2
d2 _1 l1 r* d3
d3 _1 s1 r* d4
d4 _1 e1 r* f

paccept __ t_ r* e1
e1 __ r_ r* e2
e2 __ u_ r* e3
e3 __ e_ r* f

