

(define (problem blocksworld-30)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 b8 b9 - object)
 (:init 
    (arm-empty)
    (clear b8)
    (on b8 b4)
    (on b4 b3)
    (on b3 b2)
    (on b2 b9)
    (on-table b9)
    (clear b7)
    (on b7 b5)
    (on b5 b6)
    (on b6 b1)
    (on-table b1))
 (:goal (and 
(clear b9) (on b9 b2)
))
)
