

(define (problem blocksworld-94)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 b8 b9 b10 b11 b12 b13 b14 b15 b16 b17 b18 b19 b20 b21 b22 b23 b24 b25 b26 b27 b28 - object)
 (:init 
    (arm-empty)
    (clear b24)
    (on b24 b5)
    (on b5 b9)
    (on b9 b2)
    (on b2 b16)
    (on b16 b25)
    (on b25 b4)
    (on b4 b8)
    (on b8 b15)
    (on b15 b28)
    (on b28 b27)
    (on b27 b18)
    (on b18 b12)
    (on-table b12)
    (clear b26)
    (on b26 b1)
    (on b1 b14)
    (on b14 b22)
    (on b22 b10)
    (on b10 b11)
    (on b11 b23)
    (on b23 b17)
    (on-table b17)
    (clear b20)
    (on b20 b6)
    (on b6 b21)
    (on-table b21)
    (clear b13)
    (on b13 b19)
    (on b19 b7)
    (on b7 b3)
    (on-table b3))
 (:goal (and 
(on b16 b21)
))
)
