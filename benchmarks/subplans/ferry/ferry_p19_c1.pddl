

(define (problem ferry-19)
 (:domain ferry)
 (:objects 
    car1 car2 - car
    loc1 loc2 loc3 loc4 loc5 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc1)
    (at car1 loc3)
    (at car2 loc4)
)
 (:goal (and 
(at car1 loc1)
(at car2 loc3)
))
))