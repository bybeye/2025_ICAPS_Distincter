

(define (problem ferry-72)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 car6 car7 car8 car9 car10 car11 car12 car13 car14 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 loc8 loc9 loc10 loc11 loc12 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc3)
    (at car1 loc11)
    (at car2 loc10)
    (at car3 loc6)
    (at car4 loc3)
    (at car5 loc9)
    (at car6 loc12)
    (at car7 loc11)
    (at car8 loc10)
    (at car9 loc3)
    (at car10 loc7)
    (at car11 loc5)
    (at car12 loc10)
    (at car13 loc7)
    (at car14 loc10)
)
 (:goal (and 
(at car1 loc10)
(at car2 loc1)
(at car3 loc10)
(at car4 loc10)
))
)
