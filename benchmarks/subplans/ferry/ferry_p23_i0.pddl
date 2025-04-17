

(define (problem ferry-23)
 (:domain ferry)
 (:objects 
    car1 car2 car3 - car
    loc1 loc2 loc3 loc4 loc5 loc6 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc5)
    (at car1 loc2)
    (at car2 loc4)
    (at car3 loc5)
)
 (:goal (and 
(at car1 loc5)
))
)
