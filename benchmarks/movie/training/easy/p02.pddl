


(define (problem movie-2)
(:domain movie-strips)
(:objects c1 d1 p1 z1 k1 c2 d2 p2 z2 k2 )
(:init
(chips c1)
(chips c2)
(dip d1)
(dip d2)
(pop p1)
(pop p2)
(cheese z1)
(cheese z2)
(crackers k1)
(crackers k2)
(counter-at-other-than-two-hours)
)
(:goal
(and (movie-rewound)(counter-at-zero)(have-chips)(have-dip)(have-pop)(have-cheese)(have-crackers))
)
)


