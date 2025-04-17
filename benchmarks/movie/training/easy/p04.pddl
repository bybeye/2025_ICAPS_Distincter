


(define (problem movie-4)
(:domain movie-strips)
(:objects c1 d1 p1 z1 k1 c2 d2 p2 z2 k2 c3 d3 p3 z3 k3 c4 d4 p4 z4 k4 )
(:init
(chips c1)
(chips c2)
(chips c3)
(chips c4)
(dip d1)
(dip d2)
(dip d3)
(dip d4)
(pop p1)
(pop p2)
(pop p3)
(pop p4)
(cheese z1)
(cheese z2)
(cheese z3)
(cheese z4)
(crackers k1)
(crackers k2)
(crackers k3)
(crackers k4)
(counter-at-other-than-two-hours)
)
(:goal
(and (movie-rewound)(counter-at-zero)(have-chips)(have-dip)(have-pop)(have-cheese)(have-crackers))
)
)


