

(define (problem blocksworld-24)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 - object)
 (:init 
    (arm-empty)
    (clear b4)
    (on b4 b3)
    (on b3 b1)
    (on b1 b7)
    (on b7 b6)
    (on b6 b5)
    (on b5 b2)
    (on-table b2))
 (:goal (and 
(on-table b4)
(on-table b7)
(on-table b5)
))
)
