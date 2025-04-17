

(define (problem blocksworld-18)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 - object)
 (:init 
    (arm-empty)
    (clear b1)
    (on b1 b5)
    (on b5 b2)
    (on b2 b4)
    (on-table b4)
    (clear b3)
    (on-table b3))
 (:goal (and 
(on-table b4)
(on b5 b4)
))
)
