

(define (problem ferry-34)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 car6 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc4)
    (at car1 loc1)
    (at car2 loc6)
    (at car3 loc5)
    (at car4 loc4)
    (at car5 loc5)
    (at car6 loc6)
)
 (:goal (and 
(at car2 loc2)
))
)
