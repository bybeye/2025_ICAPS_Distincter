

(define (problem ferry-52)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 car6 car7 car8 car9 car10 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 loc8 loc9 loc10 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc3)
    (at car1 loc8)
    (at car2 loc9)
    (at car3 loc5)
    (at car4 loc3)
    (at car5 loc3)
    (at car6 loc5)
    (at car7 loc3)
    (at car8 loc1)
    (at car9 loc7)
    (at car10 loc8)
)
 (:goal (and 
(at car9 loc6)
))
)
