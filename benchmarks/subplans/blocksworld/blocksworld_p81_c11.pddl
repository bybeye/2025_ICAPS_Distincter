

(define (problem blocksworld-81)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 b8 b9 b10 b11 b12 b13 b14 b15 b16 b17 b18 b19 b20 b21 b22 b23 b24 - object)
 (:init 
    (arm-empty)
    (clear b11)
    (on b11 b20)
    (on b20 b14)
    (on b14 b18)
    (on-table b18)
    (clear b8)
    (on b8 b24)
    (on b24 b1)
    (on-table b1)
    (clear b16)
    (on b16 b2)
    (on b2 b19)
    (on b19 b6)
    (on b6 b12)
    (on b12 b15)
    (on b15 b17)
    (on b17 b4)
    (on-table b4)
    (clear b10)
    (on b10 b22)
    (on b22 b7)
    (on b7 b21)
    (on b21 b9)
    (on b9 b13)
    (on b13 b3)
    (on b3 b23)
    (on b23 b5)
    (on-table b5))
 (:goal (and 
(on-table b1)
(on-table b11)
(on-table b17)
(on-table b23)
(on b12 b1)
(on b14 b11)
(on b15 b23)
(clear b21) (on b21 b15)
(on b24 b17)
(on b2 b14)
(on b3 b24)
(on b5 b2)
(on b6 b12)
))
)
