

(define (problem ferry-99)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 car6 car7 car8 car9 car10 car11 car12 car13 car14 car15 car16 car17 car18 car19 car20 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 loc8 loc9 loc10 loc11 loc12 loc13 loc14 loc15 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc10)
    (at car1 loc14)
    (at car2 loc13)
    (at car3 loc9)
    (at car4 loc5)
    (at car5 loc1)
    (at car6 loc15)
    (at car7 loc15)
    (at car8 loc15)
    (at car9 loc12)
    (at car10 loc7)
    (at car11 loc11)
    (at car12 loc4)
    (at car13 loc6)
    (at car14 loc14)
    (at car15 loc14)
    (at car16 loc4)
    (at car17 loc2)
    (at car18 loc12)
    (at car19 loc9)
    (at car20 loc3)
)
 (:goal (and 
(at car1 loc3)
(at car2 loc11)
(at car3 loc14)
(at car4 loc6)
(at car5 loc11)
))
)
