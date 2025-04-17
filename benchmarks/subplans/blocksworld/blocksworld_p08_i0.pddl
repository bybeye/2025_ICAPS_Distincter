

(define (problem blocksworld-08)
 (:domain blocksworld)
 (:objects  b1 b2 b3 - object)
 (:init 
    (arm-empty)
    (clear b1)
    (on b1 b2)
    (on b2 b3)
    (on-table b3)
)
 (:goal (and 
(on-table b1)
))
)
