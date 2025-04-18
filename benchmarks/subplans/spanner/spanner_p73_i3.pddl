

(define (problem spanner-73)
 (:domain spanner)
 (:objects 
    bob - man
    spanner1 spanner2 spanner3 spanner4 spanner5 spanner6 spanner7 - spanner
    nut1 nut2 nut3 nut4 - nut
    shed location1 location2 location3 location4 location5 location6 location7 location8 gate - location
 )
 (:init 
    (at bob shed)
    (at spanner1 location3)
    (usable spanner1)
    (at spanner2 location8)
    (usable spanner2)
    (at spanner3 location8)
    (usable spanner3)
    (at spanner4 location3)
    (usable spanner4)
    (at spanner5 location7)
    (usable spanner5)
    (at spanner6 location6)
    (usable spanner6)
    (at spanner7 location7)
    (usable spanner7)
    (at nut1 gate)
    (loose nut1)
    (at nut2 gate)
    (loose nut2)
    (at nut3 gate)
    (loose nut3)
    (at nut4 gate)
    (loose nut4)
    (link shed location1)
    (link location8 gate)
    (link location1 location2)
     (link location2 location3)
     (link location3 location4)
     (link location4 location5)
     (link location5 location6)
     (link location6 location7)
     (link location7 location8)
 )
 (:goal (and 
(tightened nut4)
))
)
