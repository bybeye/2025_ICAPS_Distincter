

(define (problem ferry-90)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 car6 car7 car8 car9 car10 car11 car12 car13 car14 car15 car16 car17 car18 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 loc8 loc9 loc10 loc11 loc12 loc13 loc14 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc9)
    (at car1 loc4)
    (at car2 loc4)
    (at car3 loc11)
    (at car4 loc12)
    (at car5 loc10)
    (at car6 loc11)
    (at car7 loc8)
    (at car8 loc2)
    (at car9 loc6)
    (at car10 loc13)
    (at car11 loc4)
    (at car12 loc4)
    (at car13 loc14)
    (at car14 loc5)
    (at car15 loc12)
    (at car16 loc6)
    (at car17 loc6)
    (at car18 loc12)
)
 (:goal (and 
(at car1 loc3)
(at car2 loc2)
(at car3 loc13)
(at car4 loc11)
))
)
