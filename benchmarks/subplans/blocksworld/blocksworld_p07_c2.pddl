

(define (problem blocksworld-07)
 (:domain blocksworld)
 (:objects  b1 b2 b3 - object)
 (:init 
    (arm-empty)
    (clear b3)
    (on b3 b2)
    (on b2 b1)
    (on-table b1)
)
 (:goal (and 
(on-table b3)
(on b2 b3)
(clear b1) (on b1 b2)
))
)
