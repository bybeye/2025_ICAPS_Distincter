

(define (problem blocksworld-73)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 b8 b9 b10 b11 b12 b13 b14 b15 b16 b17 b18 b19 b20 b21 b22 - object)
 (:init 
    (arm-empty)
    (clear b4)
    (on b4 b17)
    (on b17 b7)
    (on b7 b3)
    (on b3 b10)
    (on b10 b19)
    (on b19 b1)
    (on b1 b22)
    (on b22 b8)
    (on b8 b11)
    (on b11 b16)
    (on b16 b20)
    (on b20 b2)
    (on b2 b18)
    (on b18 b9)
    (on b9 b14)
    (on b14 b12)
    (on-table b12)
    (clear b13)
    (on b13 b6)
    (on b6 b21)
    (on b21 b15)
    (on b15 b5)
    (on-table b5))
 (:goal (and 
(on-table b6)
(on-table b3)
(on-table b1)
(on-table b11)
(on b2 b11)
(on b7 b6)
(on b8 b1)
(on b10 b3)
(clear b12) (on b12 b2)
(on b15 b8)
))
)
