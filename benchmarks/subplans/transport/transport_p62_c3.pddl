

(define (problem transport-62)
 (:domain transport)
 (:objects 
    v1 v2 v3 v4 v5 - vehicle
    p1 p2 p3 p4 p5 p6 p7 p8 p9 p10 p11 - package
    l1 l2 l3 l4 l5 l6 l7 l8 l9 l10 l11 l12 - location
    c0 c1 c2 - size
    )
 (:init (capacity v1 c2)
    (capacity v2 c1)
    (capacity v3 c1)
    (capacity v4 c1)
    (capacity v5 c2)
    (capacity-predecessor c0 c1)
    (capacity-predecessor c1 c2)
    (at p1 l8)
    (at p2 l6)
    (at p3 l11)
    (at p4 l2)
    (at p5 l8)
    (at p6 l2)
    (at p7 l3)
    (at p8 l4)
    (at p9 l10)
    (at p10 l1)
    (at p11 l1)
    (at v1 l4)
    (at v2 l2)
    (at v3 l12)
    (at v4 l3)
    (at v5 l10)
    (road l12 l1)
    (road l3 l7)
    (road l1 l6)
    (road l11 l8)
    (road l1 l12)
    (road l6 l2)
    (road l9 l7)
    (road l5 l12)
    (road l8 l11)
    (road l6 l1)
    (road l1 l8)
    (road l7 l3)
    (road l7 l9)
    (road l12 l5)
    (road l4 l10)
    (road l8 l1)
    (road l8 l7)
    (road l10 l4)
    (road l10 l1)
    (road l2 l6)
    (road l1 l10)
    (road l7 l8)
    (road l1 l7)
    (road l7 l1)
    (road l4 l6)
    (road l6 l4)
    (road l2 l12)
    (road l12 l2)
    (road l11 l12)
    (road l12 l11)
    (road l10 l12)
    (road l12 l10)
    (road l4 l8)
    (road l8 l4)
    (road l3 l10)
    (road l10 l3)
    (road l4 l11)
    (road l11 l4)
    (road l6 l12)
    (road l12 l6)
    (road l5 l10)
    (road l10 l5)
    (road l2 l8)
    (road l8 l2)
    (road l2 l10)
    (road l10 l2)
    )
 (:goal (and 
(at p1 l6)
(at p2 l2)
(at p3 l9)
(at p4 l3)
))
)
