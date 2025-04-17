;; blocks=b5, out_folder=testing/easy, instance_id=5, seed=11031

(define (problem blocksworld-b5)
 (:domain blocksworld)
 (:objects b1 b2 b3 b4 b5 - object)
 (:init 
    (arm-empty)
    (clear b5)
    (on-table b5)
    (clear b4)
    (on b4 b3)
    (on b3 b1)
    (on-table b1)
    (clear b2)
    (on-table b2))

 (:goal  (and 
    (clear b5)
    (on b5 b4)
    (on b4 b3)
    (on b3 b2)
    (on b2 b1)
    (on-table b1))))
