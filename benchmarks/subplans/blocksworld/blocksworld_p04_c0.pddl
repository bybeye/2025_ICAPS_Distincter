

(define (problem blocksworld-04)
 (:domain blocksworld)
 (:objects  b1 b2 - object)
 (:init 
    (arm-empty)
    (clear b2)
    (on b2 b1)
    (on-table b1)
)
 (:goal (and 
(on-table b1)
(clear b1)
))
)