

(define (problem blocksworld-33)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 b8 b9 b10 - object)
 (:init 
    (arm-empty)
    (clear b7)
    (on b7 b1)
    (on b1 b6)
    (on b6 b9)
    (on b9 b4)
    (on-table b4)
    (clear b2)
    (on b2 b8)
    (on-table b8)
    (clear b3)
    (on b3 b10)
    (on b10 b5)
    (on-table b5))
 (:goal (and 
(on-table b8)
(clear b1) (on b1 b6)
))
)
