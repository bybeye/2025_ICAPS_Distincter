

(define (problem ferry-28)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc5)
    (at car1 loc2)
    (at car2 loc2)
    (at car3 loc6)
    (at car4 loc6)
)
 (:goal (and 
(at car1 loc3)
))
)
