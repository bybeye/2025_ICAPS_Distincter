

(define (problem blocksworld-83)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 b8 b9 b10 b11 b12 b13 b14 b15 b16 b17 b18 b19 b20 b21 b22 b23 b24 b25 - object)
 (:init 
    (arm-empty)
    (clear b15)
    (on b15 b10)
    (on b10 b6)
    (on b6 b21)
    (on b21 b7)
    (on b7 b4)
    (on b4 b5)
    (on b5 b17)
    (on b17 b23)
    (on b23 b19)
    (on b19 b25)
    (on b25 b24)
    (on b24 b2)
    (on b2 b1)
    (on b1 b12)
    (on b12 b11)
    (on b11 b22)
    (on b22 b3)
    (on b3 b18)
    (on b18 b9)
    (on b9 b16)
    (on b16 b14)
    (on b14 b8)
    (on b8 b20)
    (on b20 b13)
    (on-table b13))
 (:goal (and 
(on b20 b1)
))
)
