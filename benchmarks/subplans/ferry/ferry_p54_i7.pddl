

(define (problem ferry-54)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 car6 car7 car8 car9 car10 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 loc8 loc9 loc10 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc5)
    (at car1 loc1)
    (at car2 loc8)
    (at car3 loc1)
    (at car4 loc9)
    (at car5 loc6)
    (at car6 loc4)
    (at car7 loc8)
    (at car8 loc8)
    (at car9 loc9)
    (at car10 loc6)
)
 (:goal (and 
(at car8 loc4)
))
)
