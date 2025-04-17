

(define (problem blocksworld-21)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 - object)
 (:init 
    (arm-empty)
    (clear b1)
    (on b1 b4)
    (on b4 b6)
    (on b6 b2)
    (on b2 b3)
    (on b3 b5)
    (on-table b5))
 (:goal (and 
(on-table b6)
(on b1 b6)
(on b4 b1)
(on b3 b4)
))
)
