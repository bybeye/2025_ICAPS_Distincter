

(define (problem blocksworld-11)
 (:domain blocksworld)
 (:objects  b1 b2 b3 b4 - object)
 (:init 
    (arm-empty)
    (clear b1)
    (on b1 b2)
    (on-table b2)
    (clear b3)
    (on b3 b4)
    (on-table b4)
)
 (:goal (and 
(on-table b4)
(on b3 b4)
(on b1 b3)
))
)