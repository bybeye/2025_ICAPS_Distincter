

(define (problem ferry-67)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 car6 car7 car8 car9 car10 car11 car12 car13 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 loc8 loc9 loc10 loc11 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc4)
    (at car1 loc7)
    (at car2 loc6)
    (at car3 loc1)
    (at car4 loc10)
    (at car5 loc1)
    (at car6 loc9)
    (at car7 loc1)
    (at car8 loc1)
    (at car9 loc5)
    (at car10 loc6)
    (at car11 loc1)
    (at car12 loc2)
    (at car13 loc11)
)
 (:goal (and 
(at car6 loc5)
))
)
