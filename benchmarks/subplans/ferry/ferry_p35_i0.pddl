

(define (problem ferry-35)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 car6 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc4)
    (at car1 loc3)
    (at car2 loc3)
    (at car3 loc5)
    (at car4 loc7)
    (at car5 loc2)
    (at car6 loc4)
)
 (:goal (and 
(at car1 loc5)
))
)
