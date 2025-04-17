

(define (problem ferry-25)
 (:domain ferry)
 (:objects 
    car1 car2 car3 - car
    loc1 loc2 loc3 loc4 loc5 loc6 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc1)
    (at car1 loc2)
    (at car2 loc2)
    (at car3 loc6)
)
 (:goal (and 
(at car2 loc1)
))
)
