

(define (problem blocksworld-31)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 b8 b9 - object)
 (:init 
    (arm-empty)
    (clear b3)
    (on b3 b5)
    (on b5 b8)
    (on b8 b1)
    (on b1 b2)
    (on b2 b7)
    (on b7 b6)
    (on b6 b9)
    (on b9 b4)
    (on-table b4))
 (:goal (and 
(on b8 b7)
))
)
