

(define (problem ferry-71)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 car6 car7 car8 car9 car10 car11 car12 car13 car14 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 loc8 loc9 loc10 loc11 loc12 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc10)
    (at car1 loc4)
    (at car2 loc9)
    (at car3 loc6)
    (at car4 loc8)
    (at car5 loc1)
    (at car6 loc11)
    (at car7 loc9)
    (at car8 loc4)
    (at car9 loc10)
    (at car10 loc4)
    (at car11 loc5)
    (at car12 loc8)
    (at car13 loc12)
    (at car14 loc4)
)
 (:goal (and 
(at car3 loc2)
))
)
