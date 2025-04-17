

(define (problem ferry-80)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 car6 car7 car8 car9 car10 car11 car12 car13 car14 car15 car16 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 loc8 loc9 loc10 loc11 loc12 loc13 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc7)
    (at car1 loc13)
    (at car2 loc10)
    (at car3 loc4)
    (at car4 loc7)
    (at car5 loc8)
    (at car6 loc12)
    (at car7 loc5)
    (at car8 loc11)
    (at car9 loc9)
    (at car10 loc12)
    (at car11 loc2)
    (at car12 loc9)
    (at car13 loc7)
    (at car14 loc9)
    (at car15 loc6)
    (at car16 loc12)
)
 (:goal (and 
(at car9 loc10)
))
)
