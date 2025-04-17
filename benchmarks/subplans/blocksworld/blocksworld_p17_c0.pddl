

(define (problem blocksworld-17)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 - object)
 (:init 
    (arm-empty)
    (clear b3)
    (on b3 b2)
    (on b2 b5)
    (on-table b5)
    (clear b1)
    (on b1 b4)
    (on-table b4))
 (:goal (and 
(on-table b1)
))
)
