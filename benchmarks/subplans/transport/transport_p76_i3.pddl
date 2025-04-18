

(define (problem transport-76)
 (:domain transport)
 (:objects 
    v1 v2 v3 v4 v5 v6 - vehicle
    p1 p2 p3 p4 p5 p6 p7 p8 p9 p10 p11 p12 p13 - package
    l1 l2 l3 l4 l5 l6 l7 l8 l9 l10 l11 l12 l13 l14 - location
    c0 c1 c2 - size
    )
 (:init (capacity v1 c1)
    (capacity v2 c1)
    (capacity v3 c1)
    (capacity v4 c1)
    (capacity v5 c2)
    (capacity v6 c1)
    (capacity-predecessor c0 c1)
    (capacity-predecessor c1 c2)
    (at p1 l9)
    (at p2 l7)
    (at p3 l12)
    (at p4 l2)
    (at p5 l12)
    (at p6 l9)
    (at p7 l10)
    (at p8 l6)
    (at p9 l6)
    (at p10 l8)
    (at p11 l13)
    (at p12 l11)
    (at p13 l1)
    (at v1 l3)
    (at v2 l6)
    (at v3 l7)
    (at v4 l2)
    (at v5 l3)
    (at v6 l7)
    (road l5 l4)
    (road l3 l10)
    (road l11 l2)
    (road l10 l3)
    (road l10 l9)
    (road l8 l12)
    (road l2 l11)
    (road l2 l14)
    (road l6 l5)
    (road l4 l5)
    (road l5 l6)
    (road l10 l2)
    (road l9 l10)
    (road l10 l8)
    (road l2 l10)
    (road l13 l10)
    (road l6 l7)
    (road l7 l6)
    (road l14 l2)
    (road l12 l8)
    (road l4 l10)
    (road l10 l4)
    (road l8 l10)
    (road l10 l1)
    (road l10 l13)
    (road l1 l10)
    (road l11 l12)
    (road l12 l11)
    (road l5 l13)
    (road l13 l5)
    (road l4 l11)
    (road l11 l4)
    (road l12 l14)
    (road l14 l12)
    (road l3 l8)
    (road l8 l3)
    (road l3 l6)
    (road l6 l3)
    (road l2 l6)
    (road l6 l2)
    (road l6 l11)
    (road l11 l6)
    (road l2 l12)
    (road l12 l2)
    (road l1 l12)
    (road l12 l1)
    (road l3 l14)
    (road l14 l3)
    (road l11 l13)
    (road l13 l11)
    (road l1 l6)
    (road l6 l1)
    (road l7 l14)
    (road l14 l7)
    (road l9 l12)
    (road l12 l9)
    (road l2 l3)
    (road l3 l2)
    (road l6 l13)
    (road l13 l6)
    (road l8 l11)
    (road l11 l8)
    (road l7 l13)
    (road l13 l7)
    (road l3 l4)
    (road l4 l3)
    (road l4 l8)
    (road l8 l4)
    (road l2 l5)
    (road l5 l2)
    (road l3 l5)
    (road l5 l3)
    (road l7 l11)
    (road l11 l7)
    (road l1 l11)
    (road l11 l1)
    (road l1 l8)
    (road l8 l1)
    (road l2 l13)
    (road l13 l2)
    (road l10 l11)
    (road l11 l10)
    (road l1 l13)
    (road l13 l1)
    (road l3 l13)
    (road l13 l3)
    (road l3 l9)
    (road l9 l3)
    (road l2 l8)
    (road l8 l2)
    (road l6 l14)
    (road l14 l6)
    )
 (:goal (and 
(at p4 l6)
))
)
