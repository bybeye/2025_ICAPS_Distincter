

(define (problem blocksworld-78)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 b8 b9 b10 b11 b12 b13 b14 b15 b16 b17 b18 b19 b20 b21 b22 b23 - object)
 (:init 
    (arm-empty)
    (clear b21)
    (on b21 b16)
    (on b16 b9)
    (on b9 b8)
    (on b8 b15)
    (on b15 b20)
    (on b20 b18)
    (on-table b18)
    (clear b13)
    (on b13 b7)
    (on b7 b2)
    (on b2 b22)
    (on b22 b14)
    (on b14 b6)
    (on b6 b3)
    (on b3 b5)
    (on b5 b11)
    (on b11 b17)
    (on b17 b10)
    (on b10 b1)
    (on b1 b4)
    (on b4 b23)
    (on b23 b12)
    (on b12 b19)
    (on-table b19))
 (:goal (and 
(on b19 b4)
))
)
