

(define (problem blocksworld-20)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 - object)
 (:init 
    (arm-empty)
    (clear b5)
    (on b5 b2)
    (on b2 b6)
    (on b6 b4)
    (on b4 b1)
    (on b1 b3)
    (on-table b3))
 (:goal (and 
(on b1 b5)
))
)
