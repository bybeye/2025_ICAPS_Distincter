

(define (problem blocksworld-61)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 b8 b9 b10 b11 b12 b13 b14 b15 b16 b17 b18 - object)
 (:init 
    (arm-empty)
    (clear b4)
    (on b4 b5)
    (on b5 b14)
    (on b14 b18)
    (on b18 b10)
    (on b10 b17)
    (on b17 b2)
    (on b2 b8)
    (on b8 b6)
    (on b6 b12)
    (on b12 b9)
    (on-table b9)
    (clear b1)
    (on b1 b16)
    (on b16 b3)
    (on b3 b15)
    (on b15 b11)
    (on b11 b7)
    (on b7 b13)
    (on-table b13))
 (:goal (and 
(on-table b1)
(on-table b4)
(on b2 b4)
(on b8 b1)
(on b10 b2)
(on b15 b8)
))
)
