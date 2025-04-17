

(define (problem blocksworld-26)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 b8 - object)
 (:init 
    (arm-empty)
    (clear b1)
    (on-table b1)
    (clear b2)
    (on-table b2)
    (clear b6)
    (on b6 b3)
    (on b3 b7)
    (on b7 b5)
    (on b5 b8)
    (on-table b8)
    (clear b4)
    (on-table b4))
 (:goal (and 
(on b4 b2)
))
)
