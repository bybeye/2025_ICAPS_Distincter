

(define (problem blocksworld-06)
 (:domain blocksworld)
 (:objects  b1 b2 b3 - object)
 (:init 
    (arm-empty)
    (clear b3)
    (on-table b3)
    (clear b2)
    (on-table b2)
    (clear b1)
    (on-table b1)
)
 (:goal (and 
(on-table b1)
(on b2 b1)
))
)
