

(define (problem ferry-27)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 - car
    loc1 loc2 loc3 loc4 loc5 loc6 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc1)
    (at car1 loc3)
    (at car2 loc5)
    (at car3 loc5)
    (at car4 loc1)
)
 (:goal (and 
(at car4 loc5)
))
)
