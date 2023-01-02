#Q = {a,b,c,d,e,f,g,h,i}

#S = {0,1}

#G = {0,1,_}

#q0 = a

#B = _

#F = {i}

#N = 2

a *_ *_ r* b
a __ __ l* i

b *_ *_ r* b
b __ __ l* d

d 0_ 00 lr e
d 1_ 11 lr e

e *_ *_ l* e
e __ __ r* f

f 0_ 00 rr f
f 1_ 11 rr f
f __ __ ll g

g ** ** ll g
g _* _* r* h

h *0 0_ rr h
h *1 1_ rr h
h _* __ ** c

c __ __ l* i

