

(define (problem blocksworld-41)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 b6 b7 b8 b9 b10 b11 b12 - object)
 (:init 
    (arm-empty)
    (clear b1)
    (on b1 b6)
    (on b6 b10)
    (on b10 b5)
    (on-table b5)
    (clear b3)
    (on b3 b7)
    (on b7 b11)
    (on b11 b4)
    (on b4 b2)
    (on-table b2)
    (clear b9)
    (on b9 b8)
    (on b8 b12)
    (on-table b12))
 (:goal (and 
(on-table b3)
(on-table b7)
(on b4 b3)
(on b10 b7)
(on b11 b4)
(clear b1) (on b1 b11)
(on b5 b10)
(on b8 b5)
(on b2 b8)
))
)
