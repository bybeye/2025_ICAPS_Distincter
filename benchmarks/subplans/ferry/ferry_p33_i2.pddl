

(define (problem ferry-33)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc4)
    (at car1 loc4)
    (at car2 loc3)
    (at car3 loc3)
    (at car4 loc4)
    (at car5 loc6)
)
 (:goal (and 
(at car3 loc7)
))
)
