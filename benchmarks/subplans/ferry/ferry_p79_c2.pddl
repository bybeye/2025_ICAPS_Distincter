

(define (problem ferry-79)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 car6 car7 car8 car9 car10 car11 car12 car13 car14 car15 car16 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 loc8 loc9 loc10 loc11 loc12 loc13 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc5)
    (at car1 loc4)
    (at car2 loc8)
    (at car3 loc8)
    (at car4 loc9)
    (at car5 loc1)
    (at car6 loc2)
    (at car7 loc4)
    (at car8 loc9)
    (at car9 loc8)
    (at car10 loc5)
    (at car11 loc10)
    (at car12 loc9)
    (at car13 loc2)
    (at car14 loc13)
    (at car15 loc4)
    (at car16 loc8)
)
 (:goal (and 
(at car1 loc10)
(at car2 loc5)
(at car3 loc7)
))
)
