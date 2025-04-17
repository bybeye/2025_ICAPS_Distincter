

(define (problem blocksworld-16)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 - object)
 (:init 
    (arm-empty)
    (clear b2)
    (on b2 b5)
    (on b5 b4)
    (on b4 b3)
    (on b3 b1)
    (on-table b1))
 (:goal (and 
(on-table b5)
(on b4 b5)
))
)
