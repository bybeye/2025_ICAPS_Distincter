


(define (problem movie-1)
(:domain movie-strips)
(:objects c1 d1 p1 z1 k1 )
(:init
(chips c1)
(dip d1)
(pop p1)
(cheese z1)
(crackers k1)
(counter-at-other-than-two-hours)
)
(:goal
(and (movie-rewound)(counter-at-zero)(have-chips)(have-dip)(have-pop)(have-cheese)(have-crackers))
)
)


