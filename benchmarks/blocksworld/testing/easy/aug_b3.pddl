;; blocks=b3, out_folder=testing/easy, instance_id=3, seed=11031

(define (problem blocksworld-b3)
 (:domain blocksworld)
 (:objects b1 b2 b3 - object)
 (:init 
    (arm-empty)
    (clear b1)
    (on b1 b3)
    (on b3 b2)
    (on-table b2)
)

 (:goal  (and 
    (clear b2)
    (on b2 b3)
    (on b3 b1)
    (on-table b1))))
