

(define (problem ferry-21)
 (:domain ferry)
 (:objects 
    car1 car2 car3 - car
    loc1 loc2 loc3 loc4 loc5 loc6 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc2)
    (at car1 loc5)
    (at car2 loc5)
    (at car3 loc2)
)
 (:goal (and 
(at car1 loc2)
))
)
