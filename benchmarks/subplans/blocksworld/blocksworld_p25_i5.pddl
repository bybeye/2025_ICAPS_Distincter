

(define (problem blocksworld-25)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 - object)
 (:init 
    (arm-empty)
    (clear b6)
    (on-table b6)
    (clear b7)
    (on b7 b2)
    (on b2 b4)
    (on b4 b5)
    (on b5 b1)
    (on b1 b3)
    (on-table b3))
 (:goal (and 
(on b5 b1)
))
)
