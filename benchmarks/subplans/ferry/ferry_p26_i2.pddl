

(define (problem ferry-26)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 - car
    loc1 loc2 loc3 loc4 loc5 loc6 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc5)
    (at car1 loc1)
    (at car2 loc4)
    (at car3 loc6)
    (at car4 loc5)
)
 (:goal (and 
(at car3 loc4)
))
)
