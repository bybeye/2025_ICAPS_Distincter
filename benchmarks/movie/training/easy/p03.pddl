


(define (problem movie-3)
(:domain movie-strips)
(:objects c1 d1 p1 z1 k1 c2 d2 p2 z2 k2 c3 d3 p3 z3 k3 )
(:init
(chips c1)
(chips c2)
(chips c3)
(dip d1)
(dip d2)
(dip d3)
(pop p1)
(pop p2)
(pop p3)
(cheese z1)
(cheese z2)
(cheese z3)
(crackers k1)
(crackers k2)
(crackers k3)
(counter-at-other-than-two-hours)
)
(:goal
(and (movie-rewound)(counter-at-zero)(have-chips)(have-dip)(have-pop)(have-cheese)(have-crackers))
)
)


