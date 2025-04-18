

(define (problem miconic-59)
 (:domain miconic)
 (:objects 
    p1 p2 p3 p4 p5 p6 - passenger
    f1 f2 f3 f4 f5 f6 f7 f8 f9 f10 f11 f12 - floor
    )
 (:init 
    (lift-at f1)
    (origin p1 f9)
    (destin p1 f6)
    (origin p2 f12)
    (destin p2 f1)
    (origin p3 f2)
    (destin p3 f10)
    (origin p4 f12)
    (destin p4 f11)
    (origin p5 f5)
    (destin p5 f8)
    (origin p6 f2)
    (destin p6 f10)
    (above f1 f2)
    (above f1 f3)
    (above f1 f4)
    (above f1 f5)
    (above f1 f6)
    (above f1 f7)
    (above f1 f8)
    (above f1 f9)
    (above f1 f10)
    (above f1 f11)
    (above f1 f12)
    (above f2 f3)
    (above f2 f4)
    (above f2 f5)
    (above f2 f6)
    (above f2 f7)
    (above f2 f8)
    (above f2 f9)
    (above f2 f10)
    (above f2 f11)
    (above f2 f12)
    (above f3 f4)
    (above f3 f5)
    (above f3 f6)
    (above f3 f7)
    (above f3 f8)
    (above f3 f9)
    (above f3 f10)
    (above f3 f11)
    (above f3 f12)
    (above f4 f5)
    (above f4 f6)
    (above f4 f7)
    (above f4 f8)
    (above f4 f9)
    (above f4 f10)
    (above f4 f11)
    (above f4 f12)
    (above f5 f6)
    (above f5 f7)
    (above f5 f8)
    (above f5 f9)
    (above f5 f10)
    (above f5 f11)
    (above f5 f12)
    (above f6 f7)
    (above f6 f8)
    (above f6 f9)
    (above f6 f10)
    (above f6 f11)
    (above f6 f12)
    (above f7 f8)
    (above f7 f9)
    (above f7 f10)
    (above f7 f11)
    (above f7 f12)
    (above f8 f9)
    (above f8 f10)
    (above f8 f11)
    (above f8 f12)
    (above f9 f10)
    (above f9 f11)
    (above f9 f12)
    (above f10 f11)
    (above f10 f12)
    (above f11 f12)
)
 (:goal (and 
(served p6)
))
)
