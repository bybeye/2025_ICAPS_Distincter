

(define (problem blocksworld-22)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 - object)
 (:init 
    (arm-empty)
    (clear b6)
    (on-table b6)
    (clear b5)
    (on b5 b2)
    (on b2 b7)
    (on b7 b4)
    (on-table b4)
    (clear b3)
    (on b3 b1)
    (on-table b1))
 (:goal (and 
(on-table b6)
(on b2 b5)
))
)
