

(define (problem ferry-76)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 car6 car7 car8 car9 car10 car11 car12 car13 car14 car15 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 loc8 loc9 loc10 loc11 loc12 loc13 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc12)
    (at car1 loc8)
    (at car2 loc1)
    (at car3 loc9)
    (at car4 loc9)
    (at car5 loc8)
    (at car6 loc1)
    (at car7 loc13)
    (at car8 loc5)
    (at car9 loc13)
    (at car10 loc2)
    (at car11 loc4)
    (at car12 loc13)
    (at car13 loc8)
    (at car14 loc6)
    (at car15 loc7)
)
 (:goal (and 
(at car4 loc5)
))
)
