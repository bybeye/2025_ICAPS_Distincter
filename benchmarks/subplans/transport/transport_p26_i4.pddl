

(define (problem transport-26)
 (:domain transport)
 (:objects 
    v1 v2 v3 v4 - vehicle
    p1 p2 p3 p4 p5 - package
    l1 l2 l3 l4 l5 l6 l7 l8 - location
    c0 c1 c2 - size
    )
 (:init (capacity v1 c1)
    (capacity v2 c1)
    (capacity v3 c2)
    (capacity v4 c2)
    (capacity-predecessor c0 c1)
    (capacity-predecessor c1 c2)
    (at p1 l7)
    (at p2 l5)
    (at p3 l7)
    (at p4 l8)
    (at p5 l5)
    (at v1 l4)
    (at v2 l1)
    (at v3 l3)
    (at v4 l5)
    (road l7 l4)
    (road l6 l5)
    (road l5 l8)
    (road l8 l7)
    (road l8 l1)
    (road l1 l8)
    (road l4 l7)
    (road l2 l5)
    (road l5 l6)
    (road l3 l6)
    (road l6 l3)
    (road l8 l5)
    (road l7 l8)
    (road l5 l2)
    (road l3 l7)
    (road l7 l3)
    (road l2 l7)
    (road l7 l2)
    (road l1 l4)
    (road l4 l1)
    (road l2 l6)
    (road l6 l2)
    (road l5 l7)
    (road l7 l5)
    (road l2 l8)
    (road l8 l2)
    (road l1 l3)
    (road l3 l1)
    (road l6 l7)
    (road l7 l6)
    (road l4 l5)
    (road l5 l4)
    (road l2 l3)
    (road l3 l2)
    (road l1 l5)
    (road l5 l1)
    (road l3 l8)
    (road l8 l3)
    (road l3 l5)
    (road l5 l3)
    (road l2 l4)
    (road l4 l2)
    (road l4 l8)
    (road l8 l4)
    (road l1 l2)
    (road l2 l1)
    (road l6 l8)
    (road l8 l6)
    (road l3 l4)
    (road l4 l3)
    (road l1 l6)
    (road l6 l1)
    (road l1 l7)
    (road l7 l1)
    )
 (:goal (and 
(at p5 l1)
))
)
