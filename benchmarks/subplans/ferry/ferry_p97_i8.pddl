

(define (problem ferry-97)
 (:domain ferry)
 (:objects 
    car1 car2 car3 car4 car5 car6 car7 car8 car9 car10 car11 car12 car13 car14 car15 car16 car17 car18 car19 car20 - car
    loc1 loc2 loc3 loc4 loc5 loc6 loc7 loc8 loc9 loc10 loc11 loc12 loc13 loc14 loc15 - location
 )
 (:init 
    (empty-ferry)
    (at-ferry loc1)
    (at car1 loc15)
    (at car2 loc10)
    (at car3 loc10)
    (at car4 loc10)
    (at car5 loc14)
    (at car6 loc2)
    (at car7 loc1)
    (at car8 loc1)
    (at car9 loc5)
    (at car10 loc4)
    (at car11 loc7)
    (at car12 loc4)
    (at car13 loc5)
    (at car14 loc11)
    (at car15 loc11)
    (at car16 loc14)
    (at car17 loc15)
    (at car18 loc15)
    (at car19 loc5)
    (at car20 loc7)
)
 (:goal (and 
(at car9 loc2)
))
)
